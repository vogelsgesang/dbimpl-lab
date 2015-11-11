#include "PrintOperator.hpp"

void PrintOperator::produce(std::ostream& out, IUSet& requiredIUs) {
  auto requiredIUsCopy = requiredIUs;
  for(auto& iu : printedColumns) {
    requiredIUsCopy.insert(iu);
  }
  child->produce(out, requiredIUsCopy);
}

void PrintOperator::consume(std::ostream& out, const QueryOperator&) {
  out << "std::cout ";
  for(const auto& col : printedColumns) {
    out << " << " << col->getVarname();
  }
  out << "<< std::endl;";
  if(parent) parent->consume(out, *this);
}

IUSet PrintOperator::getProduced() const {
  return child->getProduced();
}
