#ifndef H_Parser
#define H_Parser

#include <memory>
#include <exception>
#include <string>
#include <istream>
#include <sstream>
#include <experimental/optional>
#include "schema/Schema.hpp"
#include "QueryAst.hpp"

class ParserError : std::exception {
  public:
    ParserError(unsigned line, const std::string& m) : line(line) {
      std::ostringstream stream;
      stream << "Line " << line << ": " << m;
      msg = stream.str();
    }
    const char* what() const throw() {
      return msg.c_str();
    }
  private:
    std::string msg;
    unsigned line;
};

class Parser {
  public:
    std::unique_ptr<Schema> parseSqlSchema(std::istream& in);
    std::unique_ptr<Schema> parseSqlSchema(std::istream&& in) { return parseSqlSchema(in); }
    std::unique_ptr<ast::Query> parseQuery(std::istream& in);
    std::unique_ptr<ast::Query> parseQuery(std::istream&& in) { return parseQuery(in); }

  private:
    std::istream* in;
    int lineno;

    void skipWhitespace();
    std::experimental::optional<std::string> getNextToken(bool casesensitive = false);
    std::string expectToken(bool casesensitive = false);
    void expectToken(const char* expected);
    static bool isIdentifier(std::string token);
    std::string parseIdentifier();
    int parseInt();

    DataType parseDataType();
    std::vector<unsigned> parseColumnList(const TableDescription& table);
    void parseTableDescriptionStatement(TableDescription* currentTable);
    void parseTableDescription(TableDescription* table);
    void parseIndexDescription(Schema* schema, bool prefixIndexable = false);
    void parseWhereConditions(ast::Query* queryAst);
};

#endif
