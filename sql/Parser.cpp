#include "sql/Parser.hpp"
#include <algorithm>
#include <utility>
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
optional<string> Parser::getNextToken(bool casesensitive) {
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
      auto nextChar = in->peek();
      while(nextChar != EOF && seperators.find(nextChar) == string::npos) {
        token.push_back(nextChar);
        in->ignore();
        nextChar = in->peek();
      }
    }
    if(!casesensitive) {
      std::transform(token.begin(), token.end(), token.begin(), ::tolower);
    }
  }
  return token;
}
//-------------------------------------------------------------------
string Parser::expectToken(bool casesensitive) {
  auto token = getNextToken(casesensitive);
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
  auto firstToken = expectToken();
  auto& primaryKey = currentTable->primaryKey;
  auto& columns = currentTable->columns;
  if(firstToken == "primary") {
    auto secondToken = expectToken();
    if(secondToken == "__prefix__") {
      currentTable->primaryKeyPrefixIndexable = true;
      expectToken("key");
    } else if(secondToken != "key") {
      throw ParserError(lineno, string{"expected \"key\" or \"__prefix__\"; found: \""} + secondToken + "\"");
    }
    if(!primaryKey.empty()) {
      throw ParserError(lineno, "primary key already defined");
    }
    primaryKey = parseColumnList(*currentTable);
  } else if(isIdentifier(firstToken)) {
    columns.emplace_back(firstToken);
    auto& currentColumn = columns.back();
    columns.back().type = parseDataType();
    skipWhitespace();
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
void Parser::parseIndexDescription(Schema* schema, bool prefixIndexable) {
  auto idxName = parseIdentifier();
  expectToken("on");
  auto tableName = parseIdentifier();
  auto tableIter = std::find_if(schema->tables.begin(), schema->tables.end(),
                                [&tableName](auto& e){return e.name == tableName;});
  if(tableIter == schema->tables.cend()) {
    throw ParserError(lineno, string{"Unkown table: \""} + tableName + ")");
  }
  auto& table = *tableIter;
  table.indices.emplace_back(idxName);
  auto& index = table.indices.back();
  index.prefixIndexable = prefixIndexable;
  index.columns = parseColumnList(table);
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
        parseIndexDescription(schema.get());
      } else if(secondToken == "__prefix__") {
        expectToken("index");
        parseIndexDescription(schema.get(), true);
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
//-------------------------------------------------------------------
void Parser::parseWhereConditions(ast::Query* queryAst) {
  std::string lastToken;
  do {
    auto firstAttrName = expectToken();
    if(!isIdentifier(firstAttrName)) {
      throw ParserError(lineno, string{"Expected an identifier; found: \""} + firstAttrName + "\"");
    }
    expectToken("=");
    auto rhs = expectToken(true);
    if(rhs.find_first_not_of("0123456789") == string::npos){
      queryAst->filterPredicates.push_back(make_pair(firstAttrName, static_cast<uint64_t>(std::stoll(rhs))));
    } else if(rhs[0] == '\'' && rhs.size() >= 2 && rhs[rhs.size()-1] == '\''){
      queryAst->filterPredicates.push_back(make_pair(firstAttrName, rhs.substr(1, rhs.size()-2)));
    } else if(isIdentifier(rhs)) {
      queryAst->joinPredicates.push_back(make_pair(firstAttrName, rhs));
    } else  {
      throw ParserError(lineno, string{"Expected an identifier or a number; found: \""} + firstAttrName + "\"");
    }
    lastToken = expectToken();
  } while(lastToken == "and");
  if(lastToken != ";") {
    throw ParserError(lineno, string{"Expected \";\" or \"where\"; found \""} + lastToken + "\"");
  }
}
//-------------------------------------------------------------------
unique_ptr<ast::Query> Parser::parseQuery(std::istream& inStream) {
  this->in = &inStream;
  lineno = 1;
  auto queryAst = make_unique<ast::Query>();
  expectToken("select");
  // parse the list of columns
  std::string token;
  do {
    token = expectToken();
    if(!isIdentifier(token)) {
      throw ParserError(lineno, string{"Expected an identifier; found: \""} + token + "\"");
    }
    queryAst->selectedColumns.push_back(token);
    token = expectToken();
  } while(token == ",");
  //parse the list of tables
  if(token != "from") {
    throw ParserError(lineno, string{"Expected token \"from\"; found: \""} + token + "\"");
  }
  do {
    token = expectToken();
    if(!isIdentifier(token)) {
      throw ParserError(lineno, string{"Expected an identifier; found: \""} + token + "\"");
    }
    queryAst->tables.push_back(token);
    token = expectToken();
  } while(token == ",");
  if(token == "where") {
    parseWhereConditions(queryAst.get());
  } else if(token != ";") {
    throw ParserError(lineno, string{"Expected \";\" or \"where\"; found \""} + token + "\"");
  }
  auto lastToken = getNextToken();
  if(lastToken) {
    throw ParserError(lineno, string{"Expected end of statement; found \""} + *lastToken + "\"");
  }
  return queryAst;
}
