#include <cstdint>
#include <algorithm>
#include "schema/Schema.hpp"

void newOrder (int32_t w_id, int32_t d_id, int32_t c_id, int32_t items, int32_t supware[15], int32_t itemid[15], int32_t qty[15], Timestamp datetime) {
  // select w_tax from warehouse w where w.w_id=w_id;
  auto warehouse_iter = std::find_if(warehouse.begin(), warehouse.end(), [=](auto row){return row.w_id() == w_id;});
  if(warehouse_iter == warehouse.end()) {
    throw "warehouse entry not found";
  }
  auto w_tax = (*warehouse_iter).w_tax();

  // select c_discount from customer c where c_w_id=w_id and c_d_id=d_id and c.c_id=c_id;
  auto customer_iter = std::find_if(customer.begin(), customer.end(), [=](auto row){return row.c_w_id()==w_id && row.c_d_id()==d_id && row.c_id()==c_id;});
  if(customer_iter == customer.end()) {
    throw "customer entry not found";
  }
  auto c_discount = (*customer_iter).c_discount();

  // select d_next_o_id as o_id,d_tax from district d where d_w_id=w_id and d.d_id=d_id;
  auto district_iter = std::find_if(district.begin(), district.end(), [=](auto row){return row.d_w_id()==w_id && row.d_id()==d_id;});
  if(district_iter == district.end()) {
    throw "district entry not found";
  }
  auto o_id = (*district_iter).d_next_o_id();
  auto d_tax = (*district_iter).d_tax();
  // update district set d_next_o_id=o_id+1 where d_w_id=w_id and district.d_id=d_id;
  (*district_iter).d_next_o_id() = o_id+1;

  auto all_local = true;
  for (int32_t i = 0; i < items; i++) {
    if(w_id != supware[i]) all_local = false;
  }

  // insert into "order" values (o_id,d_id,w_id,c_id,datetime,0,items,all_local);
  order.o_id()        .push_back(o_id);
  order.o_d_id()      .push_back(d_id);
  order.o_w_id()      .push_back(w_id);
  order.o_c_id()      .push_back(c_id);
  order.o_entry_d()   .push_back(datetime);
  order.o_carrier_id().push_back(0);
  order.o_ol_cnt()    .push_back(items);
  order.o_all_local() .push_back(all_local);
  
  // insert into neworder values (o_id,d_id,w_id);
  neworder.no_o_id().push_back(o_id);
  neworder.no_d_id().push_back(d_id);
  neworder.no_w_id().push_back(w_id);

  for (int32_t index = 0; index < items; index++) {
    // select i_price from item where i_id=itemid[index];
    auto item_iter = std::find_if(item.begin(), item.end(), [=](auto row){return row.i_id() == itemid[index];});
    if(item_iter == item.end()) {
      throw "item entry not found";
    }
    auto i_price = (*item_iter).i_price();

    // select s_quantity,s_remote_cnt,s_order_cnt,case d_id when 1 then s_dist_01 when 2 then s_dist_02 ... end as s_dist from stock where s_w_id=supware[index] and s_i_id=itemid[index];
    auto stock_iter = std::find_if(stock.begin(), stock.end(), [=](auto row){return row.s_w_id() == supware[index] && row.s_i_id() == itemid[index];});
    if(stock_iter == stock.end()) {
      throw "stock entry not found";
    }
    auto s_quantity = (*stock_iter).s_quantity();
    //These two variables are defined in the original SQL script but are actually unused. No idea, why..
    //auto s_remote_cnt = (*stock_iter).s_remote_cnt();
    //auto s_order_cnt = (*stock_iter).s_order_cnt();
    Char<24> s_dist;
    switch (d_id) {
      case  1: s_dist = (*stock_iter).s_dist_01(); break;
      case  2: s_dist = (*stock_iter).s_dist_02(); break;
      case  3: s_dist = (*stock_iter).s_dist_03(); break;
      case  4: s_dist = (*stock_iter).s_dist_04(); break;
      case  5: s_dist = (*stock_iter).s_dist_05(); break;
      case  6: s_dist = (*stock_iter).s_dist_06(); break;
      case  7: s_dist = (*stock_iter).s_dist_07(); break;
      case  8: s_dist = (*stock_iter).s_dist_08(); break;
      case  9: s_dist = (*stock_iter).s_dist_09(); break;
      case 10: s_dist = (*stock_iter).s_dist_10(); break;
      default: throw "invalid d_id";;
    }
    
    if (s_quantity > qty[index]) {
       // update stock set s_quantity=s_quantity-qty[index] where s_w_id=supware[index] and s_i_id=itemid[index];
       (*stock_iter).s_quantity() = s_quantity-qty[index];
    } else {
       // update stock set s_quantity=s_quantity+91-qty[index] where s_w_id=supware[index] and s_i_id=itemid[index];
       (*stock_iter).s_quantity() = s_quantity+91-qty[index];
    }

    auto stock_iter2 = std::find_if(stock.begin(), stock.end(), [=](auto row){return row.s_w_id() == w_id && row.s_i_id() == itemid[index];});
    if(stock_iter2 == stock.end()) {
      throw "stock entry not found";
    }
    if (supware[index]!=w_id) {
       // update stock set s_remote_cnt=s_remote_cnt+1 where s_w_id=w_id and s_i_id=itemid[index];
       (*stock_iter2).s_remote_cnt() = (*stock_iter2).s_remote_cnt() + 1;
    } else {
       // update stock set s_order_cnt=s_order_cnt+1 where s_w_id=w_id and s_i_id=itemid[index];
       (*stock_iter2).s_order_cnt() = (*stock_iter2).s_order_cnt() + 1;
    }

    // var numeric(6,2) ol_amount=qty[index]*i_price*(1.0+w_tax+d_tax)*(1.0-c_discount);
    auto ol_amount = Numeric<6,2>{Numeric<6,2>{qty[index]}*Numeric<6,2>{i_price}*Numeric<6,4>{(Numeric<4,4>(10000)-c_discount) * (Numeric<4,4>(10000)+w_tax+d_tax)}};
    // insert into orderline values (o_id,d_id,w_id,index+1,itemid[index],supware[index],0,qty[index],ol_amount,s_dist);
    orderline.ol_o_id()       .push_back(o_id);
    orderline.ol_d_id()       .push_back(d_id);
    orderline.ol_w_id()       .push_back(w_id);
    orderline.ol_number()     .push_back(index+1);
    orderline.ol_i_id()       .push_back(itemid[index]);
    orderline.ol_supply_w_id().push_back(supware[index]);
    orderline.ol_delivery_d() .push_back(0);
    orderline.ol_quantity()   .push_back(qty[index]);
    orderline.ol_amount()     .push_back(ol_amount);
    orderline.ol_dist_info()  .push_back(s_dist);
  }
}
