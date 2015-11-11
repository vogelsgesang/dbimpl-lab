#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include "schema/Schema.hpp"
#include "schema/Parser.hpp"
#include "schema/Types.hpp"
#include "codegen/TableScanOperator.hpp"
#include "codegen/PrintOperator.hpp"
#include "codegen/SelectionOperator.hpp"
#include "codegen/InnerHashJoinOperator.hpp"

using namespace std;

int main() {
  auto schema_parser = Parser{};
  std::unique_ptr<Schema> schema;
  try {
    schema = schema_parser.parseSqlSchema(std::ifstream{"data/schema.sql"});
  } catch(ParserError& e) {
    cout << e.what() << endl;
  }

  auto customerTs = std::make_unique<TableScanOperator>(schema->getTableByName("customer"));
  auto orderTs = std::make_unique<TableScanOperator>(schema->getTableByName("order"));
  auto orderlineTs = std::make_unique<TableScanOperator>(schema->getTableByName("orderline"));

  // we need to hold pointers to the TableScanOperators since ownership will be given away to the parent operators
  auto customerTsPtr = customerTs.get();
  auto orderTsPtr = orderTs.get();
  auto orderlineTsPtr = orderlineTs.get();

  //select the correct customer
  auto filteredCustomer = std::make_unique<SelectionOperator>(
    customerTsPtr->getColumnIU("c_id"), 322,
    std::make_unique<SelectionOperator>(
      customerTsPtr->getColumnIU("c_w_id"), 1,
      std::make_unique<SelectionOperator>(
        customerTsPtr->getColumnIU("c_d_id"), 1,
        std::move(customerTs)
      )
    )
  );

  //join order and order_line
  auto joinedColumns = std::make_unique<InnerHashJoinOperator>(
    std::vector<std::pair<const InformationUnit*, const InformationUnit*>>{
      {orderTsPtr->getColumnIU("o_w_id"), orderlineTsPtr->getColumnIU("ol_w_id")},
      {orderTsPtr->getColumnIU("o_d_id"), orderlineTsPtr->getColumnIU("ol_d_id")},
      {orderTsPtr->getColumnIU("o_id"  ), orderlineTsPtr->getColumnIU("ol_o_id")},
    },
    std::make_unique<InnerHashJoinOperator>(
      std::vector<std::pair<const InformationUnit*, const InformationUnit*>>{
        {customerTsPtr->getColumnIU("c_w_id"), orderTsPtr->getColumnIU("o_w_id")},
        {customerTsPtr->getColumnIU("c_d_id"), orderTsPtr->getColumnIU("o_d_id")},
        {customerTsPtr->getColumnIU("c_id"  ), orderTsPtr->getColumnIU("o_c_id")}
      },
      std::move(filteredCustomer),
      std::move(orderTs)
    ),
    std::move(orderlineTs)
  );

  //add the PrintOperator
  auto queryTree = std::make_unique<PrintOperator>(
    std::vector<const InformationUnit*>{
      customerTsPtr->getColumnIU("c_first"),
      customerTsPtr->getColumnIU("c_last"),
      orderTsPtr->getColumnIU("o_all_local"),
      orderlineTsPtr->getColumnIU("ol_amount")
    },
    std::move(joinedColumns)
  );

  std::ostream& out = std::cout;
  out << "#include <unordered_map>\n"
         "#include <iostream>\n"
         "#include \"schema/Types.hpp\"\n"
         "#include \"schema/GeneratedSchema.hpp\"\n"
         "void query(table_data* tables) {";
  auto noRequiredCols = IUSet{};
  queryTree->produce(out, noRequiredCols);
  out << "}";
}
