#include <gtest/gtest.h>

#include <string>
using namespace std;

#include "schema/Types.hpp"
#include "schema/parseTbl.hpp"


TEST(ParseTblTest, parsesATable) {
  typedef tuple<std::vector<Integer>, std::vector<Numeric<1,1>>, std::vector<Varchar<8>>> TableType;
  auto tblData = std::string{"1|1.2|test\n2|2.4|str2"};
  try {
    auto table = parseTbl<TableType>(tblData.data(), tblData.data() + tblData.length());
    ASSERT_EQ(2, get<0>(table).size());
    ASSERT_EQ(2, get<1>(table).size());
    ASSERT_EQ(2, get<2>(table).size());
    EXPECT_EQ(Integer(1), get<0>(table)[0]);
    EXPECT_EQ(Integer(2), get<0>(table)[1]);
    typedef Numeric<1,1> Numeric_1_1;
    EXPECT_EQ(Numeric_1_1::buildRaw(12), get<1>(table)[0]);
    EXPECT_EQ(Numeric_1_1::buildRaw(24), get<1>(table)[1]);
    auto test_string = Varchar<8>{4,"test"};
    EXPECT_EQ(test_string, get<2>(table)[0]);
  } catch(const char* e) {
    std::cout << e << std::endl;
  }
}

TEST(ParseTblTest, throwsOnUnexpectedEOL) {
  typedef tuple<std::vector<Integer>, std::vector<Numeric<1,1>>, std::vector<Varchar<8>>> TableType;
  auto tblData = std::string{R"(\
1|1.2
2|2.4)"};
  try {
    auto table = parseTbl<TableType>(tblData.data(), tblData.data() + tblData.length());
    FAIL();
  } catch(...) {}
}

TEST(ParseTblTest, throwsOnUnexpectedColumn) {
  typedef tuple<std::vector<Integer>> TableType;
  auto tblData = std::string{R"(\
1|1.2
2|2.4)"};
  try {
    auto table = parseTbl<TableType>(tblData.data(), tblData.data() + tblData.length());
    FAIL();
  } catch(...) {}
}
