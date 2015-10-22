#include "Parser.hpp"
#include <algorithm>
using namespace std;
using namespace std::experimental;
//-------------------------------------------------------------------
void Parser::skipWhitespace() {
  const auto whitespace = string{" \t\n"};
  while(in->good() && whitespace.find(in->peek()) != string::npos) {
    if(in->get() == '\n') lineno++;
  }
}
//-------------------------------------------------------------------
optional<string> Parser::getNextToken() {
  const auto seperators = string{" \t\n,)(;\""};
  skipWhitespace();
  //parse the actual token
  string token;
  char firstChar;
  if(!in->get(firstChar)) {
    return nullopt;
  }
  if(firstChar == '"') {
    while(in->get(firstChar) && firstChar != '"') {
      token.push_back(firstChar);
    }
  } else {
    token.push_back(firstChar);
    if(seperators.find(firstChar) == string::npos) {
      while(in->good() && seperators.find(in->peek()) == string::npos) {
        token.push_back(in->get());
      }
    }
    std::transform(token.begin(), token.end(), token.begin(), ::tolower);
  }
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
DataType Parser::parseDataType() {
  DataType type;
  auto token = expectToken();
  if(token == "integer") {
    type.typeTag = DataTypeTag::Integer;
  } else if(token == "numeric") {
    type.typeTag = DataTypeTag::Numeric;
    expectToken("(");
    type.attributes.numeric.integerPlaces = parseInt();
    expectToken(",");
    type.attributes.numeric.decimalPlaces = parseInt();
    expectToken(")");
  } else if(token == "char") {
    type.typeTag = DataTypeTag::Char;
    expectToken("(");
    type.attributes.charLen = parseInt();
    expectToken(")");
  } else if(token == "varchar") {
    type.typeTag = DataTypeTag::VarChar;
    expectToken("(");
    type.attributes.charLen = parseInt();
    expectToken(")");
  } else if(token == "timestamp") {
    type.typeTag = DataTypeTag::Timestamp;
  } else {
    throw ParserError(lineno, string{"expected a valid type name; found: "} + token);
  }
  return type;
}
//-------------------------------------------------------------------
std::vector<unsigned> Parser::parseColumnList(const TableDescription& table) {
  std::vector<unsigned> idList;
  expectToken("(");
  std::string trailingToken;
  do {
    std::string name = parseIdentifier();
    auto columnIdx = std::find_if(table.columns.cbegin(), table.columns.cend(),
                                  [&name](auto& e){return e.name == name;}) - table.columns.cbegin();
    idList.push_back(columnIdx);
    trailingToken = expectToken();
  } while(trailingToken == ",");
  if(trailingToken != ")") {
    throw ParserError(lineno, string{"expected \")\"; found: \""} + trailingToken + "\"");
  }
  return idList;
}
//-------------------------------------------------------------------
void Parser::parseTableDescriptionStatement(TableDescription* currentTable) {
  auto token = expectToken();
  auto& primaryKey = currentTable->primaryKey;
  auto& columns = currentTable->columns;
  if(token == "primary") {
    expectToken("key");
    if(!primaryKey.empty()) {
      throw ParserError(lineno, "primary key already defined");
    }
    primaryKey = parseColumnList(*currentTable);
  } else if(isIdentifier(token)) {
    columns.emplace_back(token);
    auto& currentColumn = columns.back();
    columns.back().type = parseDataType();
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
    throw ParserError(lineno, string{"expected \")\"; found: \""} + token + "\"");
  }
}
//-------------------------------------------------------------------
void Parser::parseIndexDescription(const Schema& schema, IndexDescription* index) {
  expectToken("on");
  std::string tableName = parseIdentifier();
  auto tableIter = std::find_if(schema.tables.begin(), schema.tables.end(),
                                [&tableName](auto& e){return e.name == tableName;});
  if(tableIter == schema.tables.cend()) {
    throw ParserError(lineno, string{"Unkown table: \""} + tableName + ")");
  }
  index->tableName = tableName;
  index->tableNr = tableIter - schema.tables.cbegin();
  index->columns = parseColumnList(*tableIter);
}
//-------------------------------------------------------------------
unique_ptr<Schema> Parser::parseSqlSchema(istream& inStream) {
  this->in = &inStream;
  lineno = 1;
  auto schema = make_unique<Schema>();
  while(auto firstToken = getNextToken()) {
    if(*firstToken == "create") {
      auto secondToken = expectToken();
      if(secondToken == "table") {
        schema->tables.emplace_back(parseIdentifier());
        parseTableDescription(&schema->tables.back());
      } else if(secondToken == "index") {
        schema->indices.emplace_back(parseIdentifier());
        parseIndexDescription(*schema, &schema->indices.back());
      } else {
        throw ParserError(lineno, string{"expected \"Table\" or \"index\"; found: \""} + secondToken + "\"");
      }
    } else {
      throw ParserError(lineno, string{"Expected 'create'; found: '" + *firstToken + "'"});
    }
    expectToken(";");
  }
  return schema;
}
