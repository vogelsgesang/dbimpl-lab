#ifndef H_Schema
#define H_Schema
#include <vector>
#include <tuple>
#include "Types.hpp"
//-----------------------------------------------------------------------------
struct warehouse_t {
  typedef std::tuple<
    std::vector<Integer>, //w_id
    std::vector<Varchar<10>>, //w_name
    std::vector<Varchar<20>>, //w_street_1
    std::vector<Varchar<20>>, //w_street_2
    std::vector<Varchar<20>>, //w_city
    std::vector<Char<2>>, //w_state
    std::vector<Char<9>>, //w_zip
    std::vector<Numeric<4,4>>, //w_tax
    std::vector<Numeric<12,2>> //w_ytd
  > storage_type;

  auto size() {return std::get<0>(data).size();}

  auto& w_id() { return std::get<0>(data); }
  const auto& w_id() const { return std::get<0>(data); }
  auto& w_name() { return std::get<1>(data); }
  const auto& w_name() const { return std::get<1>(data); }
  auto& w_street_1() { return std::get<2>(data); }
  const auto& w_street_1() const { return std::get<2>(data); }
  auto& w_street_2() { return std::get<3>(data); }
  const auto& w_street_2() const { return std::get<3>(data); }
  auto& w_city() { return std::get<4>(data); }
  const auto& w_city() const { return std::get<4>(data); }
  auto& w_state() { return std::get<5>(data); }
  const auto& w_state() const { return std::get<5>(data); }
  auto& w_zip() { return std::get<6>(data); }
  const auto& w_zip() const { return std::get<6>(data); }
  auto& w_tax() { return std::get<7>(data); }
  const auto& w_tax() const { return std::get<7>(data); }
  auto& w_ytd() { return std::get<8>(data); }
  const auto& w_ytd() const { return std::get<8>(data); }
  

  class dereferenced_iterator {
    public:
      dereferenced_iterator(storage_type &data, size_t offset) : data(data), offset(offset) {}

      auto& w_id() { return std::get<0>(data)[offset]; }
      const auto& w_id() const { return std::get<0>(data)[offset]; }
      auto& w_name() { return std::get<1>(data)[offset]; }
      const auto& w_name() const { return std::get<1>(data)[offset]; }
      auto& w_street_1() { return std::get<2>(data)[offset]; }
      const auto& w_street_1() const { return std::get<2>(data)[offset]; }
      auto& w_street_2() { return std::get<3>(data)[offset]; }
      const auto& w_street_2() const { return std::get<3>(data)[offset]; }
      auto& w_city() { return std::get<4>(data)[offset]; }
      const auto& w_city() const { return std::get<4>(data)[offset]; }
      auto& w_state() { return std::get<5>(data)[offset]; }
      const auto& w_state() const { return std::get<5>(data)[offset]; }
      auto& w_zip() { return std::get<6>(data)[offset]; }
      const auto& w_zip() const { return std::get<6>(data)[offset]; }
      auto& w_tax() { return std::get<7>(data)[offset]; }
      const auto& w_tax() const { return std::get<7>(data)[offset]; }
      auto& w_ytd() { return std::get<8>(data)[offset]; }
      const auto& w_ytd() const { return std::get<8>(data)[offset]; }
      
    private:
      storage_type &data;
      size_t offset;
  };

  class iterator {
    public:
      iterator(storage_type &data, size_t offset) : data(data), offset(offset) {}

      iterator& operator++() {++offset; return *this;}
      iterator operator++(int) {auto copy = *this; offset++; return copy;}
      iterator& operator--() {--offset; return *this;}
      iterator operator--(int) {auto copy = *this; offset--; return copy;}

      bool operator==(const iterator& rhs) const {return &data == &rhs.data && offset == rhs.offset;}
      bool operator!=(const iterator& rhs) const {return !(*this == rhs);}

      size_t operator-(const iterator& rhs) const {return offset - rhs.offset;}

      auto operator*() { return dereferenced_iterator{data, offset}; }

    private:
      storage_type &data;
      size_t offset;
  };

  auto begin() {return iterator{data, 0};}
  auto end() {return iterator{data, size()};}

  storage_type data;
};
//-----------------------------------------------------------------------------
struct district_t {
  typedef std::tuple<
    std::vector<Integer>, //d_id
    std::vector<Integer>, //d_w_id
    std::vector<Varchar<10>>, //d_name
    std::vector<Varchar<20>>, //d_street_1
    std::vector<Varchar<20>>, //d_street_2
    std::vector<Varchar<20>>, //d_city
    std::vector<Char<2>>, //d_state
    std::vector<Char<9>>, //d_zip
    std::vector<Numeric<4,4>>, //d_tax
    std::vector<Numeric<12,2>>, //d_ytd
    std::vector<Integer> //d_next_o_id
  > storage_type;

  auto size() {return std::get<0>(data).size();}

  auto& d_id() { return std::get<0>(data); }
  const auto& d_id() const { return std::get<0>(data); }
  auto& d_w_id() { return std::get<1>(data); }
  const auto& d_w_id() const { return std::get<1>(data); }
  auto& d_name() { return std::get<2>(data); }
  const auto& d_name() const { return std::get<2>(data); }
  auto& d_street_1() { return std::get<3>(data); }
  const auto& d_street_1() const { return std::get<3>(data); }
  auto& d_street_2() { return std::get<4>(data); }
  const auto& d_street_2() const { return std::get<4>(data); }
  auto& d_city() { return std::get<5>(data); }
  const auto& d_city() const { return std::get<5>(data); }
  auto& d_state() { return std::get<6>(data); }
  const auto& d_state() const { return std::get<6>(data); }
  auto& d_zip() { return std::get<7>(data); }
  const auto& d_zip() const { return std::get<7>(data); }
  auto& d_tax() { return std::get<8>(data); }
  const auto& d_tax() const { return std::get<8>(data); }
  auto& d_ytd() { return std::get<9>(data); }
  const auto& d_ytd() const { return std::get<9>(data); }
  auto& d_next_o_id() { return std::get<10>(data); }
  const auto& d_next_o_id() const { return std::get<10>(data); }
  

  class dereferenced_iterator {
    public:
      dereferenced_iterator(storage_type &data, size_t offset) : data(data), offset(offset) {}

      auto& d_id() { return std::get<0>(data)[offset]; }
      const auto& d_id() const { return std::get<0>(data)[offset]; }
      auto& d_w_id() { return std::get<1>(data)[offset]; }
      const auto& d_w_id() const { return std::get<1>(data)[offset]; }
      auto& d_name() { return std::get<2>(data)[offset]; }
      const auto& d_name() const { return std::get<2>(data)[offset]; }
      auto& d_street_1() { return std::get<3>(data)[offset]; }
      const auto& d_street_1() const { return std::get<3>(data)[offset]; }
      auto& d_street_2() { return std::get<4>(data)[offset]; }
      const auto& d_street_2() const { return std::get<4>(data)[offset]; }
      auto& d_city() { return std::get<5>(data)[offset]; }
      const auto& d_city() const { return std::get<5>(data)[offset]; }
      auto& d_state() { return std::get<6>(data)[offset]; }
      const auto& d_state() const { return std::get<6>(data)[offset]; }
      auto& d_zip() { return std::get<7>(data)[offset]; }
      const auto& d_zip() const { return std::get<7>(data)[offset]; }
      auto& d_tax() { return std::get<8>(data)[offset]; }
      const auto& d_tax() const { return std::get<8>(data)[offset]; }
      auto& d_ytd() { return std::get<9>(data)[offset]; }
      const auto& d_ytd() const { return std::get<9>(data)[offset]; }
      auto& d_next_o_id() { return std::get<10>(data)[offset]; }
      const auto& d_next_o_id() const { return std::get<10>(data)[offset]; }
      
    private:
      storage_type &data;
      size_t offset;
  };

  class iterator {
    public:
      iterator(storage_type &data, size_t offset) : data(data), offset(offset) {}

      iterator& operator++() {++offset; return *this;}
      iterator operator++(int) {auto copy = *this; offset++; return copy;}
      iterator& operator--() {--offset; return *this;}
      iterator operator--(int) {auto copy = *this; offset--; return copy;}

      bool operator==(const iterator& rhs) const {return &data == &rhs.data && offset == rhs.offset;}
      bool operator!=(const iterator& rhs) const {return !(*this == rhs);}

      size_t operator-(const iterator& rhs) const {return offset - rhs.offset;}

      auto operator*() { return dereferenced_iterator{data, offset}; }

    private:
      storage_type &data;
      size_t offset;
  };

  auto begin() {return iterator{data, 0};}
  auto end() {return iterator{data, size()};}

  storage_type data;
};
//-----------------------------------------------------------------------------
struct customer_t {
  typedef std::tuple<
    std::vector<Integer>, //c_id
    std::vector<Integer>, //c_d_id
    std::vector<Integer>, //c_w_id
    std::vector<Varchar<16>>, //c_first
    std::vector<Char<2>>, //c_middle
    std::vector<Varchar<16>>, //c_last
    std::vector<Varchar<20>>, //c_street_1
    std::vector<Varchar<20>>, //c_street_2
    std::vector<Varchar<20>>, //c_city
    std::vector<Char<2>>, //c_state
    std::vector<Char<9>>, //c_zip
    std::vector<Char<16>>, //c_phone
    std::vector<Timestamp>, //c_since
    std::vector<Char<2>>, //c_credit
    std::vector<Numeric<12,2>>, //c_credit_lim
    std::vector<Numeric<4,4>>, //c_discount
    std::vector<Numeric<12,2>>, //c_balance
    std::vector<Numeric<12,2>>, //c_ytd_paymenr
    std::vector<Numeric<4,0>>, //c_payment_cnt
    std::vector<Numeric<4,0>>, //c_delivery_cnt
    std::vector<Varchar<500>> //c_data
  > storage_type;

  auto size() {return std::get<0>(data).size();}

  auto& c_id() { return std::get<0>(data); }
  const auto& c_id() const { return std::get<0>(data); }
  auto& c_d_id() { return std::get<1>(data); }
  const auto& c_d_id() const { return std::get<1>(data); }
  auto& c_w_id() { return std::get<2>(data); }
  const auto& c_w_id() const { return std::get<2>(data); }
  auto& c_first() { return std::get<3>(data); }
  const auto& c_first() const { return std::get<3>(data); }
  auto& c_middle() { return std::get<4>(data); }
  const auto& c_middle() const { return std::get<4>(data); }
  auto& c_last() { return std::get<5>(data); }
  const auto& c_last() const { return std::get<5>(data); }
  auto& c_street_1() { return std::get<6>(data); }
  const auto& c_street_1() const { return std::get<6>(data); }
  auto& c_street_2() { return std::get<7>(data); }
  const auto& c_street_2() const { return std::get<7>(data); }
  auto& c_city() { return std::get<8>(data); }
  const auto& c_city() const { return std::get<8>(data); }
  auto& c_state() { return std::get<9>(data); }
  const auto& c_state() const { return std::get<9>(data); }
  auto& c_zip() { return std::get<10>(data); }
  const auto& c_zip() const { return std::get<10>(data); }
  auto& c_phone() { return std::get<11>(data); }
  const auto& c_phone() const { return std::get<11>(data); }
  auto& c_since() { return std::get<12>(data); }
  const auto& c_since() const { return std::get<12>(data); }
  auto& c_credit() { return std::get<13>(data); }
  const auto& c_credit() const { return std::get<13>(data); }
  auto& c_credit_lim() { return std::get<14>(data); }
  const auto& c_credit_lim() const { return std::get<14>(data); }
  auto& c_discount() { return std::get<15>(data); }
  const auto& c_discount() const { return std::get<15>(data); }
  auto& c_balance() { return std::get<16>(data); }
  const auto& c_balance() const { return std::get<16>(data); }
  auto& c_ytd_paymenr() { return std::get<17>(data); }
  const auto& c_ytd_paymenr() const { return std::get<17>(data); }
  auto& c_payment_cnt() { return std::get<18>(data); }
  const auto& c_payment_cnt() const { return std::get<18>(data); }
  auto& c_delivery_cnt() { return std::get<19>(data); }
  const auto& c_delivery_cnt() const { return std::get<19>(data); }
  auto& c_data() { return std::get<20>(data); }
  const auto& c_data() const { return std::get<20>(data); }
  

  class dereferenced_iterator {
    public:
      dereferenced_iterator(storage_type &data, size_t offset) : data(data), offset(offset) {}

      auto& c_id() { return std::get<0>(data)[offset]; }
      const auto& c_id() const { return std::get<0>(data)[offset]; }
      auto& c_d_id() { return std::get<1>(data)[offset]; }
      const auto& c_d_id() const { return std::get<1>(data)[offset]; }
      auto& c_w_id() { return std::get<2>(data)[offset]; }
      const auto& c_w_id() const { return std::get<2>(data)[offset]; }
      auto& c_first() { return std::get<3>(data)[offset]; }
      const auto& c_first() const { return std::get<3>(data)[offset]; }
      auto& c_middle() { return std::get<4>(data)[offset]; }
      const auto& c_middle() const { return std::get<4>(data)[offset]; }
      auto& c_last() { return std::get<5>(data)[offset]; }
      const auto& c_last() const { return std::get<5>(data)[offset]; }
      auto& c_street_1() { return std::get<6>(data)[offset]; }
      const auto& c_street_1() const { return std::get<6>(data)[offset]; }
      auto& c_street_2() { return std::get<7>(data)[offset]; }
      const auto& c_street_2() const { return std::get<7>(data)[offset]; }
      auto& c_city() { return std::get<8>(data)[offset]; }
      const auto& c_city() const { return std::get<8>(data)[offset]; }
      auto& c_state() { return std::get<9>(data)[offset]; }
      const auto& c_state() const { return std::get<9>(data)[offset]; }
      auto& c_zip() { return std::get<10>(data)[offset]; }
      const auto& c_zip() const { return std::get<10>(data)[offset]; }
      auto& c_phone() { return std::get<11>(data)[offset]; }
      const auto& c_phone() const { return std::get<11>(data)[offset]; }
      auto& c_since() { return std::get<12>(data)[offset]; }
      const auto& c_since() const { return std::get<12>(data)[offset]; }
      auto& c_credit() { return std::get<13>(data)[offset]; }
      const auto& c_credit() const { return std::get<13>(data)[offset]; }
      auto& c_credit_lim() { return std::get<14>(data)[offset]; }
      const auto& c_credit_lim() const { return std::get<14>(data)[offset]; }
      auto& c_discount() { return std::get<15>(data)[offset]; }
      const auto& c_discount() const { return std::get<15>(data)[offset]; }
      auto& c_balance() { return std::get<16>(data)[offset]; }
      const auto& c_balance() const { return std::get<16>(data)[offset]; }
      auto& c_ytd_paymenr() { return std::get<17>(data)[offset]; }
      const auto& c_ytd_paymenr() const { return std::get<17>(data)[offset]; }
      auto& c_payment_cnt() { return std::get<18>(data)[offset]; }
      const auto& c_payment_cnt() const { return std::get<18>(data)[offset]; }
      auto& c_delivery_cnt() { return std::get<19>(data)[offset]; }
      const auto& c_delivery_cnt() const { return std::get<19>(data)[offset]; }
      auto& c_data() { return std::get<20>(data)[offset]; }
      const auto& c_data() const { return std::get<20>(data)[offset]; }
      
    private:
      storage_type &data;
      size_t offset;
  };

  class iterator {
    public:
      iterator(storage_type &data, size_t offset) : data(data), offset(offset) {}

      iterator& operator++() {++offset; return *this;}
      iterator operator++(int) {auto copy = *this; offset++; return copy;}
      iterator& operator--() {--offset; return *this;}
      iterator operator--(int) {auto copy = *this; offset--; return copy;}

      bool operator==(const iterator& rhs) const {return &data == &rhs.data && offset == rhs.offset;}
      bool operator!=(const iterator& rhs) const {return !(*this == rhs);}

      size_t operator-(const iterator& rhs) const {return offset - rhs.offset;}

      auto operator*() { return dereferenced_iterator{data, offset}; }

    private:
      storage_type &data;
      size_t offset;
  };

  auto begin() {return iterator{data, 0};}
  auto end() {return iterator{data, size()};}

  storage_type data;
};
//-----------------------------------------------------------------------------
struct history_t {
  typedef std::tuple<
    std::vector<Integer>, //h_c_id
    std::vector<Integer>, //h_c_d_id
    std::vector<Integer>, //h_c_w_id
    std::vector<Integer>, //h_d_id
    std::vector<Integer>, //h_w_id
    std::vector<Timestamp>, //h_date
    std::vector<Numeric<6,2>>, //h_amount
    std::vector<Varchar<24>> //h_data
  > storage_type;

  auto size() {return std::get<0>(data).size();}

  auto& h_c_id() { return std::get<0>(data); }
  const auto& h_c_id() const { return std::get<0>(data); }
  auto& h_c_d_id() { return std::get<1>(data); }
  const auto& h_c_d_id() const { return std::get<1>(data); }
  auto& h_c_w_id() { return std::get<2>(data); }
  const auto& h_c_w_id() const { return std::get<2>(data); }
  auto& h_d_id() { return std::get<3>(data); }
  const auto& h_d_id() const { return std::get<3>(data); }
  auto& h_w_id() { return std::get<4>(data); }
  const auto& h_w_id() const { return std::get<4>(data); }
  auto& h_date() { return std::get<5>(data); }
  const auto& h_date() const { return std::get<5>(data); }
  auto& h_amount() { return std::get<6>(data); }
  const auto& h_amount() const { return std::get<6>(data); }
  auto& h_data() { return std::get<7>(data); }
  const auto& h_data() const { return std::get<7>(data); }
  

  class dereferenced_iterator {
    public:
      dereferenced_iterator(storage_type &data, size_t offset) : data(data), offset(offset) {}

      auto& h_c_id() { return std::get<0>(data)[offset]; }
      const auto& h_c_id() const { return std::get<0>(data)[offset]; }
      auto& h_c_d_id() { return std::get<1>(data)[offset]; }
      const auto& h_c_d_id() const { return std::get<1>(data)[offset]; }
      auto& h_c_w_id() { return std::get<2>(data)[offset]; }
      const auto& h_c_w_id() const { return std::get<2>(data)[offset]; }
      auto& h_d_id() { return std::get<3>(data)[offset]; }
      const auto& h_d_id() const { return std::get<3>(data)[offset]; }
      auto& h_w_id() { return std::get<4>(data)[offset]; }
      const auto& h_w_id() const { return std::get<4>(data)[offset]; }
      auto& h_date() { return std::get<5>(data)[offset]; }
      const auto& h_date() const { return std::get<5>(data)[offset]; }
      auto& h_amount() { return std::get<6>(data)[offset]; }
      const auto& h_amount() const { return std::get<6>(data)[offset]; }
      auto& h_data() { return std::get<7>(data)[offset]; }
      const auto& h_data() const { return std::get<7>(data)[offset]; }
      
    private:
      storage_type &data;
      size_t offset;
  };

  class iterator {
    public:
      iterator(storage_type &data, size_t offset) : data(data), offset(offset) {}

      iterator& operator++() {++offset; return *this;}
      iterator operator++(int) {auto copy = *this; offset++; return copy;}
      iterator& operator--() {--offset; return *this;}
      iterator operator--(int) {auto copy = *this; offset--; return copy;}

      bool operator==(const iterator& rhs) const {return &data == &rhs.data && offset == rhs.offset;}
      bool operator!=(const iterator& rhs) const {return !(*this == rhs);}

      size_t operator-(const iterator& rhs) const {return offset - rhs.offset;}

      auto operator*() { return dereferenced_iterator{data, offset}; }

    private:
      storage_type &data;
      size_t offset;
  };

  auto begin() {return iterator{data, 0};}
  auto end() {return iterator{data, size()};}

  storage_type data;
};
//-----------------------------------------------------------------------------
struct neworder_t {
  typedef std::tuple<
    std::vector<Integer>, //no_o_id
    std::vector<Integer>, //no_d_id
    std::vector<Integer> //no_w_id
  > storage_type;

  auto size() {return std::get<0>(data).size();}

  auto& no_o_id() { return std::get<0>(data); }
  const auto& no_o_id() const { return std::get<0>(data); }
  auto& no_d_id() { return std::get<1>(data); }
  const auto& no_d_id() const { return std::get<1>(data); }
  auto& no_w_id() { return std::get<2>(data); }
  const auto& no_w_id() const { return std::get<2>(data); }
  

  class dereferenced_iterator {
    public:
      dereferenced_iterator(storage_type &data, size_t offset) : data(data), offset(offset) {}

      auto& no_o_id() { return std::get<0>(data)[offset]; }
      const auto& no_o_id() const { return std::get<0>(data)[offset]; }
      auto& no_d_id() { return std::get<1>(data)[offset]; }
      const auto& no_d_id() const { return std::get<1>(data)[offset]; }
      auto& no_w_id() { return std::get<2>(data)[offset]; }
      const auto& no_w_id() const { return std::get<2>(data)[offset]; }
      
    private:
      storage_type &data;
      size_t offset;
  };

  class iterator {
    public:
      iterator(storage_type &data, size_t offset) : data(data), offset(offset) {}

      iterator& operator++() {++offset; return *this;}
      iterator operator++(int) {auto copy = *this; offset++; return copy;}
      iterator& operator--() {--offset; return *this;}
      iterator operator--(int) {auto copy = *this; offset--; return copy;}

      bool operator==(const iterator& rhs) const {return &data == &rhs.data && offset == rhs.offset;}
      bool operator!=(const iterator& rhs) const {return !(*this == rhs);}

      size_t operator-(const iterator& rhs) const {return offset - rhs.offset;}

      auto operator*() { return dereferenced_iterator{data, offset}; }

    private:
      storage_type &data;
      size_t offset;
  };

  auto begin() {return iterator{data, 0};}
  auto end() {return iterator{data, size()};}

  storage_type data;
};
//-----------------------------------------------------------------------------
struct order_t {
  typedef std::tuple<
    std::vector<Integer>, //o_id
    std::vector<Integer>, //o_d_id
    std::vector<Integer>, //o_w_id
    std::vector<Integer>, //o_c_id
    std::vector<Timestamp>, //o_entry_d
    std::vector<Integer>, //o_carrier_id
    std::vector<Numeric<2,0>>, //o_ol_cnt
    std::vector<Numeric<1,0>> //o_all_local
  > storage_type;

  auto size() {return std::get<0>(data).size();}

  auto& o_id() { return std::get<0>(data); }
  const auto& o_id() const { return std::get<0>(data); }
  auto& o_d_id() { return std::get<1>(data); }
  const auto& o_d_id() const { return std::get<1>(data); }
  auto& o_w_id() { return std::get<2>(data); }
  const auto& o_w_id() const { return std::get<2>(data); }
  auto& o_c_id() { return std::get<3>(data); }
  const auto& o_c_id() const { return std::get<3>(data); }
  auto& o_entry_d() { return std::get<4>(data); }
  const auto& o_entry_d() const { return std::get<4>(data); }
  auto& o_carrier_id() { return std::get<5>(data); }
  const auto& o_carrier_id() const { return std::get<5>(data); }
  auto& o_ol_cnt() { return std::get<6>(data); }
  const auto& o_ol_cnt() const { return std::get<6>(data); }
  auto& o_all_local() { return std::get<7>(data); }
  const auto& o_all_local() const { return std::get<7>(data); }
  

  class dereferenced_iterator {
    public:
      dereferenced_iterator(storage_type &data, size_t offset) : data(data), offset(offset) {}

      auto& o_id() { return std::get<0>(data)[offset]; }
      const auto& o_id() const { return std::get<0>(data)[offset]; }
      auto& o_d_id() { return std::get<1>(data)[offset]; }
      const auto& o_d_id() const { return std::get<1>(data)[offset]; }
      auto& o_w_id() { return std::get<2>(data)[offset]; }
      const auto& o_w_id() const { return std::get<2>(data)[offset]; }
      auto& o_c_id() { return std::get<3>(data)[offset]; }
      const auto& o_c_id() const { return std::get<3>(data)[offset]; }
      auto& o_entry_d() { return std::get<4>(data)[offset]; }
      const auto& o_entry_d() const { return std::get<4>(data)[offset]; }
      auto& o_carrier_id() { return std::get<5>(data)[offset]; }
      const auto& o_carrier_id() const { return std::get<5>(data)[offset]; }
      auto& o_ol_cnt() { return std::get<6>(data)[offset]; }
      const auto& o_ol_cnt() const { return std::get<6>(data)[offset]; }
      auto& o_all_local() { return std::get<7>(data)[offset]; }
      const auto& o_all_local() const { return std::get<7>(data)[offset]; }
      
    private:
      storage_type &data;
      size_t offset;
  };

  class iterator {
    public:
      iterator(storage_type &data, size_t offset) : data(data), offset(offset) {}

      iterator& operator++() {++offset; return *this;}
      iterator operator++(int) {auto copy = *this; offset++; return copy;}
      iterator& operator--() {--offset; return *this;}
      iterator operator--(int) {auto copy = *this; offset--; return copy;}

      bool operator==(const iterator& rhs) const {return &data == &rhs.data && offset == rhs.offset;}
      bool operator!=(const iterator& rhs) const {return !(*this == rhs);}

      size_t operator-(const iterator& rhs) const {return offset - rhs.offset;}

      auto operator*() { return dereferenced_iterator{data, offset}; }

    private:
      storage_type &data;
      size_t offset;
  };

  auto begin() {return iterator{data, 0};}
  auto end() {return iterator{data, size()};}

  storage_type data;
};
//-----------------------------------------------------------------------------
struct orderline_t {
  typedef std::tuple<
    std::vector<Integer>, //ol_o_id
    std::vector<Integer>, //ol_d_id
    std::vector<Integer>, //ol_w_id
    std::vector<Integer>, //ol_number
    std::vector<Integer>, //ol_i_id
    std::vector<Integer>, //ol_supply_w_id
    std::vector<Timestamp>, //ol_delivery_d
    std::vector<Numeric<2,0>>, //ol_quantity
    std::vector<Numeric<6,2>>, //ol_amount
    std::vector<Char<24>> //ol_dist_info
  > storage_type;

  auto size() {return std::get<0>(data).size();}

  auto& ol_o_id() { return std::get<0>(data); }
  const auto& ol_o_id() const { return std::get<0>(data); }
  auto& ol_d_id() { return std::get<1>(data); }
  const auto& ol_d_id() const { return std::get<1>(data); }
  auto& ol_w_id() { return std::get<2>(data); }
  const auto& ol_w_id() const { return std::get<2>(data); }
  auto& ol_number() { return std::get<3>(data); }
  const auto& ol_number() const { return std::get<3>(data); }
  auto& ol_i_id() { return std::get<4>(data); }
  const auto& ol_i_id() const { return std::get<4>(data); }
  auto& ol_supply_w_id() { return std::get<5>(data); }
  const auto& ol_supply_w_id() const { return std::get<5>(data); }
  auto& ol_delivery_d() { return std::get<6>(data); }
  const auto& ol_delivery_d() const { return std::get<6>(data); }
  auto& ol_quantity() { return std::get<7>(data); }
  const auto& ol_quantity() const { return std::get<7>(data); }
  auto& ol_amount() { return std::get<8>(data); }
  const auto& ol_amount() const { return std::get<8>(data); }
  auto& ol_dist_info() { return std::get<9>(data); }
  const auto& ol_dist_info() const { return std::get<9>(data); }
  

  class dereferenced_iterator {
    public:
      dereferenced_iterator(storage_type &data, size_t offset) : data(data), offset(offset) {}

      auto& ol_o_id() { return std::get<0>(data)[offset]; }
      const auto& ol_o_id() const { return std::get<0>(data)[offset]; }
      auto& ol_d_id() { return std::get<1>(data)[offset]; }
      const auto& ol_d_id() const { return std::get<1>(data)[offset]; }
      auto& ol_w_id() { return std::get<2>(data)[offset]; }
      const auto& ol_w_id() const { return std::get<2>(data)[offset]; }
      auto& ol_number() { return std::get<3>(data)[offset]; }
      const auto& ol_number() const { return std::get<3>(data)[offset]; }
      auto& ol_i_id() { return std::get<4>(data)[offset]; }
      const auto& ol_i_id() const { return std::get<4>(data)[offset]; }
      auto& ol_supply_w_id() { return std::get<5>(data)[offset]; }
      const auto& ol_supply_w_id() const { return std::get<5>(data)[offset]; }
      auto& ol_delivery_d() { return std::get<6>(data)[offset]; }
      const auto& ol_delivery_d() const { return std::get<6>(data)[offset]; }
      auto& ol_quantity() { return std::get<7>(data)[offset]; }
      const auto& ol_quantity() const { return std::get<7>(data)[offset]; }
      auto& ol_amount() { return std::get<8>(data)[offset]; }
      const auto& ol_amount() const { return std::get<8>(data)[offset]; }
      auto& ol_dist_info() { return std::get<9>(data)[offset]; }
      const auto& ol_dist_info() const { return std::get<9>(data)[offset]; }
      
    private:
      storage_type &data;
      size_t offset;
  };

  class iterator {
    public:
      iterator(storage_type &data, size_t offset) : data(data), offset(offset) {}

      iterator& operator++() {++offset; return *this;}
      iterator operator++(int) {auto copy = *this; offset++; return copy;}
      iterator& operator--() {--offset; return *this;}
      iterator operator--(int) {auto copy = *this; offset--; return copy;}

      bool operator==(const iterator& rhs) const {return &data == &rhs.data && offset == rhs.offset;}
      bool operator!=(const iterator& rhs) const {return !(*this == rhs);}

      size_t operator-(const iterator& rhs) const {return offset - rhs.offset;}

      auto operator*() { return dereferenced_iterator{data, offset}; }

    private:
      storage_type &data;
      size_t offset;
  };

  auto begin() {return iterator{data, 0};}
  auto end() {return iterator{data, size()};}

  storage_type data;
};
//-----------------------------------------------------------------------------
struct item_t {
  typedef std::tuple<
    std::vector<Integer>, //i_id
    std::vector<Integer>, //i_im_id
    std::vector<Varchar<24>>, //i_name
    std::vector<Numeric<5,2>>, //i_price
    std::vector<Varchar<50>> //i_data
  > storage_type;

  auto size() {return std::get<0>(data).size();}

  auto& i_id() { return std::get<0>(data); }
  const auto& i_id() const { return std::get<0>(data); }
  auto& i_im_id() { return std::get<1>(data); }
  const auto& i_im_id() const { return std::get<1>(data); }
  auto& i_name() { return std::get<2>(data); }
  const auto& i_name() const { return std::get<2>(data); }
  auto& i_price() { return std::get<3>(data); }
  const auto& i_price() const { return std::get<3>(data); }
  auto& i_data() { return std::get<4>(data); }
  const auto& i_data() const { return std::get<4>(data); }
  

  class dereferenced_iterator {
    public:
      dereferenced_iterator(storage_type &data, size_t offset) : data(data), offset(offset) {}

      auto& i_id() { return std::get<0>(data)[offset]; }
      const auto& i_id() const { return std::get<0>(data)[offset]; }
      auto& i_im_id() { return std::get<1>(data)[offset]; }
      const auto& i_im_id() const { return std::get<1>(data)[offset]; }
      auto& i_name() { return std::get<2>(data)[offset]; }
      const auto& i_name() const { return std::get<2>(data)[offset]; }
      auto& i_price() { return std::get<3>(data)[offset]; }
      const auto& i_price() const { return std::get<3>(data)[offset]; }
      auto& i_data() { return std::get<4>(data)[offset]; }
      const auto& i_data() const { return std::get<4>(data)[offset]; }
      
    private:
      storage_type &data;
      size_t offset;
  };

  class iterator {
    public:
      iterator(storage_type &data, size_t offset) : data(data), offset(offset) {}

      iterator& operator++() {++offset; return *this;}
      iterator operator++(int) {auto copy = *this; offset++; return copy;}
      iterator& operator--() {--offset; return *this;}
      iterator operator--(int) {auto copy = *this; offset--; return copy;}

      bool operator==(const iterator& rhs) const {return &data == &rhs.data && offset == rhs.offset;}
      bool operator!=(const iterator& rhs) const {return !(*this == rhs);}

      size_t operator-(const iterator& rhs) const {return offset - rhs.offset;}

      auto operator*() { return dereferenced_iterator{data, offset}; }

    private:
      storage_type &data;
      size_t offset;
  };

  auto begin() {return iterator{data, 0};}
  auto end() {return iterator{data, size()};}

  storage_type data;
};
//-----------------------------------------------------------------------------
struct stock_t {
  typedef std::tuple<
    std::vector<Integer>, //s_i_id
    std::vector<Integer>, //s_w_id
    std::vector<Numeric<4,0>>, //s_quantity
    std::vector<Char<24>>, //s_dist_01
    std::vector<Char<24>>, //s_dist_02
    std::vector<Char<24>>, //s_dist_03
    std::vector<Char<24>>, //s_dist_04
    std::vector<Char<24>>, //s_dist_05
    std::vector<Char<24>>, //s_dist_06
    std::vector<Char<24>>, //s_dist_07
    std::vector<Char<24>>, //s_dist_08
    std::vector<Char<24>>, //s_dist_09
    std::vector<Char<24>>, //s_dist_10
    std::vector<Numeric<8,0>>, //s_ytd
    std::vector<Numeric<4,0>>, //s_order_cnt
    std::vector<Numeric<4,0>>, //s_remote_cnt
    std::vector<Varchar<50>> //s_data
  > storage_type;

  auto size() {return std::get<0>(data).size();}

  auto& s_i_id() { return std::get<0>(data); }
  const auto& s_i_id() const { return std::get<0>(data); }
  auto& s_w_id() { return std::get<1>(data); }
  const auto& s_w_id() const { return std::get<1>(data); }
  auto& s_quantity() { return std::get<2>(data); }
  const auto& s_quantity() const { return std::get<2>(data); }
  auto& s_dist_01() { return std::get<3>(data); }
  const auto& s_dist_01() const { return std::get<3>(data); }
  auto& s_dist_02() { return std::get<4>(data); }
  const auto& s_dist_02() const { return std::get<4>(data); }
  auto& s_dist_03() { return std::get<5>(data); }
  const auto& s_dist_03() const { return std::get<5>(data); }
  auto& s_dist_04() { return std::get<6>(data); }
  const auto& s_dist_04() const { return std::get<6>(data); }
  auto& s_dist_05() { return std::get<7>(data); }
  const auto& s_dist_05() const { return std::get<7>(data); }
  auto& s_dist_06() { return std::get<8>(data); }
  const auto& s_dist_06() const { return std::get<8>(data); }
  auto& s_dist_07() { return std::get<9>(data); }
  const auto& s_dist_07() const { return std::get<9>(data); }
  auto& s_dist_08() { return std::get<10>(data); }
  const auto& s_dist_08() const { return std::get<10>(data); }
  auto& s_dist_09() { return std::get<11>(data); }
  const auto& s_dist_09() const { return std::get<11>(data); }
  auto& s_dist_10() { return std::get<12>(data); }
  const auto& s_dist_10() const { return std::get<12>(data); }
  auto& s_ytd() { return std::get<13>(data); }
  const auto& s_ytd() const { return std::get<13>(data); }
  auto& s_order_cnt() { return std::get<14>(data); }
  const auto& s_order_cnt() const { return std::get<14>(data); }
  auto& s_remote_cnt() { return std::get<15>(data); }
  const auto& s_remote_cnt() const { return std::get<15>(data); }
  auto& s_data() { return std::get<16>(data); }
  const auto& s_data() const { return std::get<16>(data); }
  

  class dereferenced_iterator {
    public:
      dereferenced_iterator(storage_type &data, size_t offset) : data(data), offset(offset) {}

      auto& s_i_id() { return std::get<0>(data)[offset]; }
      const auto& s_i_id() const { return std::get<0>(data)[offset]; }
      auto& s_w_id() { return std::get<1>(data)[offset]; }
      const auto& s_w_id() const { return std::get<1>(data)[offset]; }
      auto& s_quantity() { return std::get<2>(data)[offset]; }
      const auto& s_quantity() const { return std::get<2>(data)[offset]; }
      auto& s_dist_01() { return std::get<3>(data)[offset]; }
      const auto& s_dist_01() const { return std::get<3>(data)[offset]; }
      auto& s_dist_02() { return std::get<4>(data)[offset]; }
      const auto& s_dist_02() const { return std::get<4>(data)[offset]; }
      auto& s_dist_03() { return std::get<5>(data)[offset]; }
      const auto& s_dist_03() const { return std::get<5>(data)[offset]; }
      auto& s_dist_04() { return std::get<6>(data)[offset]; }
      const auto& s_dist_04() const { return std::get<6>(data)[offset]; }
      auto& s_dist_05() { return std::get<7>(data)[offset]; }
      const auto& s_dist_05() const { return std::get<7>(data)[offset]; }
      auto& s_dist_06() { return std::get<8>(data)[offset]; }
      const auto& s_dist_06() const { return std::get<8>(data)[offset]; }
      auto& s_dist_07() { return std::get<9>(data)[offset]; }
      const auto& s_dist_07() const { return std::get<9>(data)[offset]; }
      auto& s_dist_08() { return std::get<10>(data)[offset]; }
      const auto& s_dist_08() const { return std::get<10>(data)[offset]; }
      auto& s_dist_09() { return std::get<11>(data)[offset]; }
      const auto& s_dist_09() const { return std::get<11>(data)[offset]; }
      auto& s_dist_10() { return std::get<12>(data)[offset]; }
      const auto& s_dist_10() const { return std::get<12>(data)[offset]; }
      auto& s_ytd() { return std::get<13>(data)[offset]; }
      const auto& s_ytd() const { return std::get<13>(data)[offset]; }
      auto& s_order_cnt() { return std::get<14>(data)[offset]; }
      const auto& s_order_cnt() const { return std::get<14>(data)[offset]; }
      auto& s_remote_cnt() { return std::get<15>(data)[offset]; }
      const auto& s_remote_cnt() const { return std::get<15>(data)[offset]; }
      auto& s_data() { return std::get<16>(data)[offset]; }
      const auto& s_data() const { return std::get<16>(data)[offset]; }
      
    private:
      storage_type &data;
      size_t offset;
  };

  class iterator {
    public:
      iterator(storage_type &data, size_t offset) : data(data), offset(offset) {}

      iterator& operator++() {++offset; return *this;}
      iterator operator++(int) {auto copy = *this; offset++; return copy;}
      iterator& operator--() {--offset; return *this;}
      iterator operator--(int) {auto copy = *this; offset--; return copy;}

      bool operator==(const iterator& rhs) const {return &data == &rhs.data && offset == rhs.offset;}
      bool operator!=(const iterator& rhs) const {return !(*this == rhs);}

      size_t operator-(const iterator& rhs) const {return offset - rhs.offset;}

      auto operator*() { return dereferenced_iterator{data, offset}; }

    private:
      storage_type &data;
      size_t offset;
  };

  auto begin() {return iterator{data, 0};}
  auto end() {return iterator{data, size()};}

  storage_type data;
};
//-----------------------------------------------------------------------------
warehouse_t warehouse;
district_t district;
customer_t customer;
history_t history;
neworder_t neworder;
order_t order;
orderline_t orderline;
item_t item;
stock_t stock;
//-----------------------------------------------------------------------------
#endif
