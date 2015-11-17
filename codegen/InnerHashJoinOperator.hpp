#ifndef H_InnerHashJoinOperator
#define H_InnerHashJoinOperator

#include <memory>
#include "QueryOperator.hpp"

class InnerHashJoinOperator : public QueryOperator {
  public:
    InnerHashJoinOperator(const std::vector<std::pair<const InformationUnit*, const InformationUnit*>>& joinColumns,
                          std::unique_ptr<QueryOperator> left,
                          std::unique_ptr<QueryOperator> right)
      : varnameId(nextVarnameId++)
      , left(std::move(left))
      , right(std::move(right))
      , joinColumns(joinColumns)
    {
      this->left->setParent(this);
      this->right->setParent(this);
    }

    InnerHashJoinOperator(std::unique_ptr<QueryOperator> left,
                          std::unique_ptr<QueryOperator> right)
    : InnerHashJoinOperator({}, std::move(left), std::move(right)) {}

    virtual void produce(std::ostream& out, IUSet& requiredIUs) override;
    virtual void consume(std::ostream& out, const QueryOperator& sourceOp) override;
    virtual IUSet getProduced() const override;

  protected:
    const uint64_t varnameId;
    static uint64_t nextVarnameId;
    std::unique_ptr<QueryOperator> left, right;
    IUSet leftRequiredIUs;
  public:
    std::vector<std::pair<const InformationUnit*, const InformationUnit*>> joinColumns;
};

#endif
