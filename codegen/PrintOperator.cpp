#include "PrintOperator.hpp"
#include "utils/generateList.hpp"

void PrintOperator::produce(std::ostream& out, IUSet& requiredIUs, bool parallel) {
  auto requiredIUsCopy = requiredIUs;
  for(auto& iu : printedColumns) {
    requiredIUsCopy.insert(iu);
  }
  child->produce(out, requiredIUsCopy, parallel);
  out << "std::cout << std::flush;";
}

void PrintOperator::consume(std::ostream& out, const QueryOperator&, bool parallel) {
  out << "std::cout ";
  generateList(out, printedColumns, [](auto& out, auto& col) {
    out << " << " << col->getVarname();
  }, " << \"\\t|\\t\"");
  out << "<< \"\\n\";";
  if(parent) parent->consume(out, *this, parallel);
}

IUSet PrintOperator::getProduced() const {
  return child->getProduced();
}
