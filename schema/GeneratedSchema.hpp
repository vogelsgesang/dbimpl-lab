#ifndef H_GeneratedSchema
#define H_GeneratedSchema
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <istream>
#include "utils/tupleHash.hpp"
#include "schema/Types.hpp"
//--------------------------------------------------
namespace tables {
//--------------------------------------------------
struct warehouse_t {
  std::vector<Integer> col_w_id;
  std::vector<Varchar<10>> col_w_name;
  std::vector<Varchar<20>> col_w_street_1;
  std::vector<Varchar<20>> col_w_street_2;
  std::vector<Varchar<20>> col_w_city;
  std::vector<Char<2>> col_w_state;
  std::vector<Char<9>> col_w_zip;
  std::vector<Numeric<4,4>> col_w_tax;
  std::vector<Numeric<12,2>> col_w_ytd;
  std::unordered_map<std::tuple<Integer>, size_t> primary_key_idx;
  void insert_tuple(Integer w_id, Varchar<10> w_name, Varchar<20> w_street_1, Varchar<20> w_street_2, Varchar<20> w_city, Char<2> w_state, Char<9> w_zip, Numeric<4,4> w_tax, Numeric<12,2> w_ytd) {
    this->col_w_id.push_back(w_id);
    this->col_w_name.push_back(w_name);
    this->col_w_street_1.push_back(w_street_1);
    this->col_w_street_2.push_back(w_street_2);
    this->col_w_city.push_back(w_city);
    this->col_w_state.push_back(w_state);
    this->col_w_zip.push_back(w_zip);
    this->col_w_tax.push_back(w_tax);
    this->col_w_ytd.push_back(w_ytd);
    {
      auto insertion_result = this->primary_key_idx.insert(std::make_pair(std::make_tuple(w_id), this->col_w_id.size()-1));
      if(!insertion_result.second) throw "duplicate value in index 'primary_key_idx' on table 'warehouse'";
    }
  }
  void delete_tuple(size_t tid) {
    this->primary_key_idx[std::make_tuple(this->col_w_id.back())] = tid;
    this->primary_key_idx.erase(this->primary_key_idx.find(std::make_tuple(this->col_w_id[tid])));
    this->col_w_id[tid] = this->col_w_id.back();
    this->col_w_id.pop_back();
    this->col_w_name[tid] = this->col_w_name.back();
    this->col_w_name.pop_back();
    this->col_w_street_1[tid] = this->col_w_street_1.back();
    this->col_w_street_1.pop_back();
    this->col_w_street_2[tid] = this->col_w_street_2.back();
    this->col_w_street_2.pop_back();
    this->col_w_city[tid] = this->col_w_city.back();
    this->col_w_city.pop_back();
    this->col_w_state[tid] = this->col_w_state.back();
    this->col_w_state.pop_back();
    this->col_w_zip[tid] = this->col_w_zip.back();
    this->col_w_zip.pop_back();
    this->col_w_tax[tid] = this->col_w_tax.back();
    this->col_w_tax.pop_back();
    this->col_w_ytd[tid] = this->col_w_ytd.back();
    this->col_w_ytd.pop_back();
  }
  void loadFromTbl(std::istream& in) {
    std::string buffer;
    int nextChar;
    while(in.peek() != std::char_traits<char>::eof()) {
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_w_id.push_back(Integer::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_w_name.push_back(Varchar<10>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_w_street_1.push_back(Varchar<20>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_w_street_2.push_back(Varchar<20>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_w_city.push_back(Varchar<20>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_w_state.push_back(Char<2>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_w_zip.push_back(Char<9>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_w_tax.push_back(Numeric<4,4>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_w_ytd.push_back(Numeric<12,2>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '\n') throw "expected end of line";
    }
    auto table_size = this->col_w_id.size();
    this->primary_key_idx.reserve(table_size);
    for(size_t i = 0; i < table_size; i++) {
      this->primary_key_idx.insert(std::make_pair(std::make_tuple(this->col_w_id[i]), i));
    }
  }
  void loadFromTbl(std::istream&& in) {
    loadFromTbl(in);
  }
};
//--------------------------------------------------
struct district_t {
  std::vector<Integer> col_d_id;
  std::vector<Integer> col_d_w_id;
  std::vector<Varchar<10>> col_d_name;
  std::vector<Varchar<20>> col_d_street_1;
  std::vector<Varchar<20>> col_d_street_2;
  std::vector<Varchar<20>> col_d_city;
  std::vector<Char<2>> col_d_state;
  std::vector<Char<9>> col_d_zip;
  std::vector<Numeric<4,4>> col_d_tax;
  std::vector<Numeric<12,2>> col_d_ytd;
  std::vector<Integer> col_d_next_o_id;
  std::unordered_map<std::tuple<Integer, Integer>, size_t> primary_key_idx;
  void insert_tuple(Integer d_id, Integer d_w_id, Varchar<10> d_name, Varchar<20> d_street_1, Varchar<20> d_street_2, Varchar<20> d_city, Char<2> d_state, Char<9> d_zip, Numeric<4,4> d_tax, Numeric<12,2> d_ytd, Integer d_next_o_id) {
    this->col_d_id.push_back(d_id);
    this->col_d_w_id.push_back(d_w_id);
    this->col_d_name.push_back(d_name);
    this->col_d_street_1.push_back(d_street_1);
    this->col_d_street_2.push_back(d_street_2);
    this->col_d_city.push_back(d_city);
    this->col_d_state.push_back(d_state);
    this->col_d_zip.push_back(d_zip);
    this->col_d_tax.push_back(d_tax);
    this->col_d_ytd.push_back(d_ytd);
    this->col_d_next_o_id.push_back(d_next_o_id);
    {
      auto insertion_result = this->primary_key_idx.insert(std::make_pair(std::make_tuple(d_w_id, d_id), this->col_d_id.size()-1));
      if(!insertion_result.second) throw "duplicate value in index 'primary_key_idx' on table 'district'";
    }
  }
  void delete_tuple(size_t tid) {
    this->primary_key_idx[std::make_tuple(this->col_d_w_id.back(), this->col_d_id.back())] = tid;
    this->primary_key_idx.erase(this->primary_key_idx.find(std::make_tuple(this->col_d_w_id[tid], this->col_d_id[tid])));
    this->col_d_id[tid] = this->col_d_id.back();
    this->col_d_id.pop_back();
    this->col_d_w_id[tid] = this->col_d_w_id.back();
    this->col_d_w_id.pop_back();
    this->col_d_name[tid] = this->col_d_name.back();
    this->col_d_name.pop_back();
    this->col_d_street_1[tid] = this->col_d_street_1.back();
    this->col_d_street_1.pop_back();
    this->col_d_street_2[tid] = this->col_d_street_2.back();
    this->col_d_street_2.pop_back();
    this->col_d_city[tid] = this->col_d_city.back();
    this->col_d_city.pop_back();
    this->col_d_state[tid] = this->col_d_state.back();
    this->col_d_state.pop_back();
    this->col_d_zip[tid] = this->col_d_zip.back();
    this->col_d_zip.pop_back();
    this->col_d_tax[tid] = this->col_d_tax.back();
    this->col_d_tax.pop_back();
    this->col_d_ytd[tid] = this->col_d_ytd.back();
    this->col_d_ytd.pop_back();
    this->col_d_next_o_id[tid] = this->col_d_next_o_id.back();
    this->col_d_next_o_id.pop_back();
  }
  void loadFromTbl(std::istream& in) {
    std::string buffer;
    int nextChar;
    while(in.peek() != std::char_traits<char>::eof()) {
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_d_id.push_back(Integer::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_d_w_id.push_back(Integer::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_d_name.push_back(Varchar<10>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_d_street_1.push_back(Varchar<20>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_d_street_2.push_back(Varchar<20>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_d_city.push_back(Varchar<20>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_d_state.push_back(Char<2>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_d_zip.push_back(Char<9>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_d_tax.push_back(Numeric<4,4>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_d_ytd.push_back(Numeric<12,2>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_d_next_o_id.push_back(Integer::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '\n') throw "expected end of line";
    }
    auto table_size = this->col_d_id.size();
    this->primary_key_idx.reserve(table_size);
    for(size_t i = 0; i < table_size; i++) {
      this->primary_key_idx.insert(std::make_pair(std::make_tuple(this->col_d_w_id[i], this->col_d_id[i]), i));
    }
  }
  void loadFromTbl(std::istream&& in) {
    loadFromTbl(in);
  }
};
//--------------------------------------------------
struct customer_t {
  std::vector<Integer> col_c_id;
  std::vector<Integer> col_c_d_id;
  std::vector<Integer> col_c_w_id;
  std::vector<Varchar<16>> col_c_first;
  std::vector<Char<2>> col_c_middle;
  std::vector<Varchar<16>> col_c_last;
  std::vector<Varchar<20>> col_c_street_1;
  std::vector<Varchar<20>> col_c_street_2;
  std::vector<Varchar<20>> col_c_city;
  std::vector<Char<2>> col_c_state;
  std::vector<Char<9>> col_c_zip;
  std::vector<Char<16>> col_c_phone;
  std::vector<Timestamp> col_c_since;
  std::vector<Char<2>> col_c_credit;
  std::vector<Numeric<12,2>> col_c_credit_lim;
  std::vector<Numeric<4,4>> col_c_discount;
  std::vector<Numeric<12,2>> col_c_balance;
  std::vector<Numeric<12,2>> col_c_ytd_paymenr;
  std::vector<Numeric<4,0>> col_c_payment_cnt;
  std::vector<Numeric<4,0>> col_c_delivery_cnt;
  std::vector<Varchar<500>> col_c_data;
  std::unordered_map<std::tuple<Integer, Integer, Integer>, size_t> primary_key_idx;
  std::unordered_multimap<std::tuple<Integer, Integer, Varchar<16>, Varchar<16>>, size_t> idx_customer_wdl;
  void insert_tuple(Integer c_id, Integer c_d_id, Integer c_w_id, Varchar<16> c_first, Char<2> c_middle, Varchar<16> c_last, Varchar<20> c_street_1, Varchar<20> c_street_2, Varchar<20> c_city, Char<2> c_state, Char<9> c_zip, Char<16> c_phone, Timestamp c_since, Char<2> c_credit, Numeric<12,2> c_credit_lim, Numeric<4,4> c_discount, Numeric<12,2> c_balance, Numeric<12,2> c_ytd_paymenr, Numeric<4,0> c_payment_cnt, Numeric<4,0> c_delivery_cnt, Varchar<500> c_data) {
    this->col_c_id.push_back(c_id);
    this->col_c_d_id.push_back(c_d_id);
    this->col_c_w_id.push_back(c_w_id);
    this->col_c_first.push_back(c_first);
    this->col_c_middle.push_back(c_middle);
    this->col_c_last.push_back(c_last);
    this->col_c_street_1.push_back(c_street_1);
    this->col_c_street_2.push_back(c_street_2);
    this->col_c_city.push_back(c_city);
    this->col_c_state.push_back(c_state);
    this->col_c_zip.push_back(c_zip);
    this->col_c_phone.push_back(c_phone);
    this->col_c_since.push_back(c_since);
    this->col_c_credit.push_back(c_credit);
    this->col_c_credit_lim.push_back(c_credit_lim);
    this->col_c_discount.push_back(c_discount);
    this->col_c_balance.push_back(c_balance);
    this->col_c_ytd_paymenr.push_back(c_ytd_paymenr);
    this->col_c_payment_cnt.push_back(c_payment_cnt);
    this->col_c_delivery_cnt.push_back(c_delivery_cnt);
    this->col_c_data.push_back(c_data);
    {
      auto insertion_result = this->primary_key_idx.insert(std::make_pair(std::make_tuple(c_w_id, c_d_id, c_id), this->col_c_id.size()-1));
      if(!insertion_result.second) throw "duplicate value in index 'primary_key_idx' on table 'customer'";
    }
    {
      this->idx_customer_wdl.insert(std::make_pair(std::make_tuple(c_w_id, c_d_id, c_last, c_first), this->col_c_id.size()-1));
    }
  }
  void delete_tuple(size_t tid) {
    this->primary_key_idx[std::make_tuple(this->col_c_w_id.back(), this->col_c_d_id.back(), this->col_c_id.back())] = tid;
    this->primary_key_idx.erase(this->primary_key_idx.find(std::make_tuple(this->col_c_w_id[tid], this->col_c_d_id[tid], this->col_c_id[tid])));
    {
      auto replacement_range = this->idx_customer_wdl.equal_range(std::make_tuple(this->col_c_w_id.back(), this->col_c_d_id.back(), this->col_c_last.back(), this->col_c_first.back()));
      auto replacement_tid = this->col_c_id.size();
      (std::find_if(replacement_range.first, replacement_range.second,
          [replacement_tid](auto& e){return e.second == replacement_tid;}))->second = tid;
      auto deleted_range = this->idx_customer_wdl.equal_range(std::make_tuple(this->col_c_w_id[tid], this->col_c_d_id[tid], this->col_c_last[tid], this->col_c_first[tid]));
      this->idx_customer_wdl.erase(std::find_if(
         deleted_range.first, deleted_range.second,
         [tid](auto& e){return e.second == tid;}));
    }
    this->col_c_id[tid] = this->col_c_id.back();
    this->col_c_id.pop_back();
    this->col_c_d_id[tid] = this->col_c_d_id.back();
    this->col_c_d_id.pop_back();
    this->col_c_w_id[tid] = this->col_c_w_id.back();
    this->col_c_w_id.pop_back();
    this->col_c_first[tid] = this->col_c_first.back();
    this->col_c_first.pop_back();
    this->col_c_middle[tid] = this->col_c_middle.back();
    this->col_c_middle.pop_back();
    this->col_c_last[tid] = this->col_c_last.back();
    this->col_c_last.pop_back();
    this->col_c_street_1[tid] = this->col_c_street_1.back();
    this->col_c_street_1.pop_back();
    this->col_c_street_2[tid] = this->col_c_street_2.back();
    this->col_c_street_2.pop_back();
    this->col_c_city[tid] = this->col_c_city.back();
    this->col_c_city.pop_back();
    this->col_c_state[tid] = this->col_c_state.back();
    this->col_c_state.pop_back();
    this->col_c_zip[tid] = this->col_c_zip.back();
    this->col_c_zip.pop_back();
    this->col_c_phone[tid] = this->col_c_phone.back();
    this->col_c_phone.pop_back();
    this->col_c_since[tid] = this->col_c_since.back();
    this->col_c_since.pop_back();
    this->col_c_credit[tid] = this->col_c_credit.back();
    this->col_c_credit.pop_back();
    this->col_c_credit_lim[tid] = this->col_c_credit_lim.back();
    this->col_c_credit_lim.pop_back();
    this->col_c_discount[tid] = this->col_c_discount.back();
    this->col_c_discount.pop_back();
    this->col_c_balance[tid] = this->col_c_balance.back();
    this->col_c_balance.pop_back();
    this->col_c_ytd_paymenr[tid] = this->col_c_ytd_paymenr.back();
    this->col_c_ytd_paymenr.pop_back();
    this->col_c_payment_cnt[tid] = this->col_c_payment_cnt.back();
    this->col_c_payment_cnt.pop_back();
    this->col_c_delivery_cnt[tid] = this->col_c_delivery_cnt.back();
    this->col_c_delivery_cnt.pop_back();
    this->col_c_data[tid] = this->col_c_data.back();
    this->col_c_data.pop_back();
  }
  void loadFromTbl(std::istream& in) {
    std::string buffer;
    int nextChar;
    while(in.peek() != std::char_traits<char>::eof()) {
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_c_id.push_back(Integer::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_c_d_id.push_back(Integer::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_c_w_id.push_back(Integer::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_c_first.push_back(Varchar<16>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_c_middle.push_back(Char<2>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_c_last.push_back(Varchar<16>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_c_street_1.push_back(Varchar<20>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_c_street_2.push_back(Varchar<20>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_c_city.push_back(Varchar<20>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_c_state.push_back(Char<2>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_c_zip.push_back(Char<9>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_c_phone.push_back(Char<16>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_c_since.push_back(Timestamp::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_c_credit.push_back(Char<2>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_c_credit_lim.push_back(Numeric<12,2>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_c_discount.push_back(Numeric<4,4>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_c_balance.push_back(Numeric<12,2>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_c_ytd_paymenr.push_back(Numeric<12,2>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_c_payment_cnt.push_back(Numeric<4,0>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_c_delivery_cnt.push_back(Numeric<4,0>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_c_data.push_back(Varchar<500>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '\n') throw "expected end of line";
    }
    auto table_size = this->col_c_id.size();
    this->primary_key_idx.reserve(table_size);
    this->idx_customer_wdl.reserve(table_size);
    for(size_t i = 0; i < table_size; i++) {
      this->primary_key_idx.insert(std::make_pair(std::make_tuple(this->col_c_w_id[i], this->col_c_d_id[i], this->col_c_id[i]), i));
      this->idx_customer_wdl.insert(std::make_pair(std::make_tuple(this->col_c_w_id[i], this->col_c_d_id[i], this->col_c_last[i], this->col_c_first[i]), i));
    }
  }
  void loadFromTbl(std::istream&& in) {
    loadFromTbl(in);
  }
};
//--------------------------------------------------
struct history_t {
  std::vector<Integer> col_h_c_id;
  std::vector<Integer> col_h_c_d_id;
  std::vector<Integer> col_h_c_w_id;
  std::vector<Integer> col_h_d_id;
  std::vector<Integer> col_h_w_id;
  std::vector<Timestamp> col_h_date;
  std::vector<Numeric<6,2>> col_h_amount;
  std::vector<Varchar<24>> col_h_data;
  void insert_tuple(Integer h_c_id, Integer h_c_d_id, Integer h_c_w_id, Integer h_d_id, Integer h_w_id, Timestamp h_date, Numeric<6,2> h_amount, Varchar<24> h_data) {
    this->col_h_c_id.push_back(h_c_id);
    this->col_h_c_d_id.push_back(h_c_d_id);
    this->col_h_c_w_id.push_back(h_c_w_id);
    this->col_h_d_id.push_back(h_d_id);
    this->col_h_w_id.push_back(h_w_id);
    this->col_h_date.push_back(h_date);
    this->col_h_amount.push_back(h_amount);
    this->col_h_data.push_back(h_data);
  }
  void delete_tuple(size_t tid) {
    this->col_h_c_id[tid] = this->col_h_c_id.back();
    this->col_h_c_id.pop_back();
    this->col_h_c_d_id[tid] = this->col_h_c_d_id.back();
    this->col_h_c_d_id.pop_back();
    this->col_h_c_w_id[tid] = this->col_h_c_w_id.back();
    this->col_h_c_w_id.pop_back();
    this->col_h_d_id[tid] = this->col_h_d_id.back();
    this->col_h_d_id.pop_back();
    this->col_h_w_id[tid] = this->col_h_w_id.back();
    this->col_h_w_id.pop_back();
    this->col_h_date[tid] = this->col_h_date.back();
    this->col_h_date.pop_back();
    this->col_h_amount[tid] = this->col_h_amount.back();
    this->col_h_amount.pop_back();
    this->col_h_data[tid] = this->col_h_data.back();
    this->col_h_data.pop_back();
  }
  void loadFromTbl(std::istream& in) {
    std::string buffer;
    int nextChar;
    while(in.peek() != std::char_traits<char>::eof()) {
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_h_c_id.push_back(Integer::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_h_c_d_id.push_back(Integer::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_h_c_w_id.push_back(Integer::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_h_d_id.push_back(Integer::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_h_w_id.push_back(Integer::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_h_date.push_back(Timestamp::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_h_amount.push_back(Numeric<6,2>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_h_data.push_back(Varchar<24>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '\n') throw "expected end of line";
    }
    auto table_size = this->col_h_c_id.size();
    for(size_t i = 0; i < table_size; i++) {
    }
  }
  void loadFromTbl(std::istream&& in) {
    loadFromTbl(in);
  }
};
//--------------------------------------------------
struct neworder_t {
  std::vector<Integer> col_no_o_id;
  std::vector<Integer> col_no_d_id;
  std::vector<Integer> col_no_w_id;
  std::map<std::tuple<Integer, Integer, Integer>, size_t> primary_key_idx;
  void insert_tuple(Integer no_o_id, Integer no_d_id, Integer no_w_id) {
    this->col_no_o_id.push_back(no_o_id);
    this->col_no_d_id.push_back(no_d_id);
    this->col_no_w_id.push_back(no_w_id);
    {
      auto insertion_result = this->primary_key_idx.insert(std::make_pair(std::make_tuple(no_w_id, no_d_id, no_o_id), this->col_no_o_id.size()-1));
      if(!insertion_result.second) throw "duplicate value in index 'primary_key_idx' on table 'neworder'";
    }
  }
  void delete_tuple(size_t tid) {
    this->primary_key_idx[std::make_tuple(this->col_no_w_id.back(), this->col_no_d_id.back(), this->col_no_o_id.back())] = tid;
    this->primary_key_idx.erase(this->primary_key_idx.find(std::make_tuple(this->col_no_w_id[tid], this->col_no_d_id[tid], this->col_no_o_id[tid])));
    this->col_no_o_id[tid] = this->col_no_o_id.back();
    this->col_no_o_id.pop_back();
    this->col_no_d_id[tid] = this->col_no_d_id.back();
    this->col_no_d_id.pop_back();
    this->col_no_w_id[tid] = this->col_no_w_id.back();
    this->col_no_w_id.pop_back();
  }
  void loadFromTbl(std::istream& in) {
    std::string buffer;
    int nextChar;
    while(in.peek() != std::char_traits<char>::eof()) {
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_no_o_id.push_back(Integer::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_no_d_id.push_back(Integer::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_no_w_id.push_back(Integer::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '\n') throw "expected end of line";
    }
    auto table_size = this->col_no_o_id.size();
    for(size_t i = 0; i < table_size; i++) {
      this->primary_key_idx.insert(std::make_pair(std::make_tuple(this->col_no_w_id[i], this->col_no_d_id[i], this->col_no_o_id[i]), i));
    }
  }
  void loadFromTbl(std::istream&& in) {
    loadFromTbl(in);
  }
};
//--------------------------------------------------
struct order_t {
  std::vector<Integer> col_o_id;
  std::vector<Integer> col_o_d_id;
  std::vector<Integer> col_o_w_id;
  std::vector<Integer> col_o_c_id;
  std::vector<Timestamp> col_o_entry_d;
  std::vector<Integer> col_o_carrier_id;
  std::vector<Numeric<2,0>> col_o_ol_cnt;
  std::vector<Numeric<1,0>> col_o_all_local;
  std::unordered_map<std::tuple<Integer, Integer, Integer>, size_t> primary_key_idx;
  std::unordered_multimap<std::tuple<Integer, Integer, Integer, Integer>, size_t> idx_order_wdc;
  void insert_tuple(Integer o_id, Integer o_d_id, Integer o_w_id, Integer o_c_id, Timestamp o_entry_d, Integer o_carrier_id, Numeric<2,0> o_ol_cnt, Numeric<1,0> o_all_local) {
    this->col_o_id.push_back(o_id);
    this->col_o_d_id.push_back(o_d_id);
    this->col_o_w_id.push_back(o_w_id);
    this->col_o_c_id.push_back(o_c_id);
    this->col_o_entry_d.push_back(o_entry_d);
    this->col_o_carrier_id.push_back(o_carrier_id);
    this->col_o_ol_cnt.push_back(o_ol_cnt);
    this->col_o_all_local.push_back(o_all_local);
    {
      auto insertion_result = this->primary_key_idx.insert(std::make_pair(std::make_tuple(o_w_id, o_d_id, o_id), this->col_o_id.size()-1));
      if(!insertion_result.second) throw "duplicate value in index 'primary_key_idx' on table 'order'";
    }
    {
      this->idx_order_wdc.insert(std::make_pair(std::make_tuple(o_w_id, o_d_id, o_c_id, o_id), this->col_o_id.size()-1));
    }
  }
  void delete_tuple(size_t tid) {
    this->primary_key_idx[std::make_tuple(this->col_o_w_id.back(), this->col_o_d_id.back(), this->col_o_id.back())] = tid;
    this->primary_key_idx.erase(this->primary_key_idx.find(std::make_tuple(this->col_o_w_id[tid], this->col_o_d_id[tid], this->col_o_id[tid])));
    {
      auto replacement_range = this->idx_order_wdc.equal_range(std::make_tuple(this->col_o_w_id.back(), this->col_o_d_id.back(), this->col_o_c_id.back(), this->col_o_id.back()));
      auto replacement_tid = this->col_o_id.size();
      (std::find_if(replacement_range.first, replacement_range.second,
          [replacement_tid](auto& e){return e.second == replacement_tid;}))->second = tid;
      auto deleted_range = this->idx_order_wdc.equal_range(std::make_tuple(this->col_o_w_id[tid], this->col_o_d_id[tid], this->col_o_c_id[tid], this->col_o_id[tid]));
      this->idx_order_wdc.erase(std::find_if(
         deleted_range.first, deleted_range.second,
         [tid](auto& e){return e.second == tid;}));
    }
    this->col_o_id[tid] = this->col_o_id.back();
    this->col_o_id.pop_back();
    this->col_o_d_id[tid] = this->col_o_d_id.back();
    this->col_o_d_id.pop_back();
    this->col_o_w_id[tid] = this->col_o_w_id.back();
    this->col_o_w_id.pop_back();
    this->col_o_c_id[tid] = this->col_o_c_id.back();
    this->col_o_c_id.pop_back();
    this->col_o_entry_d[tid] = this->col_o_entry_d.back();
    this->col_o_entry_d.pop_back();
    this->col_o_carrier_id[tid] = this->col_o_carrier_id.back();
    this->col_o_carrier_id.pop_back();
    this->col_o_ol_cnt[tid] = this->col_o_ol_cnt.back();
    this->col_o_ol_cnt.pop_back();
    this->col_o_all_local[tid] = this->col_o_all_local.back();
    this->col_o_all_local.pop_back();
  }
  void loadFromTbl(std::istream& in) {
    std::string buffer;
    int nextChar;
    while(in.peek() != std::char_traits<char>::eof()) {
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_o_id.push_back(Integer::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_o_d_id.push_back(Integer::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_o_w_id.push_back(Integer::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_o_c_id.push_back(Integer::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_o_entry_d.push_back(Timestamp::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_o_carrier_id.push_back(Integer::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_o_ol_cnt.push_back(Numeric<2,0>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_o_all_local.push_back(Numeric<1,0>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '\n') throw "expected end of line";
    }
    auto table_size = this->col_o_id.size();
    this->primary_key_idx.reserve(table_size);
    this->idx_order_wdc.reserve(table_size);
    for(size_t i = 0; i < table_size; i++) {
      this->primary_key_idx.insert(std::make_pair(std::make_tuple(this->col_o_w_id[i], this->col_o_d_id[i], this->col_o_id[i]), i));
      this->idx_order_wdc.insert(std::make_pair(std::make_tuple(this->col_o_w_id[i], this->col_o_d_id[i], this->col_o_c_id[i], this->col_o_id[i]), i));
    }
  }
  void loadFromTbl(std::istream&& in) {
    loadFromTbl(in);
  }
};
//--------------------------------------------------
struct orderline_t {
  std::vector<Integer> col_ol_o_id;
  std::vector<Integer> col_ol_d_id;
  std::vector<Integer> col_ol_w_id;
  std::vector<Integer> col_ol_number;
  std::vector<Integer> col_ol_i_id;
  std::vector<Integer> col_ol_supply_w_id;
  std::vector<Timestamp> col_ol_delivery_d;
  std::vector<Numeric<2,0>> col_ol_quantity;
  std::vector<Numeric<6,2>> col_ol_amount;
  std::vector<Char<24>> col_ol_dist_info;
  std::unordered_map<std::tuple<Integer, Integer, Integer, Integer>, size_t> primary_key_idx;
  void insert_tuple(Integer ol_o_id, Integer ol_d_id, Integer ol_w_id, Integer ol_number, Integer ol_i_id, Integer ol_supply_w_id, Timestamp ol_delivery_d, Numeric<2,0> ol_quantity, Numeric<6,2> ol_amount, Char<24> ol_dist_info) {
    this->col_ol_o_id.push_back(ol_o_id);
    this->col_ol_d_id.push_back(ol_d_id);
    this->col_ol_w_id.push_back(ol_w_id);
    this->col_ol_number.push_back(ol_number);
    this->col_ol_i_id.push_back(ol_i_id);
    this->col_ol_supply_w_id.push_back(ol_supply_w_id);
    this->col_ol_delivery_d.push_back(ol_delivery_d);
    this->col_ol_quantity.push_back(ol_quantity);
    this->col_ol_amount.push_back(ol_amount);
    this->col_ol_dist_info.push_back(ol_dist_info);
    {
      auto insertion_result = this->primary_key_idx.insert(std::make_pair(std::make_tuple(ol_w_id, ol_d_id, ol_o_id, ol_number), this->col_ol_o_id.size()-1));
      if(!insertion_result.second) throw "duplicate value in index 'primary_key_idx' on table 'orderline'";
    }
  }
  void delete_tuple(size_t tid) {
    this->primary_key_idx[std::make_tuple(this->col_ol_w_id.back(), this->col_ol_d_id.back(), this->col_ol_o_id.back(), this->col_ol_number.back())] = tid;
    this->primary_key_idx.erase(this->primary_key_idx.find(std::make_tuple(this->col_ol_w_id[tid], this->col_ol_d_id[tid], this->col_ol_o_id[tid], this->col_ol_number[tid])));
    this->col_ol_o_id[tid] = this->col_ol_o_id.back();
    this->col_ol_o_id.pop_back();
    this->col_ol_d_id[tid] = this->col_ol_d_id.back();
    this->col_ol_d_id.pop_back();
    this->col_ol_w_id[tid] = this->col_ol_w_id.back();
    this->col_ol_w_id.pop_back();
    this->col_ol_number[tid] = this->col_ol_number.back();
    this->col_ol_number.pop_back();
    this->col_ol_i_id[tid] = this->col_ol_i_id.back();
    this->col_ol_i_id.pop_back();
    this->col_ol_supply_w_id[tid] = this->col_ol_supply_w_id.back();
    this->col_ol_supply_w_id.pop_back();
    this->col_ol_delivery_d[tid] = this->col_ol_delivery_d.back();
    this->col_ol_delivery_d.pop_back();
    this->col_ol_quantity[tid] = this->col_ol_quantity.back();
    this->col_ol_quantity.pop_back();
    this->col_ol_amount[tid] = this->col_ol_amount.back();
    this->col_ol_amount.pop_back();
    this->col_ol_dist_info[tid] = this->col_ol_dist_info.back();
    this->col_ol_dist_info.pop_back();
  }
  void loadFromTbl(std::istream& in) {
    std::string buffer;
    int nextChar;
    while(in.peek() != std::char_traits<char>::eof()) {
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_ol_o_id.push_back(Integer::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_ol_d_id.push_back(Integer::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_ol_w_id.push_back(Integer::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_ol_number.push_back(Integer::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_ol_i_id.push_back(Integer::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_ol_supply_w_id.push_back(Integer::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_ol_delivery_d.push_back(Timestamp::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_ol_quantity.push_back(Numeric<2,0>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_ol_amount.push_back(Numeric<6,2>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_ol_dist_info.push_back(Char<24>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '\n') throw "expected end of line";
    }
    auto table_size = this->col_ol_o_id.size();
    this->primary_key_idx.reserve(table_size);
    for(size_t i = 0; i < table_size; i++) {
      this->primary_key_idx.insert(std::make_pair(std::make_tuple(this->col_ol_w_id[i], this->col_ol_d_id[i], this->col_ol_o_id[i], this->col_ol_number[i]), i));
    }
  }
  void loadFromTbl(std::istream&& in) {
    loadFromTbl(in);
  }
};
//--------------------------------------------------
struct item_t {
  std::vector<Integer> col_i_id;
  std::vector<Integer> col_i_im_id;
  std::vector<Varchar<24>> col_i_name;
  std::vector<Numeric<5,2>> col_i_price;
  std::vector<Varchar<50>> col_i_data;
  std::unordered_map<std::tuple<Integer>, size_t> primary_key_idx;
  void insert_tuple(Integer i_id, Integer i_im_id, Varchar<24> i_name, Numeric<5,2> i_price, Varchar<50> i_data) {
    this->col_i_id.push_back(i_id);
    this->col_i_im_id.push_back(i_im_id);
    this->col_i_name.push_back(i_name);
    this->col_i_price.push_back(i_price);
    this->col_i_data.push_back(i_data);
    {
      auto insertion_result = this->primary_key_idx.insert(std::make_pair(std::make_tuple(i_id), this->col_i_id.size()-1));
      if(!insertion_result.second) throw "duplicate value in index 'primary_key_idx' on table 'item'";
    }
  }
  void delete_tuple(size_t tid) {
    this->primary_key_idx[std::make_tuple(this->col_i_id.back())] = tid;
    this->primary_key_idx.erase(this->primary_key_idx.find(std::make_tuple(this->col_i_id[tid])));
    this->col_i_id[tid] = this->col_i_id.back();
    this->col_i_id.pop_back();
    this->col_i_im_id[tid] = this->col_i_im_id.back();
    this->col_i_im_id.pop_back();
    this->col_i_name[tid] = this->col_i_name.back();
    this->col_i_name.pop_back();
    this->col_i_price[tid] = this->col_i_price.back();
    this->col_i_price.pop_back();
    this->col_i_data[tid] = this->col_i_data.back();
    this->col_i_data.pop_back();
  }
  void loadFromTbl(std::istream& in) {
    std::string buffer;
    int nextChar;
    while(in.peek() != std::char_traits<char>::eof()) {
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_i_id.push_back(Integer::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_i_im_id.push_back(Integer::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_i_name.push_back(Varchar<24>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_i_price.push_back(Numeric<5,2>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_i_data.push_back(Varchar<50>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '\n') throw "expected end of line";
    }
    auto table_size = this->col_i_id.size();
    this->primary_key_idx.reserve(table_size);
    for(size_t i = 0; i < table_size; i++) {
      this->primary_key_idx.insert(std::make_pair(std::make_tuple(this->col_i_id[i]), i));
    }
  }
  void loadFromTbl(std::istream&& in) {
    loadFromTbl(in);
  }
};
//--------------------------------------------------
struct stock_t {
  std::vector<Integer> col_s_i_id;
  std::vector<Integer> col_s_w_id;
  std::vector<Numeric<4,0>> col_s_quantity;
  std::vector<Char<24>> col_s_dist_01;
  std::vector<Char<24>> col_s_dist_02;
  std::vector<Char<24>> col_s_dist_03;
  std::vector<Char<24>> col_s_dist_04;
  std::vector<Char<24>> col_s_dist_05;
  std::vector<Char<24>> col_s_dist_06;
  std::vector<Char<24>> col_s_dist_07;
  std::vector<Char<24>> col_s_dist_08;
  std::vector<Char<24>> col_s_dist_09;
  std::vector<Char<24>> col_s_dist_10;
  std::vector<Numeric<8,0>> col_s_ytd;
  std::vector<Numeric<4,0>> col_s_order_cnt;
  std::vector<Numeric<4,0>> col_s_remote_cnt;
  std::vector<Varchar<50>> col_s_data;
  std::unordered_map<std::tuple<Integer, Integer>, size_t> primary_key_idx;
  void insert_tuple(Integer s_i_id, Integer s_w_id, Numeric<4,0> s_quantity, Char<24> s_dist_01, Char<24> s_dist_02, Char<24> s_dist_03, Char<24> s_dist_04, Char<24> s_dist_05, Char<24> s_dist_06, Char<24> s_dist_07, Char<24> s_dist_08, Char<24> s_dist_09, Char<24> s_dist_10, Numeric<8,0> s_ytd, Numeric<4,0> s_order_cnt, Numeric<4,0> s_remote_cnt, Varchar<50> s_data) {
    this->col_s_i_id.push_back(s_i_id);
    this->col_s_w_id.push_back(s_w_id);
    this->col_s_quantity.push_back(s_quantity);
    this->col_s_dist_01.push_back(s_dist_01);
    this->col_s_dist_02.push_back(s_dist_02);
    this->col_s_dist_03.push_back(s_dist_03);
    this->col_s_dist_04.push_back(s_dist_04);
    this->col_s_dist_05.push_back(s_dist_05);
    this->col_s_dist_06.push_back(s_dist_06);
    this->col_s_dist_07.push_back(s_dist_07);
    this->col_s_dist_08.push_back(s_dist_08);
    this->col_s_dist_09.push_back(s_dist_09);
    this->col_s_dist_10.push_back(s_dist_10);
    this->col_s_ytd.push_back(s_ytd);
    this->col_s_order_cnt.push_back(s_order_cnt);
    this->col_s_remote_cnt.push_back(s_remote_cnt);
    this->col_s_data.push_back(s_data);
    {
      auto insertion_result = this->primary_key_idx.insert(std::make_pair(std::make_tuple(s_w_id, s_i_id), this->col_s_i_id.size()-1));
      if(!insertion_result.second) throw "duplicate value in index 'primary_key_idx' on table 'stock'";
    }
  }
  void delete_tuple(size_t tid) {
    this->primary_key_idx[std::make_tuple(this->col_s_w_id.back(), this->col_s_i_id.back())] = tid;
    this->primary_key_idx.erase(this->primary_key_idx.find(std::make_tuple(this->col_s_w_id[tid], this->col_s_i_id[tid])));
    this->col_s_i_id[tid] = this->col_s_i_id.back();
    this->col_s_i_id.pop_back();
    this->col_s_w_id[tid] = this->col_s_w_id.back();
    this->col_s_w_id.pop_back();
    this->col_s_quantity[tid] = this->col_s_quantity.back();
    this->col_s_quantity.pop_back();
    this->col_s_dist_01[tid] = this->col_s_dist_01.back();
    this->col_s_dist_01.pop_back();
    this->col_s_dist_02[tid] = this->col_s_dist_02.back();
    this->col_s_dist_02.pop_back();
    this->col_s_dist_03[tid] = this->col_s_dist_03.back();
    this->col_s_dist_03.pop_back();
    this->col_s_dist_04[tid] = this->col_s_dist_04.back();
    this->col_s_dist_04.pop_back();
    this->col_s_dist_05[tid] = this->col_s_dist_05.back();
    this->col_s_dist_05.pop_back();
    this->col_s_dist_06[tid] = this->col_s_dist_06.back();
    this->col_s_dist_06.pop_back();
    this->col_s_dist_07[tid] = this->col_s_dist_07.back();
    this->col_s_dist_07.pop_back();
    this->col_s_dist_08[tid] = this->col_s_dist_08.back();
    this->col_s_dist_08.pop_back();
    this->col_s_dist_09[tid] = this->col_s_dist_09.back();
    this->col_s_dist_09.pop_back();
    this->col_s_dist_10[tid] = this->col_s_dist_10.back();
    this->col_s_dist_10.pop_back();
    this->col_s_ytd[tid] = this->col_s_ytd.back();
    this->col_s_ytd.pop_back();
    this->col_s_order_cnt[tid] = this->col_s_order_cnt.back();
    this->col_s_order_cnt.pop_back();
    this->col_s_remote_cnt[tid] = this->col_s_remote_cnt.back();
    this->col_s_remote_cnt.pop_back();
    this->col_s_data[tid] = this->col_s_data.back();
    this->col_s_data.pop_back();
  }
  void loadFromTbl(std::istream& in) {
    std::string buffer;
    int nextChar;
    while(in.peek() != std::char_traits<char>::eof()) {
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_s_i_id.push_back(Integer::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_s_w_id.push_back(Integer::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_s_quantity.push_back(Numeric<4,0>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_s_dist_01.push_back(Char<24>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_s_dist_02.push_back(Char<24>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_s_dist_03.push_back(Char<24>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_s_dist_04.push_back(Char<24>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_s_dist_05.push_back(Char<24>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_s_dist_06.push_back(Char<24>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_s_dist_07.push_back(Char<24>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_s_dist_08.push_back(Char<24>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_s_dist_09.push_back(Char<24>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_s_dist_10.push_back(Char<24>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_s_ytd.push_back(Numeric<8,0>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_s_order_cnt.push_back(Numeric<4,0>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_s_remote_cnt.push_back(Numeric<4,0>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '|') throw "unexpected end of row";
      buffer.clear();
      nextChar = in.get();
      while(nextChar != '|' && nextChar != '\n' && nextChar != std::char_traits<char>::eof()) { buffer.push_back(nextChar); nextChar = in.get(); }
      this->col_s_data.push_back(Varchar<50>::castString(buffer.c_str(), buffer.size()));
      if(nextChar != '\n') throw "expected end of line";
    }
    auto table_size = this->col_s_i_id.size();
    this->primary_key_idx.reserve(table_size);
    for(size_t i = 0; i < table_size; i++) {
      this->primary_key_idx.insert(std::make_pair(std::make_tuple(this->col_s_w_id[i], this->col_s_i_id[i]), i));
    }
  }
  void loadFromTbl(std::istream&& in) {
    loadFromTbl(in);
  }
};
//--------------------------------------------------
} //namespace table
//--------------------------------------------------
struct table_data {
  tables::warehouse_t warehouse;
  tables::district_t district;
  tables::customer_t customer;
  tables::history_t history;
  tables::neworder_t neworder;
  tables::order_t order;
  tables::orderline_t orderline;
  tables::item_t item;
  tables::stock_t stock;
};
#endif
