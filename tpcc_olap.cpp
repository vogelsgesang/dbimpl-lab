#include <iostream>
#include <fstream>
#include <memory>
#include <ctime>
#include <atomic>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "schema/GeneratedSchema.hpp"
#include "schema/Types.hpp"
#include "queries/neworderrandom.hpp"
#include "queries/delivery.hpp"
#include "queries/bsum.hpp"

using namespace std;

atomic<bool> olapRunning{false};
atomic<clock_t> olapQueryTime{0};
atomic<clock_t> olapForkBegin;

static void SIGCHLD_handler(int /*sig*/) {
  olapQueryTime += clock() - olapForkBegin;
  int status;
  wait(&status);
  olapRunning = false;
}

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
              << "order table size: " << tables->order.col_o_id.size() << "\n"
              << "neworder table size: " << tables->neworder.col_no_o_id.size() << "\n"
              << "orderline table size: " << tables->orderline.col_ol_o_id.size() << "\n";

    //set up the signal handler
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags=0;
    sa.sa_handler=SIGCHLD_handler;
    sigaction(SIGCHLD,&sa,NULL);

    auto olapQueryCnt = unsigned{0};
    auto olapForkTime = clock_t{0};
    //run the queries
    std::cout << "tpcc & olap benchmark" << std::endl;
    clock_t begin_time = clock();
    auto iterations = 1'000'000;
    for(int i = 0; i < iterations; i++) {
      //fork a new child if no child is running currently
      if(!olapRunning) {
        olapRunning = true;
        olapQueryCnt++;
        olapForkBegin = clock();
        auto pid = fork();
        olapForkTime += clock() - olapForkBegin;
        if(pid == -1) {
          std::cerr << "unable to fork" << std::endl;
          return -1;
        }
        if(!pid) { // forked child
          bsum(tables.get());
          return 0; // child is finished
        }
      }
      if(random()%10) {
        newOrderRandom(tables.get(), Timestamp(), random()%5+1);
      } else {
        delivery(tables.get(), random()%5+1, 0, Timestamp());
      }
      #ifdef DEBUG
      if((i & 0xfff) == 0) std::cout << "." << std::flush;
      #endif
    }
    std::cout << std::endl
              << "order table size: " << tables->order.col_o_id.size() << "\n"
              << "neworder table size: " << tables->neworder.col_no_o_id.size() << "\n"
              << "orderline table size: " << tables->orderline.col_ol_o_id.size() << "\n";
    clock_t end_time = clock();
    double elapsed_secs = double(end_time - begin_time) / CLOCKS_PER_SEC;
    std::cout << "TPCC transactions / sec:" << (iterations / elapsed_secs) << std::endl;
    std::cout << "OLAP transactions: " << olapQueryCnt << std::endl;
    std::cout << "average fork time: " << (double(olapForkTime) / olapQueryCnt / CLOCKS_PER_SEC * 1000) << "ms" << std::endl;
    std::cout << "average OLAP exec time: " << (double(olapQueryTime - olapForkTime) / CLOCKS_PER_SEC / olapQueryCnt) << "s" << std::endl;
  } catch(const char* e) {
    std::cout << "ERROR: " << e << std::endl;
    return -1;
  }
}
