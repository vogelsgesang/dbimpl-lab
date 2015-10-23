#include "delivery.hpp"
#include <algorithm>

void delivery(table_data* tables, int32_t w_id, int32_t o_carrier_id, Timestamp datetime) {
  for(int d_id = 1; d_id <= 10; ++d_id) {
    // select min(no_o_id) as o_id from neworder where no_w_id=w_id and no_d_id=d_id order by no_o_id else { continue; } -- ignore this district if no row found
    auto idx_lower = tables->neworder.primary_key_idx.lower_bound(std::make_tuple(w_id, d_id, std::numeric_limits<int32_t>::min()));
    if(std::get<0>(idx_lower->first) != w_id || std::get<1>(idx_lower->first) != d_id) continue;
    auto o_id = std::get<2>(idx_lower->first);

    // delete from neworder where no_w_id=w_id and no_d_id=d_id and no_o_id=o_id;
    auto neworder_iter = tables->neworder.primary_key_idx.find(std::make_tuple(w_id,d_id,o_id));
    if(neworder_iter != tables->neworder.primary_key_idx.end()) {
      auto neworder_tid = neworder_iter->second;
      tables->neworder.delete_tuple(neworder_tid);
    }

    // select o_ol_cnt,o_c_id from "order" o where o_w_id=w_id and o_d_id=d_id and o.o_id=o_id;
    auto order_iter = tables->order.primary_key_idx.find(std::make_tuple(w_id, d_id, o_id));
    if(order_iter == tables->order.primary_key_idx.end()) {
      throw "order entry not found";
    }
    auto order_tid = order_iter->second;
    auto o_ol_cnt = tables->order.col_o_ol_cnt[order_tid];
    auto o_c_id = tables->order.col_o_c_id[order_tid];

    // update "order" set o_carrier_id=o_carrier_id where o_w_id=w_id and o_d_id=d_id and "order".o_id=o_id;
    tables->order.col_o_carrier_id[order_tid] = o_carrier_id;

    // var numeric(6,2) ol_total=0;
    auto ol_total = Numeric<6,2>{0};
    // forsequence (ol_number between 1 and o_ol_cnt) {
    for(int ol_number = 1; ol_number <= o_ol_cnt.getInteger(); ++ol_number) {
      // select ol_amount from orderline where ol_w_id=w_id and ol_d_id=d_id and ol_o_id=o_id and orderline.ol_number=ol_number;
      auto orderline_iter = tables->orderline.primary_key_idx.find(std::make_tuple(w_id, d_id, o_id, ol_number));
      if(orderline_iter == tables->orderline.primary_key_idx.end()) {
        throw "orderline entry not found";
      }
      auto orderline_tid = orderline_iter->second;
      auto ol_amount = tables->orderline.col_ol_amount[orderline_tid];
      // ol_total=ol_total+ol_amount;
      ol_total = ol_total + ol_amount;
      // update orderline set ol_delivery_d=datetime where ol_w_id=w_id and ol_d_id=d_id and ol_o_id=o_id and orderline.ol_number=ol_number;
      tables->orderline.col_ol_delivery_d[orderline_tid] = datetime;
    }

    // update customer set c_balance=c_balance+ol_total where c_w_id=w_id and c_d_id=d_id and c_id=o_c_id;
    auto customer_iter = tables->customer.primary_key_idx.find(std::make_tuple(w_id, d_id, o_c_id));
    if(customer_iter == tables->customer.primary_key_idx.end()) {
      throw "customer entry not found";
    }
    auto customer_tid = customer_iter->second;
    tables->customer.col_c_balance[customer_tid] =
      tables->customer.col_c_balance[customer_tid] + ol_total;
  }
}
