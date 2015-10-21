#include <iostream>
#include <fstream>
#include <ctime>
#include "schema/Schema.hpp"
#include "schema/parseTbl.hpp"
#include "schema/Types.hpp"
#include "queries/neworderrandom.hpp"

using namespace std;

warehouse_t warehouse;
district_t district;
customer_t customer;
history_t history;
neworder_t neworder;
order_t order;
orderline_t orderline;
item_t item;
stock_t stock;

int main() {
  //load all the tables from disk
  try {
    auto data_folder = std::string{"data/"};
    loadTblFromFile(&warehouse, (data_folder + "tpcc_warehouse.tbl").c_str());  
    loadTblFromFile(&district, (data_folder + "tpcc_district.tbl").c_str());  
    loadTblFromFile(&customer, (data_folder + "tpcc_customer.tbl").c_str());  
    loadTblFromFile(&history, (data_folder + "tpcc_history.tbl").c_str());  
    loadTblFromFile(&neworder, (data_folder + "tpcc_neworder.tbl").c_str());  
    loadTblFromFile(&order, (data_folder + "tpcc_order.tbl").c_str());  
    loadTblFromFile(&orderline, (data_folder + "tpcc_orderline.tbl").c_str());  
    loadTblFromFile(&item, (data_folder + "tpcc_item.tbl").c_str());  
    loadTblFromFile(&stock, (data_folder + "tpcc_stock.tbl").c_str());  
  } catch(const char* e) {
    std::cout << e << std::endl;
  }

  clock_t begin_time = clock();
  auto iterations = 1'000'000;
  for(int i = 0; i < iterations; i++) {
    newOrderRandom(Timestamp(), 1);
  }
  clock_t end_time = clock();
  double elapsed_secs = double(end_time - begin_time) / CLOCKS_PER_SEC;
  std::cout << (iterations / elapsed_secs) << " transactions per seconds" << std::endl;
}
