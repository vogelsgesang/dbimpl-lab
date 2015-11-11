#include "SelectionOperator.hpp"


void SelectionOperator::produce(std::ostream& out, IUSet& requiredIUs) {
  auto requiredIUsCopy = requiredIUs;
  requiredIUsCopy.insert(column);
  child->produce(out, requiredIUsCopy);
}

void SelectionOperator::consume(std::ostream& out, const QueryOperator&) {
  out << "if(" << column->getVarname() << " == " << value << ") {";
  parent->consume(out, *this);
  out << "}";
}

IUSet SelectionOperator::getProduced() const {
  return child->getProduced();
}
