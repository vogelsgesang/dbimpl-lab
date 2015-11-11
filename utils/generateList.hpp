#ifndef H_generateList
#define H_generateList

#include <ostream>
#include <string>

template<typename T, typename Callable>
static void generateList(std::ostream& out, const T& elements,
                         Callable cb, std::string seperator = ", ") {
  auto first = true;
  for(const auto& elem : elements) {
    if(!first) out << seperator;
    cb(out, elem);
    first = false;
  }
}

#endif
