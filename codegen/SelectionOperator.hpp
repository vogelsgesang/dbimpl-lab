#ifndef H_SelectionOperator
#define H_SelectionOperator

#include <memory>
#include <unordered_map>
#include "QueryOperator.hpp"

class SelectionOperator : public QueryOperator {
  public:
    SelectionOperator(const InformationUnit* column, uint64_t value, std::unique_ptr<QueryOperator> child)
      : child(std::move(child))
      , column(column)
      , value(value)
    {
      this->child->setParent(this);
    }

    virtual void produce(std::ostream& out, IUSet& requiredIUs) override;
    virtual void consume(std::ostream& out, const QueryOperator& sourceOp) override;
    virtual IUSet getProduced() const override;

  protected:
    std::unique_ptr<QueryOperator> child;
    const InformationUnit* column;
    uint64_t value;
};

#endif
