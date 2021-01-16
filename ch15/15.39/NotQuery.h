#ifndef NOTQUERY_H
#define NOTQUERY_H


class TextQuery;
class QueryResult;

#include <string>
#include <memory>
#include "Query.h"
#include "Query_base.h"

//构造函数接受const Query &  因为Query本身可能是复合类型  ~（Query(a) & Query(b))
//override了两个继承于Query_base的虚函数

class NotQuery : public Query_base {
  // operator~需要使用NotQuery的构造函数 应该是友元函数
  friend Query operator~(const Query &);

  NotQuery(const Query &query) : q(query) {
  }
  QueryResult eval(const TextQuery &) const override;
  std::string rep() const override {
    return "~(" + q.rep() + ")";
  }
  Query q;
};

// NOTE The `operator~` should not be `inline` here and should be put into
// seperate implementation file. Otherwise, we must include this header in all
// files where we want to use this operator. And since this header is not part
// of the interface, its a bad idea to have the user include this header in
// user code.
//inline Query operator~(const Query &q) {
//  return std::shared_ptr<Query_base>(new NotQuery(q));
//}
Query operator~(const Query &);

#endif
