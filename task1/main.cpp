#include <fstream>
#include "Schema.hpp"
#include "parseTbl.hpp"
#include "Types.hpp"
#include "neworderrandom.hpp"

using namespace std;

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
