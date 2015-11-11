#ifndef H_InformationUnit
#define H_InformationUnit

#include <string>
#include <sstream>
#include "schema/Schema.hpp"

class InformationUnit {
  public:
    InformationUnit(const DataType& type, const std::string& name = "")
    : type(type) {
      static uint64_t nextVarId = 0;
      std::ostringstream buffer;
      buffer << "iu_" << name << "_" << (nextVarId++);
      varname = buffer.str();
    }

    auto getType() const {return type;}
    auto getVarname() const {return varname;}

  protected:
    DataType type;
    std::string varname;
};

#endif
