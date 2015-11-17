#ifndef H_buildQueryTree
#define H_buildQueryTree

#include <memory>
#include "schema/Schema.hpp"
#include "codegen/QueryOperator.hpp"
#include "sql/QueryAst.hpp"

std::unique_ptr<QueryOperator> buildQueryTree(const ast::Query& query, const Schema& schema);

#endif
