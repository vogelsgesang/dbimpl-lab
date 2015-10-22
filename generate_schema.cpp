#include <iostream>
#include "schema/Schema.hpp"
#include "schema/Parser.hpp"

using namespace std;

int main() {
  auto parser = Parser{};

  try {
    auto schema = parser.parseSqlSchema(cin);
    schema->generateCppCode(cout);
  } catch(ParserError& e) {
    cout << e.what() << endl;
  }
}
