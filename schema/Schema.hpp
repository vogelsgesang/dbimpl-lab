#ifndef H_Schema
#define H_Schema

#include <string>
#include <vector>
#include <ostream>

enum class DataTypeTag : char { Integer, Numeric, Char, VarChar, Date, Timestamp };

struct DataType {
  DataTypeTag typeTag;
  union {
    unsigned int charLen;
    struct {
      unsigned char integerPlaces;
      unsigned char decimalPlaces;
    } numeric;
  } attributes;

  std::string generateCppCode() const;
};

struct ColumnDescription {
  std::string name;
  bool notNull;
  DataType type;
  ColumnDescription(const std::string& name) : name(name), notNull(false) {}
};

struct IndexDescription {
  std::string name;
  std::vector<unsigned> columns;
  bool prefixIndexable = false;
  IndexDescription(const std::string& name) : name(name) {}
};

struct TableDescription {
  std::string name;
  std::vector<ColumnDescription> columns;
  std::vector<unsigned> primaryKey;
  bool primaryKeyPrefixIndexable = false;
  std::vector<IndexDescription> indices;
  TableDescription(const std::string& name) : name(name) {}
  const ColumnDescription& getColumnByName(const std::string& name) const;
};

struct Schema {
  std::vector<TableDescription> tables;
  std::vector<IndexDescription> indices;

  const TableDescription& getTableByName(const std::string& name) const;
  void generateCppCode(std::ostream& out);
};

#endif
