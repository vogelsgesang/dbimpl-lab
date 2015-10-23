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
static void generateList(std::ostream& out, const std::vector<T>& elements,
                         Callable cb, std::string seperator = ", ") {
  auto first = true;
  for(const auto& elem : elements) {
    if(!first) out << seperator;
    cb(out, elem);
    first = false;
  }
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
           out << translateType(table.columns[col].type);
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
  out << "), this->col_" << table.columns[0].name << ".size()));\n";
  if(unique) {
    out << "      if(!insertion_result.second) throw \"duplicate value in index '"
        << idxName << "' on table '" << table.name <<"'\";\n";
  }
  out << "    }\n";
}

static void generateUniqueIndexDelete(std::ostream& out, const TableDescription& table,
                                      const std::string& idxName, const std::vector<unsigned>& idxCols){
  out << "    this->" << idxName << ".erase(this->"
      << idxName << ".find(std::make_tuple(";
  generateList(out, idxCols, [&](auto& out, auto& col){
          out << "this->col_" << table.columns[col].name << "[tid]";
      });
  out << ")));\n";
  out << "    this->" << idxName << "[std::make_tuple(";
  generateList(out, idxCols, [&](auto& out, auto& col){
          out << "this->col_" << table.columns[col].name << ".back()";
      });
  out << ")] = tid;\n";
}

auto generateIndexDelete(std::ostream& out, const TableDescription& table,
                         const std::string& idxName, const std::vector<unsigned>& idxCols){
  out << "    {\n";
  out << "      auto deleted_range = this->" << idxName << ".equal_range(std::make_tuple(";
  generateList(out, idxCols, [&](auto& out, auto& col){
          out << "this->col_" << table.columns[col].name << "[tid]";
      });
  out << "));\n"
         "      this->" << idxName << ".erase(std::find_if(\n"
         "         deleted_range.first, deleted_range.second,\n"
         "         [tid](auto& e){return e.second == tid;}));\n"
         "      auto replacement_range = this->" << idxName << ".equal_range(std::make_tuple(";
  generateList(out, idxCols, [&](auto& out, auto& col){
          out << "this->col_" << table.columns[col].name << ".back()";
      });
  out << "));\n";
  out << "      auto replacement_tid = this->col_" << table.columns[0].name << ".size();\n"
         "      (std::find_if(replacement_range.first, replacement_range.second,\n"
         "          [replacement_tid](auto& e){return e.second == replacement_tid;}))->second = tid;\n"
         "    }\n";
}

void Schema::generateCppCode(std::ostream& out) {
  out <<
    "#ifndef H_Schema\n"
    "#define H_Schema\n"
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
      out << "  std::vector<" << translateType(column.type) << "> col_" << column.name << ";\n";
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
            out << translateType(column.type) << " " << column.name;
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
             "      this->col_" << col.name << ".push_back(" << translateType(col.type) << "::castString(buffer.c_str(), buffer.size()));\n";
    }
    out << "      if(nextChar != '\\n') throw \"expected end of line\";\n";
    out << "    }\n"
           "    auto table_size = this->col_" << table.columns[0].name << ".size();\n";
    if(!table.primaryKey.empty()) {
      out << "    this->primary_key_idx.reserve(table_size);\n";
    }
    for(auto& idx : table.indices) {
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
