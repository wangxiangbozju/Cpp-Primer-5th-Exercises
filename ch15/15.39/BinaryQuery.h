#ifndef BINARYQUERY_H
#define BINARYQUERY_H

#include <string>
#include "Query_base.h"
#include "Query.h"

class BinaryQuery : public Query_base {
  //protected派生类可以用 private派生类也不能用
protected:
  BinaryQuery(const Query &l, const Query &r, const std::string &o)
     : lhs(l), rhs(r), op(o) {
  }

  std::string rep() const override {
    return "(" + lhs.rep() + " " + op + " " + rhs.rep() + ")";
  }

  Query lhs, rhs;
  std::string op;
};

#endif
