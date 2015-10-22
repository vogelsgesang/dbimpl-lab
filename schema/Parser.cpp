#include "Parser.hpp"
#include <algorithm>
using namespace std;
using namespace std::experimental;
//-------------------------------------------------------------------
optional<string> Parser::getNextToken() {
  const auto whitespace = string{" \t\n"};
  const auto seperators = string{" \t\n,)(;"};
  //ignore leading whitespace
  while(in->good() && whitespace.find(in->peek()) != string::npos) {
    if(in->get() == '\n') lineno++;
  }
  //parse the actual token
  string token;
  char firstChar;
  if(!in->get(firstChar)) {
    return nullopt;
  }
  token.push_back(firstChar);
  if(seperators.find(firstChar) == string::npos) {
    while(in->good() && seperators.find(in->peek()) == string::npos) {
      token.push_back(in->get());
    }
  }
  std::transform(token.begin(), token.end(), token.begin(), ::tolower);
  return token;
}
//-------------------------------------------------------------------
string Parser::expectToken() {
  auto token = getNextToken();
  if(!token) {
    throw ParserError(lineno, "Unexpected end of stream");
  }
  return *token;
}
//-------------------------------------------------------------------
void Parser::expectToken(const char* expected) {
  auto token = expectToken();
  if(token != expected){
    throw ParserError(lineno, string{"Expected token \""} + expected + "\"; found: \"" + token + "\"");
  }
}
//-------------------------------------------------------------------
bool Parser::isIdentifier(string token) {
  if(token == "create"    ||
     token == "table"     ||
     token == "integer"   ||
     token == "numeric"   ||
     token == "char"      ||
     token == "varchar"   ||
     token == "timestamp" ||
     token == "not"       ||
     token == "null"      ||
     token == "primary"   ||
     token == "key") {
    return false;
  }
  return token.find_first_not_of("abcdefghijklmnopqrstuvwxyz_1234567890") == std::string::npos;
}
//-------------------------------------------------------------------
string Parser::parseIdentifier() {
  auto token = expectToken();
  if(!isIdentifier(token)) {
    throw ParserError(lineno, string{"Expected an identifier; found: \""} + token + "\"");
  }
  return token;
}
//-------------------------------------------------------------------
int Parser::parseInt() {
  auto token = expectToken();
  if(token.find_first_not_of("0123456789") != std::string::npos) {
    throw ParserError(lineno, string{"Expected an integer; found: \""} + token + "\"");
  }
  return std::atoi(token.c_str());
}
//-------------------------------------------------------------------
void Parser::parseTableDescriptionStatement(TableDescription* currentTable) {
  auto token = expectToken();
  auto& primaryKey = currentTable->primaryKey;
  auto& columns = currentTable->columns;
  if(token == "primary") {
    expectToken("key");
    expectToken("(");
    if(!primaryKey.empty()) {
      throw ParserError(lineno, "primary key already defined");
    }
    std::string trailingToken;
    do {
      std::string name = parseIdentifier();
      auto columnIdx = std::find_if(columns.begin(), columns.end(),
                                    [&name](auto& e){return e.name == name;}) - columns.begin();
      currentTable->primaryKey.push_back(columnIdx);
      trailingToken = expectToken();
    } while(trailingToken == ",");
    if(trailingToken != ")") {
      throw ParserError(lineno, string{"expected \")\"; found: \""} + token + ")");
    }
  } else if(isIdentifier(token)) {
    columns.emplace_back(token);
    auto& currentColumn = columns.back();
    auto dataType = expectToken();
    if(dataType == "integer") {
      currentColumn.type = DataType::Integer;
    } else if(dataType == "numeric") {
      currentColumn.type = DataType::Numeric;
      expectToken("(");
      currentColumn.typeAttributes.numeric.integerPlaces = parseInt();
      expectToken(",");
      currentColumn.typeAttributes.numeric.decimalPlaces = parseInt();
      expectToken(")");
    } else if(dataType == "char") {
      currentColumn.type = DataType::Char;
      expectToken("(");
      currentColumn.typeAttributes.charLen = parseInt();
      expectToken(")");
    } else if(dataType == "varchar") {
      currentColumn.type = DataType::VarChar;
      expectToken("(");
      currentColumn.typeAttributes.charLen = parseInt();
      expectToken(")");
    } else if(dataType == "timestamp") {
      currentColumn.type = DataType::Timestamp;
    } else {
      throw ParserError(lineno, string{"expected a valid type name; found: "} + dataType);
    }
    if(in->peek() != ',' && in->peek() != ')') {
      expectToken("not");
      expectToken("null");
      currentColumn.notNull = true;
    }
  }
}
//-------------------------------------------------------------------
void Parser::parseTableDescription(TableDescription* table) {
  expectToken("(");
  string token;
  do {
    parseTableDescriptionStatement(table);
    token = expectToken();
  } while(token == ",");
  if(token != ")") {
    throw ParserError(lineno, string{"expected \")\"; found: \""} + token + ")");
  }
  expectToken(";");
}
//-------------------------------------------------------------------
unique_ptr<Schema> Parser::parseSqlSchema(istream& inStream) {
  this->in = &inStream;
  lineno = 1;
  auto schema = make_unique<Schema>();
  while(auto firstToken = getNextToken()) {
    if(*firstToken == "create") {
      expectToken("table");
      schema->tables.emplace_back(parseIdentifier());
      parseTableDescription(&schema->tables.back());
    } else {
      throw ParserError(lineno, string{"Expected 'create'; found: '" + *firstToken + "'"});
    }
  }
  return schema;
}
