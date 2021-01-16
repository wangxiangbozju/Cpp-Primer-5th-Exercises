#ifndef QUERY_H
#define QUERY_H

class TextQuery;
class QueryResult;
class Query_base;

#include <memory>
#include <string>
#include <iostream>

//给用户查询的接口 提供接受string的构造函数（public) 和接受Query_base的指针的构造函数(private)
//其内部的数据是静态类型为指向Query_base的指针 在运行时该指针绑定Query_base的不同派生类
//其函数eval 和rep 是通过指向Query_base的指针调用Query_base的不同派生类override的虚函数
class Query {
  //  operator~  operator& operator|都需要使用Query的构造函数 应该申明为友元函数
  friend Query operator~(const Query &);
  friend Query operator&(const Query &, const Query &);
  friend Query operator|(const Query &, const Query &);

public:
  Query(const std::string &);
  QueryResult eval(const TextQuery &t) const;
  std::string rep() const;
private:
  Query(std::shared_ptr<Query_base> query) : pq(query) {
  }

  std::shared_ptr<Query_base> pq;
};

inline std::ostream &operator<<(std::ostream &os, const Query &q) {
  return os << q.rep();
}

#endif
