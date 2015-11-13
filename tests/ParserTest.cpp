#include <gtest/gtest.h>
#include <sstream>

#include "sql/Parser.hpp"

TEST(SchemaParserTest, parsesAnEmptySchemaDefinitions) {
  auto parser = Parser{};
  std::string schemaDefinition(" \n \t ");
  std::istringstream in(schemaDefinition);

  auto schema = parser.parseSqlSchema(in);
  EXPECT_EQ(0, schema->tables.size());
}

TEST(SchemaParserTest, parsesValidSchemaDefinitions) {
  auto parser = Parser{};
  std::string schemaDefinition(
      "CREATE TABLE test1 ("
      "  attr1 integer,"
      "  attr2 INTEGER,"
      "  attr3 Numeric(9, 2),"
      "  attr4 CHAR(1),"
      "  attr5 VarCHAR(10) not NULL,"
      "  PRIMARY key (attr2, attr4)"
      ");"
      "CREATE TABLE test2 ("
      "  attr1 integer"
      ");"
      );
  std::istringstream in(schemaDefinition);

  try {
    auto schema = parser.parseSqlSchema(in);

    ASSERT_EQ(2, schema->tables.size());
    EXPECT_EQ(std::string("test1"), schema->tables[0].name);
    EXPECT_EQ(std::string("test2"), schema->tables[1].name);

    ASSERT_EQ(5, schema->tables[0].columns.size());

    EXPECT_EQ(std::string("attr1"), schema->tables[0].columns[0].name);
    EXPECT_EQ(DataTypeTag::Integer, schema->tables[0].columns[0].type.typeTag);
    EXPECT_FALSE(                   schema->tables[0].columns[0].notNull);

    EXPECT_EQ(std::string("attr2"), schema->tables[0].columns[1].name);
    EXPECT_EQ(DataTypeTag::Integer, schema->tables[0].columns[1].type.typeTag);
    EXPECT_FALSE(                   schema->tables[0].columns[1].notNull);

    EXPECT_EQ(std::string("attr3"), schema->tables[0].columns[2].name);
    EXPECT_EQ(DataTypeTag::Numeric, schema->tables[0].columns[2].type.typeTag);
    EXPECT_EQ(9,                    schema->tables[0].columns[2].type.attributes.numeric.integerPlaces);
    EXPECT_EQ(2,                    schema->tables[0].columns[2].type.attributes.numeric.decimalPlaces);
    EXPECT_FALSE(                   schema->tables[0].columns[2].notNull);

    EXPECT_EQ(std::string("attr4"), schema->tables[0].columns[3].name);
    EXPECT_EQ(DataTypeTag::Char,       schema->tables[0].columns[3].type.typeTag);
    EXPECT_EQ(10,                   schema->tables[0].columns[4].type.attributes.charLen);
    EXPECT_FALSE(                   schema->tables[0].columns[3].notNull);

    EXPECT_EQ(std::string("attr5"), schema->tables[0].columns[4].name);
    EXPECT_EQ(DataTypeTag::VarChar,    schema->tables[0].columns[4].type.typeTag);
    EXPECT_EQ(10,                   schema->tables[0].columns[4].type.attributes.charLen);
    EXPECT_TRUE(                    schema->tables[0].columns[4].notNull);

    EXPECT_EQ(std::string("attr1"), schema->tables[1].columns[0].name);
    EXPECT_EQ(DataTypeTag::Integer,    schema->tables[1].columns[0].type.typeTag);
    EXPECT_FALSE(                   schema->tables[1].columns[0].notNull);

    ASSERT_EQ(2, schema->tables[0].primaryKey.size());
    EXPECT_EQ(1, schema->tables[0].primaryKey[0]);
    EXPECT_EQ(3, schema->tables[0].primaryKey[1]);
    ASSERT_EQ(0, schema->tables[1].primaryKey.size());
  } catch(ParserError& e) {
    std::cout << e.what() << std::endl;
    FAIL();
  }
}

TEST(SchemaParserTest, parsesIndexDefinitions) {
  auto parser = Parser{};
  std::string schemaDefinition(
      "CREATE TABLE test1 ("
      "  attr1 integer,"
      "  attr2 integer,"
      "  attr3 integer"
      ");"
      "CREATE INDEX idx ON test1 (attr1, attr3);"
      );
  std::istringstream in(schemaDefinition);

  try {
    auto schema = parser.parseSqlSchema(in);

    ASSERT_EQ(1, schema->tables.size());
    EXPECT_EQ(std::string("test1"), schema->tables[0].name);

    ASSERT_EQ(3, schema->tables[0].columns.size());
    EXPECT_EQ(std::string("attr1"), schema->tables[0].columns[0].name);
    EXPECT_EQ(std::string("attr2"), schema->tables[0].columns[1].name);
    EXPECT_EQ(std::string("attr3"), schema->tables[0].columns[2].name);

    ASSERT_EQ(1, schema->tables[0].indices.size());
    ASSERT_EQ(0, schema->tables[0].indices[0].columns[0]);
    ASSERT_EQ(2, schema->tables[0].indices[0].columns[1]);
  } catch(ParserError& e) {
    std::cout << e.what() << std::endl;
    FAIL();
  }
}

TEST(SchemaParserTest, rejectsTruncatedSchemaDefinitions) {
  Parser parser;
  std::string schemaDefinition(
      "CREATE TABLE test("
      "  attr1 integer,"
      );
  std::istringstream in(schemaDefinition);

  EXPECT_ANY_THROW(auto schema = parser.parseSqlSchema(in));
}
