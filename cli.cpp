#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <chrono>
#include <cstdlib>
#include <dlfcn.h>
#include "schema/GeneratedSchema.hpp"
#include "schema/Types.hpp"
#include "sql/Parser.hpp"
#include "sql/buildQueryTree.hpp"

using namespace std;

void executeQuery(QueryOperator& query, table_data* tables);

int main() {
  auto tables = std::make_unique<table_data>();
  try {
    //load all the tables from disk
    std::cout << "loading data tables" << std::endl;
    auto data_folder = std::string{"data/"};
    tables->warehouse.loadFromTbl(std::ifstream((data_folder + "tpcc_warehouse.tbl").c_str()));
    std::cout << "." << std::flush;
    tables->district.loadFromTbl(std::ifstream((data_folder + "tpcc_district.tbl").c_str()));
    std::cout << "." << std::flush;
    tables->customer.loadFromTbl(std::ifstream((data_folder + "tpcc_customer.tbl").c_str()));
    std::cout << "." << std::flush;
    tables->history.loadFromTbl(std::ifstream((data_folder + "tpcc_history.tbl").c_str()));
    std::cout << "." << std::flush;
    tables->neworder.loadFromTbl(std::ifstream((data_folder + "tpcc_neworder.tbl").c_str()));
    std::cout << "." << std::flush;
    tables->order.loadFromTbl(std::ifstream((data_folder + "tpcc_order.tbl").c_str()));
    std::cout << "." << std::flush;
    tables->orderline.loadFromTbl(std::ifstream((data_folder + "tpcc_orderline.tbl").c_str()));
    std::cout << "." << std::flush;
    tables->item.loadFromTbl(std::ifstream((data_folder + "tpcc_item.tbl").c_str()));
    std::cout << "." << std::flush;
    tables->stock.loadFromTbl(std::ifstream((data_folder + "tpcc_stock.tbl").c_str()));
    std::cout << "." << std::flush;
    std::cout << std::endl;
  } catch(const char* e) {
    std::cout << "ERROR: " << e << std::endl;
    return -1;
  }

  Parser parser;
  std::unique_ptr<Schema> schema;
  try {
    schema = parser.parseSqlSchema(std::ifstream{"data/schema.sql"});
  } catch(ParserError& e) {
    cout << "ERROR: " << e.what() << endl;
    return -1;
  }

  while(std::cin.good()) {
    std::cout << "> ";
    std::string line;
    std::getline(std::cin, line);
    if(line.length() == 0) continue;
    //parse the query
    try {
      auto queryAst = parser.parseQuery(std::istringstream(line));
      auto queryPlan = buildQueryTree(*queryAst, *schema);
      executeQuery(*queryPlan, tables.get());
    } catch(ParserError& e) {
      std::cout << "ParserError: " << e.what() << std::endl;
      continue;
    } catch(std::string& e) {
      std::cout << "Error: " << e << std::endl;
    } catch(const char* e) {
      std::cout << "Error: " << e << std::endl;
    }
  }
}

void executeQuery(QueryOperator& query, table_data* tables) {
  //generate the C++ code
  {
    std::ofstream out ("generated/cli_query.cpp", ios_base::trunc);
    out << "#include <unordered_map>\n"
           "#include <iostream>\n"
           "#include \"schema/Types.hpp\"\n"
           "#include \"schema/GeneratedSchema.hpp\"\n"
           "extern \"C\" void query(table_data* tables) {";
    auto noRequiredCols = IUSet{};
    query.produce(out, noRequiredCols);
    out << "}";
    out.flush();
  }
  //compile it
  auto start_time = std::chrono::high_resolution_clock::now();
  if(system("g++ -O3 -std=c++14 -shared -fPIC generated/cli_query.cpp -I. -o generated/cli_query.so") != 0) {
    throw "compilation failed";
  }
  auto end_time = std::chrono::high_resolution_clock::now();
  auto compilation_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time).count();
  //load the library
  void* handle = dlopen("generated/cli_query.so",RTLD_NOW);
  if (!handle) throw string{"unable to load .so: "} + dlerror();
  //execute the function
  auto fn = reinterpret_cast<void (*)(table_data*)>(dlsym(handle, "query"));
  if (!fn) {
    dlclose(handle);
    throw string{"query function not found"};
  }
  start_time = std::chrono::high_resolution_clock::now();
  fn(tables);
  end_time = std::chrono::high_resolution_clock::now();
  auto execution_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time).count();
  //unload the library
  if (dlclose(handle)) throw string{"unable to unload .so: "} + dlerror();
  std::cout << "Compilation time: " << compilation_time << "ms" << std::endl;
  std::cout << "Execution time: " << execution_time << "ms" << std::endl;
}
