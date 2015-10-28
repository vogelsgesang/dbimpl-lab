#include "bsum.hpp"
#include <unordered_map>
#include <iostream>

void bsum(table_data* tables) {
  auto& customer = tables->customer;
  auto& order = tables->order;
  auto& orderline = tables->orderline;

  // build hash set for join between user and order
  // use a set instead of a map since we do not need any attributes from the customer after
  // the join. we do not need a multiset since c_w_id, c_d_id and c_id form the primary
  // key of the customer table.
  std::unordered_map<std::tuple<Integer, Integer, Integer>, Numeric<12,2>> qualified_customers;
  for(size_t customer_tid = 0; customer_tid < customer.col_c_id.size(); ++customer_tid) {
    // c_last like 'B%'
    if(customer.col_c_last[customer_tid].value[0] == 'B') {
      qualified_customers.insert(std::make_pair(
          std::make_tuple(customer.col_c_w_id[customer_tid], customer.col_c_d_id[customer_tid], customer.col_c_id[customer_tid]),
          customer.col_c_balance[customer_tid]
        ));
    }
  }

  // build hash set for join between order and orderline
  std::unordered_map<std::tuple<Integer, Integer, Integer>, Numeric<14,2>> qualified_orders;
  for(size_t order_tid = 0; order_tid < order.col_o_id.size(); order_tid++) {
    auto customer_iter = qualified_customers.find(std::make_tuple(order.col_o_w_id[order_tid], order.col_o_d_id[order_tid], order.col_o_c_id[order_tid]));
    if(customer_iter != qualified_customers.end()) {
      qualified_orders.insert(std::make_pair(
          std::make_tuple(order.col_o_w_id[order_tid], order.col_o_d_id[order_tid], order.col_o_id[order_tid]),
          Numeric<14,2>{Numeric<14,2>{customer_iter->second} * order.col_o_ol_cnt[order_tid]}
        ));
    }
  }

  // aggregate over all qualified orderlines
  auto sum = Numeric<14,2>{0};
  for(size_t ol_tid = 0; ol_tid < orderline.col_ol_o_id.size(); ol_tid++) {
    auto order_iter = qualified_orders.find(std::make_tuple(orderline.col_ol_w_id[ol_tid], orderline.col_ol_d_id[ol_tid], orderline.col_ol_o_id[ol_tid]));
    if(order_iter != qualified_orders.end()) {
      sum = sum + Numeric<14,2>{orderline.col_ol_amount[ol_tid] * orderline.col_ol_quantity[ol_tid]} - order_iter->second;
    }
  }
  std::cout << "sum: " << sum << std::endl;
}
