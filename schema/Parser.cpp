#include "Parser.hpp"
#include <algorithm>
using namespace std;
using namespace std::experimental;
//-------------------------------------------------------------------
optional<string> Parser::getNextToken() {
  const auto whitespace = string{" \t\n"};
  const auto seperators = string{" \t\n,)(;"};
  //ignore leading whitespace
  while(in.good() && whitespace.find(in.peek()) != string::npos) in.ignore();
  //make sure, the input stream did not reach its end yet
  if(!in.good()) {
    return nullopt;
  }
  //parse the actual token
  ostringstream token_content;
  do {
    token_content << in.get();
  } while(in.good() && seperators.find(in.peek()) != string::npos);
  string token = token_content.str();
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
  if(token.find_first_not_of("0123456789") == std::string::npos) {
    throw ParserError(lineno, string{"Expected an integer; found: \""} + token + "\"");
  }
  return std::atoi(token.c_str());
}
//-------------------------------------------------------------------
void Parser::parseTableDescriptionStatement(TableDescription* currentTable) {
  auto token = expectToken();
  if(token == "primary") {
    expectToken("key");
    expectToken("(");
    if(!currentTable->primaryKeyColums.empty()) {
      throw ParserError(lineno, "primary key already defined");
    }
    std::string token = expectToken();
    while(isIdentifier(token)) {
      currentTable->primaryKeyColums.push_back(token);
      expectToken(",");
    }
    if(token != ")") {
      throw ParserError(lineno, string{"expected \")\"; found: \""} + token + ")");
    }
  } else if(isIdentifier(token)) {
    currentTable->columns.emplace_back(token);
    auto& currentColumn = currentTable->columns.back();
    auto dataType = expectToken();
    if(dataType == "integer") {
      currentColumn.type = DataType::Integer;
    } else if(dataType == "numeric") {
      currentColumn.type = DataType::Integer;
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
    expectToken("not");
    expectToken("null");
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
  expectToken(")");
  expectToken(";");
}
//-------------------------------------------------------------------
unique_ptr<Schema> Parser::parseSqlSchema() {
  auto schema = make_unique<Schema>();
  while(auto firstToken= getNextToken()) {
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
