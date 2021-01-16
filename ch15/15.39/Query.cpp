#include "Query.h"
#include "WordQuery.h"
#include "TextQuery.h"
#include "QueryResult.h"
#include "Query_base.h"

#if DEBUG_LEVEL >= 1
#include <iostream>
#endif

// NOTE The following member functions cannot be inline or they must be put
// into header, because they are used in different translation units.

//inline


Query::Query(const std::string &s) : pq(new WordQuery(s)) {}

//inline

QueryResult Query::eval(const TextQuery &t) const {
  return pq->eval(t);
}
//inline
std::string Query::rep() const {
  return pq->rep();
}
