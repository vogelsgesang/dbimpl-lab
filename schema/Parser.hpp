#ifndef H_Parser
#define H_Parser

#include <memory>
#include <exception>
#include <string>
#include <istream>
#include <sstream>
#include <experimental/optional>
#include "Schema.hpp"

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
    std::string fileName;
    std::istream& in;
    Parser(std::istream& in) : in(in), lineno(0) {}
    std::unique_ptr<Schema> parseSqlSchema();

  private:
    int lineno;

    std::experimental::optional<std::string> getNextToken();
    std::string expectToken();
    void expectToken(const char* expected);
    static bool isIdentifier(std::string token);

    std::string parseIdentifier();
    int parseInt();
    void parseTableDescriptionStatement(TableDescription* currentTable);
    void parseTableDescription(TableDescription* table);
};

#endif
