#ifndef H_QueryOperator
#define H_QueryOperator

#include <ostream>
#include <unordered_set>
#include "InformationUnit.hpp"

typedef std::unordered_set<const InformationUnit*> IUSet;

class QueryOperator {
  public:
    virtual ~QueryOperator(){}

    virtual void produce(std::ostream& out, IUSet& requiredIUs, bool parallel) = 0;
    virtual void consume(std::ostream& out, const QueryOperator& sourceOp, bool parallel) = 0;
    virtual IUSet getProduced() const = 0;

    void setParent(QueryOperator* p) {parent = p;}

  protected:
    QueryOperator* parent = nullptr;
};

#endif
