#include "Schema.hpp"
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include "utils/generateList.hpp"

std::string DataType::generateCppCode() const {
  std::ostringstream stream;
  if(this->typeTag == DataTypeTag::Integer) {
    stream << "Integer";
  } else if(this->typeTag == DataTypeTag::Numeric) {
    stream << "Numeric<" << (int) this->attributes.numeric.integerPlaces
           << "," << (int) this->attributes.numeric.decimalPlaces << ">";
  } else if(this->typeTag == DataTypeTag::Char) {
    stream << "Char<" << (int) this->attributes.charLen << ">";
  } else if(this->typeTag == DataTypeTag::VarChar) {
    stream << "Varchar<" << (int) this->attributes.charLen << ">";
  } else if(this->typeTag == DataTypeTag::Date) {
    stream << "Date";
  } else if(this->typeTag == DataTypeTag::Timestamp) {
    stream << "Timestamp";
  }
  return stream.str();
}


const ColumnDescription& TableDescription::getColumnByName(const std::string& name) const {
  auto iter = std::find_if(columns.begin(), columns.end(),
                           [&name](auto& c) {return c.name == name;});
  if(iter == columns.end()) throw std::out_of_range("TableDescription::getColumnByName");
  return *iter;
}


const TableDescription& Schema::getTableByName(const std::string& name) const {
  auto iter = std::find_if(tables.begin(), tables.end(),
                           [&name](auto& c) {return c.name == name;});
  if(iter == tables.end()) throw std::out_of_range("Schema::getTableByName");
  return *iter;
}


static void generateIndexMember(std::ostream& out, const TableDescription& table,
                                const std::string& idxName, const std::vector<unsigned>& idxCols,
                                bool prefixIndexable, bool unique){
  out << "  ";
  if(unique) {
    if(prefixIndexable) out << "std::map";
    else out << "std::unordered_map";
  } else {
    if(prefixIndexable) out << "std::multimap";
    else out << "std::unordered_multimap";
  }
  out << "<std::tuple<";
  generateList(out, idxCols, [&](auto& out, auto& col){
           out << table.columns[col].type.generateCppCode();
      });
  out << ">, size_t> " << idxName << ";\n";
}

static void generateIndexInsert(std::ostream& out, const TableDescription& table,
                                const std::string& idxName, const std::vector<unsigned>& idxCols,
                                bool unique){
  out << "    {\n"
         "      ";
  if(unique) out << "auto insertion_result = ";
  out << "this->" << idxName << ".insert(std::make_pair(std::make_tuple(";
  generateList(out, idxCols, [&](auto& out, auto& col){
          out << table.columns[col].name;
      });
  out << "), this->col_" << table.columns[0].name << ".size()-1));\n";
  if(unique) {
    out << "      if(!insertion_result.second) throw \"duplicate value in index '"
        << idxName << "' on table '" << table.name <<"'\";\n";
  }
  out << "    }\n";
}

static void generateUniqueIndexDelete(std::ostream& out, const TableDescription& table,
                                      const std::string& idxName, const std::vector<unsigned>& idxCols){
  // first update, then delete. This order is important if we try to
  // delete the element at the back of the vectors.
  out << "    this->" << idxName << "[std::make_tuple(";
  generateList(out, idxCols, [&](auto& out, auto& col){
          out << "this->col_" << table.columns[col].name << ".back()";
      });
  out << ")] = tid;\n";
  out << "    this->" << idxName << ".erase(this->"
      << idxName << ".find(std::make_tuple(";
  generateList(out, idxCols, [&](auto& out, auto& col){
          out << "this->col_" << table.columns[col].name << "[tid]";
      });
  out << ")));\n";
}

auto generateIndexDelete(std::ostream& out, const TableDescription& table,
                         const std::string& idxName, const std::vector<unsigned>& idxCols){
  // first update, then delete. This order is important if we try to
  // delete the element at the back of the vectors.
  out << "    {\n"
         "      auto replacement_range = this->" << idxName << ".equal_range(std::make_tuple(";
  generateList(out, idxCols, [&](auto& out, auto& col){
          out << "this->col_" << table.columns[col].name << ".back()";
      });
  out << "));\n";
  out << "      auto replacement_tid = this->col_" << table.columns[0].name << ".size();\n"
         "      (std::find_if(replacement_range.first, replacement_range.second,\n"
         "          [replacement_tid](auto& e){return e.second == replacement_tid;}))->second = tid;\n";
  out << "      auto deleted_range = this->" << idxName << ".equal_range(std::make_tuple(";
  generateList(out, idxCols, [&](auto& out, auto& col){
          out << "this->col_" << table.columns[col].name << "[tid]";
      });
  out << "));\n"
         "      this->" << idxName << ".erase(std::find_if(\n"
         "         deleted_range.first, deleted_range.second,\n"
         "         [tid](auto& e){return e.second == tid;}));\n"
         "    }\n";
}

void Schema::generateCppCode(std::ostream& out) {
  out <<
    "#ifndef H_GeneratedSchema\n"
    "#define H_GeneratedSchema\n"
    "#include <vector>\n"
    "#include <map>\n"
    "#include <unordered_map>\n"
    "#include <algorithm>\n"
    "#include <istream>\n"
    "#include \"utils/tupleHash.hpp\"\n"
    "#include \"schema/Types.hpp\"\n";
  out << "//--------------------------------------------------\n"
         "namespace tables {\n";
  for(auto& table : tables) {
    out << "//--------------------------------------------------\n"
           "struct " << table.name << "_t {\n";
    // actual columns
    for(auto& column : table.columns) {
      out << "  std::vector<" << column.type.generateCppCode() << "> col_" << column.name << ";\n";
    }
    // primary key index
    if(!table.primaryKey.empty()) {
      generateIndexMember(out, table, "primary_key_idx", table.primaryKey, table.primaryKeyPrefixIndexable, true);
    }
    // additional idcs
    for(auto& idx : table.indices) {
      generateIndexMember(out, table, "idx_" + idx.name, idx.columns, idx.prefixIndexable, false);
    }
    // insert
    out << "  void insert_tuple(";
    generateList(out, table.columns, [&](auto& out, auto& column){
            out << column.type.generateCppCode() << " " << column.name;
        });
    out << ") {\n";
    for(auto& column : table.columns) {
      out << "    this->col_" << column.name << ".push_back(" << column.name << ");\n";
    }
    if(!table.primaryKey.empty()) {
      generateIndexInsert(out, table, "primary_key_idx", table.primaryKey, true);
    }
    for(auto& idx : table.indices) {
      generateIndexInsert(out, table, "idx_" + idx.name, idx.columns, false);
    }
    out << "  }\n";
    // delete
    out << "  void delete_tuple(size_t tid) {\n";
    if(!table.primaryKey.empty()) {
      generateUniqueIndexDelete(out, table, "primary_key_idx", table.primaryKey);
    }
    for(auto& idx : table.indices) {
      generateIndexDelete(out, table, "idx_" + idx.name, idx.columns);
    }
    for(auto& column : table.columns) {
      out << "    this->col_" << column.name << "[tid] = this->col_" << column.name << ".back();\n";
      out << "    this->col_" << column.name << ".pop_back();\n";
    }
    out << "  }\n";
    // loadFromTbl
    out << "  void loadFromTbl(std::istream& in) {\n"
        << "    std::string buffer;\n"
        << "    int nextChar;\n"
        << "    while(in.peek() != std::char_traits<char>::eof()) {\n";
    bool first = true;
    for(auto& col : table.columns) {
      if(!first) out << "      if(nextChar != '|') throw \"unexpected end of row\";\n";
      first = false;
      out << "      buffer.clear();\n"
             "      nextChar = in.get();\n"
             "      while(nextChar != '|' && nextChar != '\\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }\n"
             "      this->col_" << col.name << ".push_back(" << col.type.generateCppCode() << "::castString(buffer.c_str(), buffer.size()));\n";
    }
    out << "      if(nextChar != '\\n') throw \"expected end of line\";\n";
    out << "    }\n"
           "    auto table_size = this->col_" << table.columns[0].name << ".size();\n";
    if(!table.primaryKey.empty() && !table.primaryKeyPrefixIndexable) {
      out << "    this->primary_key_idx.reserve(table_size);\n";
    }
    for(auto& idx : table.indices) {
      if(!idx.prefixIndexable)
        out << "    this->idx_" << idx.name << ".reserve(table_size);\n";
    }
    out << "    for(size_t i = 0; i < table_size; i++) {\n";
    auto generateIndexBulkLoad = [&](const std::string& idxName, const std::vector<unsigned>& idxCols){
      out << "      this->" << idxName << ".insert(std::make_pair(std::make_tuple(";
      generateList(out, idxCols, [&](auto& out, auto& col){
              out << "this->col_" << table.columns[col].name << "[i]";
          });
      out << "), i));\n";
    };
    if(!table.primaryKey.empty()) {
      generateIndexBulkLoad("primary_key_idx", table.primaryKey);
    }
    for(auto& idx : table.indices) {
      generateIndexBulkLoad("idx_" + idx.name, idx.columns);
    }
    out << "    }\n"
           "  }\n";
    out << "  void loadFromTbl(std::istream&& in) {\n"
        << "    loadFromTbl(in);\n"
        << "  }\n";
    //finish struct
    out << "};\n";
  }
  out << "//--------------------------------------------------\n"
         "} //namespace table\n"
         "//--------------------------------------------------\n"
         "struct table_data {\n";
  for(auto& table : tables) {
    out << "  tables::" << table.name << "_t " << table.name << ";\n";
  }
  out << "};\n"
         "#endif\n";
}
