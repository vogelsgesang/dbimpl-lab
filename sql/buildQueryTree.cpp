#include "sql/buildQueryTree.hpp"
#include <unordered_map>
#include <string>
#include "codegen/PrintOperator.hpp"
#include "codegen/TableScanOperator.hpp"
#include "codegen/SelectionOperator.hpp"
#include "codegen/InnerHashJoinOperator.hpp"
#include "utils/tupleHash.hpp"

using namespace std;

static unique_ptr<QueryOperator> addFilters(
    unique_ptr<TableScanOperator>&& op,
    const std::vector<const std::pair<string, uint64_t>*>& predicates);

unique_ptr<QueryOperator> buildQueryTree(const ast::Query& query, const Schema& schema) {
  //maps table names to table descriptions
  auto tableDescriptions = unordered_map<string, const TableDescription*>{};
  //maps column names to corresponding table names
  auto knownColumnNames = unordered_map<string, string>{};
  for(const auto& tableName : query.tables) {
    const TableDescription* tableDesc;
    try {
      tableDesc = &schema.getTableByName(tableName);
    } catch(std::out_of_range) {
      throw string{"unknown table "} + tableName;
    }
    if(!tableDescriptions.insert(make_pair(tableName, tableDesc)).second) {
      throw string{"unable to scan table "} + tableName + " multiple times";
    }
    for(const auto& column : tableDesc->columns) {
      if(!knownColumnNames.insert(make_pair(column.name, tableName)).second) {
        throw string{"column name "} + column.name + " is not unique";
      }
    }
  }
  //map filter predicates to corresponding table scans
  auto filterPredicatesByTable = unordered_map<string, std::vector<const std::pair<string, uint64_t>*>>{};
  for(const auto& filterPred : query.filterPredicates) {
    auto tableNameIter = knownColumnNames.find(filterPred.first);
    if(tableNameIter == knownColumnNames.end()) {
      throw string{"unknown column "} + filterPred.first;
    }
    filterPredicatesByTable[tableNameIter->second].push_back(&filterPred);
  }
  //build the tree of joins
  auto tableScans = unordered_map<string, TableScanOperator*>{};
  auto joins = unordered_map<tuple<string, string>, InnerHashJoinOperator*>{};
  auto treeRoot = unique_ptr<QueryOperator>{};
  auto lastTableName = std::string{};
  for(const auto& tableName : query.tables) {
    auto tableScan = make_unique<TableScanOperator>(*tableDescriptions[tableName]);
    tableScans.insert(make_pair(tableName, tableScan.get()));
    if(!treeRoot) {
      treeRoot = addFilters(std::move(tableScan), filterPredicatesByTable[tableName]);
    } else {
      auto join = make_unique<InnerHashJoinOperator>(std::move(treeRoot),
                    addFilters(std::move(tableScan), filterPredicatesByTable[tableName])
                  );
      joins.insert(make_pair(make_tuple(lastTableName, tableName), join.get()));
      treeRoot = std::move(join);
    }
    lastTableName = tableName;
  }
  // add join conditions
  for(const auto& joinPred : query.joinPredicates) {
    auto tableName1Iter = knownColumnNames.find(joinPred.first);
    if(tableName1Iter == knownColumnNames.end()) {
      throw string{"unknown column "} + joinPred.first;
    }
    auto tableName2Iter = knownColumnNames.find(joinPred.second);
    if(tableName2Iter == knownColumnNames.end()) {
      throw string{"unknown column "} + joinPred.second;
    }
    auto& tableName1 = tableName1Iter->second;
    auto& tableName2 = tableName2Iter->second;
    const InformationUnit* iu1 = tableScans[tableName1]->getColumnIU(joinPred.first);
    const InformationUnit* iu2 = tableScans[tableName2]->getColumnIU(joinPred.second);
    auto joinOpIter = joins.find(make_tuple(tableName1, tableName2));
    if(joinOpIter != joins.end()) {
      joinOpIter->second->joinColumns.push_back(make_pair(iu1, iu2));
    } else {
      joinOpIter = joins.find(make_tuple(tableName2, tableName1));
      if(joinOpIter != joins.end()) {
        joinOpIter->second->joinColumns.push_back(make_pair(iu2, iu1));
      } else {
        throw string{"unable to place join condition on "} + joinPred.first + " and " + joinPred.second;
      }
    }
  }
  //add print operator at top of query tree
  auto selectedIUs = vector<const InformationUnit*>{};
  selectedIUs.reserve(query.selectedColumns.size());
  for(const auto& selectedColumn : query.selectedColumns) {
    auto tableNameIter = knownColumnNames.find(selectedColumn);
    if(tableNameIter == knownColumnNames.end()) {
      throw string{"unknown column "} + selectedColumn;
    }
    auto tableScan = tableScans[tableNameIter->second];
    selectedIUs.push_back(tableScan->getColumnIU(selectedColumn));
  }
  treeRoot = make_unique<PrintOperator>(selectedIUs, std::move(treeRoot));
  return treeRoot;
}


static unique_ptr<QueryOperator> addFilters(
    unique_ptr<TableScanOperator>&& op,
    const std::vector<const std::pair<string, uint64_t>*>& predicates)
{
  auto tableScanPtr = op.get();
  auto treeRoot = unique_ptr<QueryOperator>{std::move(op)};
  for(const auto predicate : predicates) {
    auto columnIU = tableScanPtr->getColumnIU(predicate->first);
    treeRoot = make_unique<SelectionOperator>(columnIU, predicate->second,
                                              std::move(treeRoot));
  }
  return treeRoot;
}
