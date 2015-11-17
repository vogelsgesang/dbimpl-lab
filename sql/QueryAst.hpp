#ifndef H_QueryAst
#define H_QueryAst

#include <string>
#include <vector>
#include <memory>
#include "schema/Schema.hpp"

namespace ast {

struct Query {
  std::vector<std::string> selectedColumns;
  std::vector<std::string> tables;
  std::vector<std::pair<std::string, std::string>> joinPredicates;
  std::vector<std::pair<std::string, uint64_t>> filterPredicates;
};

}

#endif
