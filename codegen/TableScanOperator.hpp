#ifndef H_TableScanOperator
#define H_TableScanOperator

#include <unordered_map>
#include <string>
#include "QueryOperator.hpp"

class TableScanOperator : public QueryOperator {
  public:
    TableScanOperator(const TableDescription& tableDesc)
      : tableDesc(tableDesc) {}

    virtual void produce(std::ostream& out, IUSet& requiredIUs) override;
    virtual void consume(std::ostream& out, const QueryOperator& sourceOp) override;
    virtual IUSet getProduced() const override;

    const InformationUnit* getColumnIU(const std::string& colName);

  protected:
    const TableDescription tableDesc;
    std::unordered_map<std::string, InformationUnit> columnIUs;
    std::unordered_map<const InformationUnit*, std::string> iuColumnNames;
};

#endif
