#include "TableScanOperator.hpp"


void TableScanOperator::produce(std::ostream& out, IUSet& requiredIUs) {
  out << "for(size_t tid = 0; tid < tables->" << tableDesc.name
      << ".col_" << iuColumnNames.at(*requiredIUs.begin()) << ".size(); tid++) {";
  for(const auto& iu : requiredIUs) {
    out << "auto& " << iu->getVarname() << " = tables->" << tableDesc.name
        << ".col_" << iuColumnNames.at(iu) << "[tid];";
  }
  parent->consume(out, *this);
  out << "}";
}

void TableScanOperator::consume(std::ostream&, const QueryOperator&) {
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
