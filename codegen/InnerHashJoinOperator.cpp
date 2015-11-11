#include "InnerHashJoinOperator.hpp"
#include "utils/generateList.hpp"


void InnerHashJoinOperator::produce(std::ostream& out, IUSet& requiredIUs) {
  //figure out which IUs are required from the left subtree
  IUSet leftProduced = left->getProduced();
  leftRequiredIUs.clear();
  for(const auto& iu : requiredIUs) {
    if(leftProduced.count(iu)) {
      leftRequiredIUs.insert(iu);
    }
  }
  //initialize the hashmap
  out << "std::unordered_multimap<std::tuple<";
  generateList(out, joinColumns, [](auto& out, auto& cols) {
                out << cols.first->getType().generateCppCode();
              });
  out << ">, std::tuple<";
  generateList(out, leftRequiredIUs, [](auto& out, auto& iu) {
    out << iu->getType().generateCppCode();
  });
  out <<">> ht_" << varnameId << ";";
  //add join columns to the sets of required UIs
  auto downwardsLeftRequiredIUs = leftRequiredIUs;
  IUSet downwardsRightRequiredIUs = requiredIUs;
  for(const auto& iu : leftRequiredIUs) {
    downwardsRightRequiredIUs.erase(iu);
  }
  for(const auto& cols : joinColumns) {
    downwardsLeftRequiredIUs.insert(cols.first);
    downwardsRightRequiredIUs.insert(cols.second);
  }
  left->produce(out, downwardsLeftRequiredIUs);
  right->produce(out, downwardsRightRequiredIUs);
}

void InnerHashJoinOperator::consume(std::ostream& out, const QueryOperator& sourceOp) {
  if(&sourceOp == left.get()) {
    out << "ht_" << varnameId << ".insert(std::make_pair(std::make_tuple(";
    generateList(out, joinColumns, [](auto& out, auto& cols) {
                  out << cols.first->getVarname();
                });
    out << "), std::make_tuple(";
    generateList(out, leftRequiredIUs, [](auto& out, auto& iu) {
                  out << iu->getVarname();
                });
    out << ")));";
  } else {
    out << "{";
    out << "auto range_iters = ht_" << varnameId << ".equal_range(std::make_tuple(";
    generateList(out, joinColumns, [](auto& out, auto& cols) {
                  out << cols.second->getVarname();
                });
    out << "));";
    out << "auto iter = range_iters.first;"
           "auto limit = range_iters.second;";
    out << "for(;iter != limit; iter++) {";
    unsigned tupleIdx = 0;
    for(const auto& requiredIU : leftRequiredIUs) {
      out << "auto& " << requiredIU->getVarname()
          << " = std::get<" << tupleIdx << ">(iter->second);";
      tupleIdx++;
    }
    parent->consume(out, *this);
    out << "}"
           "}";
  }
}

IUSet InnerHashJoinOperator::getProduced() const {
  auto produced = left->getProduced();
  auto rightProduced = right->getProduced();
  produced.insert(rightProduced.begin(), rightProduced.end());
  return produced;
}

uint64_t InnerHashJoinOperator::nextVarnameId = 0;
