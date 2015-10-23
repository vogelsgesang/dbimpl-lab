#include <iostream>
#include <fstream>
#include <memory>
#include <ctime>
#include "schema/GeneratedSchema.hpp"
#include "schema/Types.hpp"
#include "queries/neworderrandom.hpp"

using namespace std;

int main() {
  auto tables = std::make_unique<table_data>();
  //load all the tables from disk
  try {
    auto data_folder = std::string{"data/"};
    tables->warehouse.loadFromTbl(std::ifstream((data_folder + "tpcc_warehouse.tbl").c_str()));
    tables->district.loadFromTbl(std::ifstream((data_folder + "tpcc_district.tbl").c_str()));
    tables->customer.loadFromTbl(std::ifstream((data_folder + "tpcc_customer.tbl").c_str()));
    tables->history.loadFromTbl(std::ifstream((data_folder + "tpcc_history.tbl").c_str()));
    tables->neworder.loadFromTbl(std::ifstream((data_folder + "tpcc_neworder.tbl").c_str()));
    tables->order.loadFromTbl(std::ifstream((data_folder + "tpcc_order.tbl").c_str()));
    tables->orderline.loadFromTbl(std::ifstream((data_folder + "tpcc_orderline.tbl").c_str()));
    tables->item.loadFromTbl(std::ifstream((data_folder + "tpcc_item.tbl").c_str()));
    tables->stock.loadFromTbl(std::ifstream((data_folder + "tpcc_stock.tbl").c_str()));
  } catch(const char* e) {
    std::cout << e << std::endl;
    return -1;
  }

  clock_t begin_time = clock();
  auto iterations = 1'000'000;
  for(int i = 0; i < iterations; i++) {
    newOrderRandom(tables.get(), Timestamp(), 1);
  }
  clock_t end_time = clock();
  double elapsed_secs = double(end_time - begin_time) / CLOCKS_PER_SEC;
  std::cout << (iterations / elapsed_secs) << " transactions per seconds" << std::endl;
}
