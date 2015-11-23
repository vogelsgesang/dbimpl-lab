#include "SelectionOperator.hpp"
#include <boost/variant/get.hpp>


void SelectionOperator::produce(std::ostream& out, IUSet& requiredIUs, bool parallel) {
  auto requiredIUsCopy = requiredIUs;
  requiredIUsCopy.insert(column);
  child->produce(out, requiredIUsCopy, parallel);
}

void SelectionOperator::consume(std::ostream& out, const QueryOperator&, bool parallel) {
  out << "if(" << column->getVarname() << " == ";
  switch(value.which()) {
    case 0:
      out << value;
      break;
    case 1:
      {
        auto& stringValue = boost::get<std::string>(value);
        // protection against code injection attacks
        if(stringValue.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWabcdefghijklmnopqrstuvw0123456789_") != std::string::npos) {
          throw "unexpected character in string";
        }
        out << '"' << stringValue << '"';
        break;
      }
    default: throw "unexpected value type";
  }
  out << ") {";
  parent->consume(out, *this, parallel);
  out << "}";
}

IUSet SelectionOperator::getProduced() const {
  return child->getProduced();
}
