#include "Schema.hpp"
#include <sstream>

static std::string translateType(const DataType& type) {
  std::ostringstream stream;
  if(type.typeTag == DataTypeTag::Integer) {
    stream << "Integer";
  } else if(type.typeTag == DataTypeTag::Numeric) {
    stream << "Numeric<" << (int) type.attributes.numeric.integerPlaces
           << "," << (int) type.attributes.numeric.decimalPlaces << ">";
  } else if(type.typeTag == DataTypeTag::Char) {
    stream << "Char<" << (int) type.attributes.charLen << ">";
  } else if(type.typeTag == DataTypeTag::VarChar) {
    stream << "Varchar<" << (int) type.attributes.charLen << ">";
  } else if(type.typeTag == DataTypeTag::Date) {
    stream << "Date";
  } else if(type.typeTag == DataTypeTag::Timestamp) {
    stream << "Timestamp";
  }
  return stream.str();
}

template<typename T, typename Callable>
static void generateList(std::ostream& out, const std::vector<T>& elements, Callable cb, std::string seperator = ", ") {
  auto first = true;
  for(const auto& elem : elements) {
    if(!first) out << seperator;
    cb(out, elem);
    first = false;
  }
}

void Schema::generateCppCode(std::ostream& out) {
  out <<
    "#ifndef H_Schema\n"
    "#define H_Schema\n"
    "#include <vector>\n"
    "#include <map>\n"
    "#include <unordered_map>\n"
    "#include <schema/Types.hpp>\n";
  for(auto& table : tables) {
    out <<
      "//--------------------------------------------------\n"
      "struct " << table.name << "_t {\n";
    // actual columns
    for(auto& column : table.columns) {
      out << "  std::vector<" << translateType(column.type) << "> col_" << column.name << ";\n";
    }
    // primary key index
    if(!table.primaryKey.empty()) {
      out << "  std::unordered_map<std::tuple<";
      generateList(out, table.primaryKey, [&](auto& out, auto& col){
               out << translateType(table.columns[col].type);
          });
      out << ">, size_t> primary_key_idx;\n";
    }
    // additional idcs
    for(auto& idx : indices) {
      if(idx.tableName != table.name) continue;
      out << "  std::map<std::tuple<";
      generateList(out, idx.columns, [&](auto& out, auto& col){
               out << translateType(table.columns[col].type);
          });
      out << ">, size_t> idx_" << idx.name << ";\n";
    }
    // insert
    out << "  void insert_tuple(";
    generateList(out, table.columns, [&](auto& out, auto& column){
            out << translateType(column.type) << " " << column.name;
        });
    out << ") {\n";
    for(auto& column : table.columns) {
      out << "    this->col_" << column.name << ".push_back(" << column.name << ");\n";
    }
    if(!table.primaryKey.empty()) {
      out << "    this->primary_key_idx.insert(std::make_pair(std::make_tuple(";
      generateList(out, table.primaryKey, [&](auto& out, auto& col){
              out << table.columns[col].name;
          });
      out << "), this->col_" << table.columns[0].name << ".size()))\n";
    }
    for(auto& idx : indices) {
      if(idx.tableName != table.name) continue;
      out << "    this->idx_" << idx.name << ".insert(std::make_pair(std::make_tuple(";
      generateList(out, idx.columns, [&](auto& out, auto& col){
               out << table.columns[col].name;
          });
      out << "), this->col_" << table.columns[0].name << ".size()))\n";
    }
    out << "  }\n";
    // delete
    out << "  void delete_tuple(size_t tid) {\n";
    for(auto& column : table.columns) {
      out << "    this->col_" << column.name << "[tid] = this->col_" << column.name << ".back();\n";
      out << "    this->col_" << column.name << ".pop_back();\n";
    }
    if(!table.primaryKey.empty()) {
      out << "    this->primary_key_idx.erase(std::make_tuple(";
      generateList(out, table.primaryKey, [&](auto& out, auto& col){
              out << "this->col_" << table.columns[col].name << "[tid]";
          });
      out << "));\n";
      out << "    this->primary_key_idx[std::make_tuple(";
      generateList(out, table.primaryKey, [&](auto& out, auto& col){
              out << "this->col_" << table.columns[col].name << ".back()";
          });
      out << ")] = tid;\n";
    }
    for(auto& idx : indices) {
      if(idx.tableName != table.name) continue;
      out << "    this->idx_" << idx.name << ".erase(std::make_tuple(";
      generateList(out, idx.columns, [&](auto& out, auto& col){
               out << "this->col_" << table.columns[col].name << "[tid]";
          });
      out << "));\n";
      out << "    this->primary_key_idx[std::make_tuple(";
      generateList(out, idx.columns, [&](auto& out, auto& col){
              out << "this->col_" << table.columns[col].name << ".back()";
          });
      out << ")] = tid;\n";
    }
    out << "  }\n";
    //TODO: parse
    out << "}\n";
  }
}
