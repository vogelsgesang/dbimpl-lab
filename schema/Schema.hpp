#ifndef H_Schema
#define H_Schema

#include <string>
#include <vector>

enum class DataType : char { Integer, Numeric, Char, VarChar, Date, Timestamp };

struct ColumnDescription {
  std::string name;
  DataType type;
  union {
    unsigned char charLen;
    struct {
      unsigned char integerPlaces;
      unsigned char decimalPlaces;
    } numeric;
  } typeAttributes;
  ColumnDescription(const std::string& name) : name(name) {}
};

struct TableDescription {
  std::string name;
  std::vector<ColumnDescription> columns;
  std::vector<std::string> primaryKeyColums;
  TableDescription(const std::string& name) : name(name) {}
};

struct Schema {
  std::vector<TableDescription> tables;
};

#endif
