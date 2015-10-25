#include <iostream>
#include <fstream>
#include <memory>
#include <ctime>
#include "schema/GeneratedSchema.hpp"
#include "schema/Types.hpp"
#include "queries/bsum.hpp"

using namespace std;

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

    std::cout << std::endl
              << "order table size " << tables->order.col_o_id.size() << "\n"
              << "neworder table size " << tables->neworder.col_no_o_id.size() << "\n"
              << "orderline table size " << tables->orderline.col_ol_o_id.size() << "\n";

    // execute the OLAP queries
    std::cout << "olap benchmark" << std::endl;
    for(int i = 0; i < 10; i++) {
      clock_t begin_time = clock();
      bsum(tables.get());
      clock_t end_time = clock();
      double elapsed_secs = double(end_time - begin_time) / CLOCKS_PER_SEC;
      std::cout << "time: " << elapsed_secs << " sec" << std::endl;
      #ifdef DEBUG
      if((i & 0xfff) == 0) std::cout << "." << std::flush;
      #endif
    }
  } catch(const char* e) {
    std::cout << "ERROR: " << e << std::endl;
    return -1;
  }
}
