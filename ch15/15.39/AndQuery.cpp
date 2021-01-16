#include "AndQuery.h"
#include <memory>  // make_shared
#include <algorithm>  // set_intersection
#include <iterator>  // inserter
#include "TextQuery.h"
#include "QueryResult.h"

#if DEBUG_LEVEL >= 1
#include <iostream>
#endif

QueryResult AndQuery::eval(const TextQuery &t) const {
  //虚调用（可能是递归调用） 
  auto left = lhs.eval(t), right = rhs.eval(t);
  //ret_lines是指向set的一个指针
  auto ret_lines = std::make_shared<std::set<line_no_type>>();
  // NOTE we cannot use `back_inserter` here, because a `set` does not have a
  // `push_back` member function.
  std::set_intersection(left.cbegin(), left.cend(),
                        right.cbegin(), right.cend(),
                        std::inserter(*ret_lines, ret_lines->begin()));
  return QueryResult(rep(), ret_lines->size(), ret_lines, left.get_file());
}

Query operator&(const Query &lhs, const Query &rhs) {

  // NOTE we cannot use `std::make_shared` here, because the type of the
  // dynamically created object and the type of the object pointed by shared
  // pointer are different.
  return std::shared_ptr<Query_base>(new AndQuery(lhs, rhs));
}
