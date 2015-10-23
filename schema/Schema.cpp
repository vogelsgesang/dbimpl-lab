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
      if(table.primaryKeyPrefixIndexable) {
        out << "  std::map<std::tuple<";
      } else {
        out << "  std::unordered_map<std::tuple<";
      }
      generateList(out, table.primaryKey, [&](auto& out, auto& col){
               out << translateType(table.columns[col].type);
          });
      out << ">, size_t> primary_key_idx;\n";
    }
    // additional idcs
    for(auto& idx : table.indices) {
      if(idx.prefixIndexable) {
        out << "  std::map<std::tuple<";
      } else {
        out << "  std::unordered_map<std::tuple<";
      }
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
    auto generateIndexInsert = [&](const std::string& idxName, const std::vector<unsigned>& idxCols){
      out << "    this->" << idxName << ".insert(std::make_pair(std::make_tuple(";
      generateList(out, idxCols, [&](auto& out, auto& col){
              out << table.columns[col].name;
          });
      out << "), this->col_" << table.columns[0].name << ".size()));\n";
    };
    if(!table.primaryKey.empty()) {
      generateIndexInsert("primary_key_idx", table.primaryKey);
    }
    for(auto& idx : table.indices) {
      generateIndexInsert("idx_" + idx.name, idx.columns);
    }
    out << "  }\n";
    // delete
    out << "  void delete_tuple(size_t tid) {\n";
    auto generateIndexDelete = [&](const std::string& idxName, const std::vector<unsigned>& idxCols){
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
    };
    if(!table.primaryKey.empty()) {
      generateIndexDelete("primary_key_idx", table.primaryKey);
    }
    for(auto& idx : table.indices) {
      generateIndexDelete("idx_" + idx.name, idx.columns);
    }
    for(auto& column : table.columns) {
      out << "    this->col_" << column.name << "[tid] = this->col_" << column.name << ".back();\n";
      out << "    this->col_" << column.name << ".pop_back();\n";
    }
    out << "  }\n";
    // loadFromTbl
    out << "  void loadFromTbl(std::istream& in) {\n"
        << "    std::string buffer;\n"
        << "    char nextChar;\n"
        << "    while(in.good()) {\n";
    bool first = true;
    for(auto& col : table.columns) {
      if(!first) out << "      if(nextChar == '\\n') throw \"unexpected end of line\";\n";
      first = false;
      out << "      buffer.clear();\n"
             "      nextChar = in.peek();\n"
             "      while(nextChar != '|' && nextChar != '\\n') { buffer.push_back(nextChar); nextChar = in.get(); }\n"
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
