#include "Schema.hpp"
#include <sstream>

static std::string translateType(const ColumnDescription& col) {
  std::ostringstream stream;
  if(col.type == DataType::Integer) {
    stream << "Integer";
  } else if(col.type == DataType::Numeric) {
    stream << "Numeric<" << (int) col.typeAttributes.numeric.integerPlaces
           << "," << (int) col.typeAttributes.numeric.decimalPlaces << ">";
  } else if(col.type == DataType::Char) {
    stream << "Char<" << (int) col.typeAttributes.charLen << ">";
  } else if(col.type == DataType::VarChar) {
    stream << "Varchar<" << (int) col.typeAttributes.charLen << ">";
  } else if(col.type == DataType::Date) {
    stream << "Date";
  } else if(col.type == DataType::Timestamp) {
    stream << "Timestamp";
  }
  return stream.str();
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
      out << "  std::vector<" << translateType(column) << "> col_" << column.name << ";\n";
    }
    // primary key index
    bool first;
    if(!table.primaryKey.empty()) {
      out << "  std::unordered_map<std::tuple<";
      first = true;
      for(auto& pkColumn : table.primaryKey) {
        auto& column = table.columns[pkColumn];
        if(!first) out << ",";
        out << translateType(column);
        first = false;
      }
      out << ">, size_t> primary_key_idx;\n";
    }
    //insert
    out << "  void insert_tuple(";
    first = true;
    for(auto& column : table.columns) {
      if(!first) out << ",";
      out << translateType(column) << " " << column.name;
      first = false;
    }
    out << ") {\n";
    for(auto& column : table.columns) {
      out << "    this->col_" << column.name << ".push_back(" << column.name << ");\n";
    }
    out << "    this->primary_key_idx.insert(std::make_pair(std::make_tuple(";
    first = true;
    for(auto& pkColumn : table.primaryKey) {
      auto& column = table.columns[pkColumn];
      if(!first) out << ",";
      out << column.name;
      first = false;
    }
    out << "), this->col_" << table.columns[0].name << ".size()))\n";
    out << "  }\n";
    //delete
    out << "  void delete_tuple(size_t tid) {\n"
           "    this->primary_key_idx.erase(std::make_tuple(";
    first = true;
    for(auto& pkColumn : table.primaryKey) {
      auto& column = table.columns[pkColumn];
      if(!first) out << ",";
      out << "this->col_" << column.name << "[tid]";
      first = false;
    }
    out << "));\n";
    for(auto& column : table.columns) {
      out << "    this->col_" << column.name << "[tid] = this->col_" << column.name << ".back();\n";
      out << "    this->col_" << column.name << ".pop_back();\n";
    }
    out << "  }\n";
    //TODO: parse
    out << "}\n";
  }
}
