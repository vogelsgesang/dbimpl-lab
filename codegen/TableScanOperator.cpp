#include "TableScanOperator.hpp"


void TableScanOperator::produce(std::ostream& out, IUSet& requiredIUs, bool parallel) {
  std::string sizeColumn;
  if(requiredIUs.size()) sizeColumn = iuColumnNames.at(*requiredIUs.begin());
  else sizeColumn = tableDesc.columns.front().name;
  if(parallel) {
    out << "tbb::parallel_for(tbb::blocked_range<size_t>(0, tables->" << tableDesc.name << ".col_"
        << sizeColumn << ".size()), [&](const tbb::blocked_range<size_t>& range){"
           "for(size_t tid = range.begin(); tid != range.end(); tid++) {";
  } else {
    out << "for(size_t tid = 0; tid < tables->" << tableDesc.name
        << ".col_" << sizeColumn << ".size(); tid++) {";
  }

  for(const auto& iu : requiredIUs) {
    out << "auto& " << iu->getVarname() << " = tables->" << tableDesc.name
        << ".col_" << iuColumnNames.at(iu) << "[tid];";
  }
  parent->consume(out, *this, parallel);
  out << "}";
  if(parallel) out << "});";
}

void TableScanOperator::consume(std::ostream&, const QueryOperator&, bool parallel) {
  throw "TableScanOperator::consume was called";
}

const InformationUnit* TableScanOperator::getColumnIU(const std::string& colName) {
  if(columnIUs.count(colName)) {
    return &columnIUs.find(colName)->second;
  } else {
    auto& type = tableDesc.getColumnByName(colName).type;
    auto iter = std::get<0>(columnIUs.emplace(std::make_pair(colName, InformationUnit{type, colName})));
    auto& iu = iter->second;
    iuColumnNames[&iu] = colName;
    return &iu;
  }
}

IUSet TableScanOperator::getProduced() const {
  auto produced = IUSet{};
  produced.reserve(columnIUs.size());
  for(const auto& entry : columnIUs) {
    produced.insert(&entry.second);
  }
  return produced;
}
