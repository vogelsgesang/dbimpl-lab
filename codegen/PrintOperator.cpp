#include "PrintOperator.hpp"
#include "utils/generateList.hpp"

void PrintOperator::produce(std::ostream& out, IUSet& requiredIUs) {
  auto requiredIUsCopy = requiredIUs;
  for(auto& iu : printedColumns) {
    requiredIUsCopy.insert(iu);
  }
  child->produce(out, requiredIUsCopy);
  out << "std::cout << std::flush;";
}

void PrintOperator::consume(std::ostream& out, const QueryOperator&) {
  out << "std::cout ";
  generateList(out, printedColumns, [](auto& out, auto& col) {
    out << " << " << col->getVarname();
  }, " << \"\\t|\\t\"");
  out << "<< \"\\n\";";
  if(parent) parent->consume(out, *this);
}

IUSet PrintOperator::getProduced() const {
  return child->getProduced();
}
