#ifndef H_Schema
#define H_Schema

#include <string>
#include <vector>
#include <ostream>

enum class DataTypeTag : char { Integer, Numeric, Char, VarChar, Date, Timestamp };

struct DataType {
  DataTypeTag typeTag;
  union {
    unsigned char charLen;
    struct {
      unsigned char integerPlaces;
      unsigned char decimalPlaces;
    } numeric;
  } attributes;
};

struct ColumnDescription {
  std::string name;
  bool notNull;
  DataType type;
  ColumnDescription(const std::string& name) : name(name), notNull(false) {}
};

struct TableDescription {
  std::string name;
  std::vector<ColumnDescription> columns;
  std::vector<unsigned> primaryKey;
  TableDescription(const std::string& name) : name(name) {}
};

struct IndexDescription {
  std::string name;
  int tableNr;
  std::string tableName;
  std::vector<unsigned> columns;
  IndexDescription(const std::string& name) : name(name) {}
};

struct Schema {
  std::vector<TableDescription> tables;
  std::vector<IndexDescription> indices;
  void generateCppCode(std::ostream& out);
};

#endif
