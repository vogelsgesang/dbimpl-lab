#include "neworder.hpp"
#include <algorithm>

void newOrder(table_data* tables, int32_t w_id, int32_t d_id, int32_t c_id, int32_t items, int32_t supware[15], int32_t itemid[15], int32_t qty[15], Timestamp datetime) {
  // select w_tax from warehouse w where w.w_id=w_id;
  auto warehouse_iter = tables->warehouse.primary_key_idx.find(std::make_tuple(w_id));
  if(warehouse_iter == tables->warehouse.primary_key_idx.end()) {
    throw "warehouse entry not found";
  }
  auto warehouse_tid = warehouse_iter->second;
  auto w_tax = tables->warehouse.col_w_tax[warehouse_tid];

  // select c_discount from customer c where c_w_id=w_id and c_d_id=d_id and c.c_id=c_id;
  auto customer_iter = tables->customer.primary_key_idx.find(std::make_tuple(w_id, d_id, c_id));
  if(customer_iter == tables->customer.primary_key_idx.end()) {
    throw "customer entry not found";
  }
  auto customer_tid = customer_iter->second;
  auto c_discount = tables->customer.col_c_discount[customer_tid];

  // select d_next_o_id as o_id,d_tax from district d where d_w_id=w_id and d.d_id=d_id;
  auto district_iter = tables->district.primary_key_idx.find(std::make_tuple(w_id, d_id));
  if(district_iter == tables->district.primary_key_idx.end()) {
    throw "district entry not found";
  }
  auto disctrict_tid = district_iter->second;
  auto o_id = tables->district.col_d_next_o_id[disctrict_tid];
  auto d_tax = tables->district.col_d_tax[disctrict_tid];
  // update district set d_next_o_id=o_id+1 where d_w_id=w_id and district.d_id=d_id;
  tables->district.col_d_next_o_id[disctrict_tid] = o_id+1;

  auto all_local = true;
  for (int32_t i = 0; i < items; i++) {
    if(w_id != supware[i]) all_local = false;
  }

  // insert into "order" values (o_id,d_id,w_id,c_id,datetime,0,items,all_local);
  tables->order.insert_tuple(o_id, d_id, w_id, c_id, datetime, 0, items, all_local);
  
  // insert into neworder values (o_id,d_id,w_id);
  tables->neworder.insert_tuple(o_id, d_id, w_id);

  for (int32_t index = 0; index < items; index++) {
    // select i_price from item where i_id=itemid[index];
    auto item_iter = tables->item.primary_key_idx.find(std::make_tuple(itemid[index]));
    if(item_iter == tables->item.primary_key_idx.end()) {
      throw "item entry not found";
    }
    auto item_tid = item_iter->second;
    auto i_price = tables->item.col_i_price[item_tid];

    // select s_quantity,s_remote_cnt,s_order_cnt,case d_id when 1 then s_dist_01 when 2 then s_dist_02 ... end as s_dist from stock where s_w_id=supware[index] and s_i_id=itemid[index];
    auto stock_iter = tables->stock.primary_key_idx.find(std::make_tuple(supware[index], itemid[index]));
    if(stock_iter == tables->stock.primary_key_idx.end()) {
      throw "stock entry not found";
    }
    auto stock_tid = stock_iter->second;
    auto s_quantity = tables->stock.col_s_quantity[stock_tid];
    //These two variables are defined in the original SQL script but are actually unused. No idea, why..
    //auto s_remote_cnt = (*stock_iter).s_remote_cnt();
    //auto s_order_cnt = (*stock_iter).s_order_cnt();
    Char<24> s_dist;
    switch (d_id) {
      case  1: s_dist = tables->stock.col_s_dist_01[stock_tid]; break;
      case  2: s_dist = tables->stock.col_s_dist_02[stock_tid]; break;
      case  3: s_dist = tables->stock.col_s_dist_03[stock_tid]; break;
      case  4: s_dist = tables->stock.col_s_dist_04[stock_tid]; break;
      case  5: s_dist = tables->stock.col_s_dist_05[stock_tid]; break;
      case  6: s_dist = tables->stock.col_s_dist_06[stock_tid]; break;
      case  7: s_dist = tables->stock.col_s_dist_07[stock_tid]; break;
      case  8: s_dist = tables->stock.col_s_dist_08[stock_tid]; break;
      case  9: s_dist = tables->stock.col_s_dist_09[stock_tid]; break;
      case 10: s_dist = tables->stock.col_s_dist_10[stock_tid]; break;
      default: throw "invalid d_id";;
    }
    
    if (s_quantity > qty[index]) {
       // update stock set s_quantity=s_quantity-qty[index] where s_w_id=supware[index] and s_i_id=itemid[index];
       tables->stock.col_s_quantity[stock_tid] = tables->stock.col_s_quantity[stock_tid] - qty[index];
    } else {
       // update stock set s_quantity=s_quantity+91-qty[index] where s_w_id=supware[index] and s_i_id=itemid[index];
       tables->stock.col_s_quantity[stock_tid] = tables->stock.col_s_quantity[stock_tid] + 91 - qty[index];
    }


    auto stock_iter2 = tables->stock.primary_key_idx.find(std::make_tuple(w_id, itemid[index]));
    if(stock_iter2 == tables->stock.primary_key_idx.end()) {
      throw "stock entry not found";
    }
    auto stock_tid2 = stock_iter2->second;
    if (supware[index]!=w_id) {
       // update stock set s_remote_cnt=s_remote_cnt+1 where s_w_id=w_id and s_i_id=itemid[index];
       tables->stock.col_s_remote_cnt[stock_tid2] = tables->stock.col_s_remote_cnt[stock_tid2] + 1;
    } else {
       // update stock set s_order_cnt=s_order_cnt+1 where s_w_id=w_id and s_i_id=itemid[index];
       tables->stock.col_s_order_cnt[stock_tid2] = tables->stock.col_s_order_cnt[stock_tid2] + 1;
    }

    // var numeric(6,2) ol_amount=qty[index]*i_price*(1.0+w_tax+d_tax)*(1.0-c_discount);
    auto ol_amount = Numeric<6,2>{Numeric<6,2>{qty[index]}*Numeric<6,2>{i_price}*Numeric<6,4>{(Numeric<4,4>(10000)-c_discount) * (Numeric<4,4>(10000)+w_tax+d_tax)}};
    // insert into orderline values (o_id,d_id,w_id,index+1,itemid[index],supware[index],0,qty[index],ol_amount,s_dist);
    tables->orderline.insert_tuple(o_id, d_id, w_id, index+1, itemid[index], supware[index], 0, qty[index], ol_amount, s_dist);
  }
}
