#ifndef QUERYRESULT_H
#define QUERYRESULT_H

#include <iostream>
#include <memory>
#include <set>
#include <vector>
#include <string>

class QueryResult {
  friend std::ostream &print(std::ostream &os, const QueryResult &qr);
public:
  typedef std::vector<std::string>::size_type line_no_type;

  //explicit QueryResult(const std::string &wd)
  //    : word(wd), total(0), line_numbers(), line_text() {}
  QueryResult(const std::string &wd,
              line_no_type t,
              std::shared_ptr<std::set<line_no_type>> lns,
              std::shared_ptr<std::vector<std::string>> lt)
      : word(wd), total(t), line_numbers(lns), line_text(lt) {}

private:
  std::string word;//查什么单词
  line_no_type total;//出现总次数
  std::shared_ptr<std::set<line_no_type>> line_numbers;//各出现在多少行的set （指针） 
  std::shared_ptr<std::vector<std::string>> line_text;//出现行的这些行的文本vector (指针）
};

std::ostream &print(std::ostream &os, const QueryResult &qr); //友元函数声明和函数声明都需要

#endif
