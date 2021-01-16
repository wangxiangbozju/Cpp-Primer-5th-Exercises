#ifndef WORDQUERY_H
#define WORDQUERY_H

#include <string>
#include "TextQuery.h"
#include "QueryResult.h"
#include "Query_base.h"

//WordQuery ：继承抽象的Query_base   override其两个虚函数 
class WordQuery : public Query_base {
  friend class Query;
  WordQuery(const std::string &s) : query_word(s) {}
  QueryResult eval(const TextQuery& t) const override {
    return t.query(query_word);
  }
  std::string rep() const override {
    return query_word;
  }
  std::string query_word;
};

#endif
