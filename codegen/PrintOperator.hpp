#ifndef H_PrintOperator
#define H_PrintOperator

#include <memory>
#include <vector>
#include "QueryOperator.hpp"

class PrintOperator : public QueryOperator {
  public:
    PrintOperator(const std::vector<const InformationUnit*>& columns,
                  std::unique_ptr<QueryOperator> child)
      : child(std::move(child))
    {
      this->printedColumns = columns;
      this->child->setParent(this);
    }

    virtual void produce(std::ostream& out, IUSet& requiredIUs) override;
    virtual void consume(std::ostream& out, const QueryOperator& sourceOp) override;
    virtual IUSet getProduced() const override;

  protected:
    std::vector<const InformationUnit*> printedColumns;
    std::unique_ptr<QueryOperator> child;
};

#endif
