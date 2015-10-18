#ifndef H_parseTbl
#define H_parseTbl

#include <vector>
#include <tuple>
#include "task1/Types.hpp"

//---------------------------------------------------------------------------
// internal template magic
namespace detail {
  //compares two size_t values for equality
  template<size_t T1, size_t T2>
  struct is_equal_size_t : std::false_type {};
  template<size_t T>
  struct is_equal_size_t<T,T> : std::true_type {};
  //---------------------------------------------------------------------------
  //parses a tbl file into already existing vectors. Expects a sequential index sequence as last parameter.
  template<typename IterT, typename ...RowTs, size_t ...idcs>
  void _parseTbl(const IterT begin, const IterT end, std::tuple<RowTs...>& table, std::index_sequence<idcs...>) {
    auto pos = begin;
    using swallow = int[]; // guaranties left to right order
    while(pos != end) {
      (void)swallow{0, (void(parseTblRow(pos, end, std::get<idcs>(table), is_equal_size_t<idcs, sizeof...(idcs)-1>())), 0)...};
    }
  }
  //---------------------------------------------------------------------------
  //passes the required index_sequence to _parseTbl
  template<typename IterT, typename ...RowTs, size_t ...idcs>
  void parseTbl(const IterT begin, const IterT end, std::tuple<RowTs...>& table) {
    return _parseTbl(begin, end, table, std::index_sequence_for<RowTs...>());
  }
  //---------------------------------------------------------------------------
  //parses one column (not at the end of a row)
  template<typename IterT, typename ColT>
  void parseTblRow(IterT &pos, const IterT end, std::vector<ColT>& column, std::false_type /*is_last_column*/) {
    auto begin = pos;
    uint32_t len = 0;
    while(*pos != '|') {
      len++;
      pos++;
      if(pos == end || *pos == '\n') {
        throw "unexpected end of row";
      }
    }
    pos++; // skip the colum sepearator
    column.push_back(ColT::castString(&*begin, len));
  }
  //---------------------------------------------------------------------------
  //parses the last column of a table row
  template<typename IterT, typename ColT>
  void parseTblRow(IterT &pos, const IterT end, std::vector<ColT>& column, std::true_type /*is_last_column*/) {
    auto begin = pos;
    uint32_t len = 0;
    while(pos != end && *pos != '\n') {
      if(*pos == '|') {
        throw "unexpected column seperator";
      }
      len++;
      pos++;
    }
    if(pos != end) pos++; // skip the new line
    column.push_back(ColT::castString(&*begin, len));
  }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//parses a complete table and returns its contents
template<typename TableType, typename IterT>
TableType parseTbl (const IterT begin, const IterT end) {
  TableType table;
  detail::parseTbl(begin, end, table);
  return table;
}
//---------------------------------------------------------------------------
#endif
