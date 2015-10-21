#include <fstream>
#include "Schema.hpp"
#include "parseTbl.hpp"
#include "Types.hpp"
#include "neworderrandom.hpp"

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
  auto data_folder = std::string{"task1/data/"};
  loadTblFromFile(&warehouse, (data_folder + "tpcc_warehouse.tbl").c_str());  
  loadTblFromFile(&district, (data_folder + "tpcc_district.tbl").c_str());  
  loadTblFromFile(&customer, (data_folder + "tpcc_customer.tbl").c_str());  
  loadTblFromFile(&history, (data_folder + "tpcc_history.tbl").c_str());  
  loadTblFromFile(&neworder, (data_folder + "tpcc_neworder.tbl").c_str());  
  loadTblFromFile(&order, (data_folder + "tpcc_order.tbl").c_str());  
  loadTblFromFile(&orderline, (data_folder + "tpcc_orderline.tbl").c_str());  
  loadTblFromFile(&item, (data_folder + "tpcc_item.tbl").c_str());  
  loadTblFromFile(&stock, (data_folder + "tpcc_stock.tbl").c_str());  


}
