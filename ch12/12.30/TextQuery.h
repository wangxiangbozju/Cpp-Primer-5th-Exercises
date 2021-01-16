#ifndef TEXTQUERY_H
#define TEXTQUERY_H

class QueryResult;//必须先声明 否则后面query的返回类型不知道QueryResult是什么东西

#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>

class TextQuery {
public:
  typedef std::vector<std::string>::size_type line_no_type;

  explicit TextQuery(std::ifstream &in);

  QueryResult query(const std::string &word) const;

private:
  std::shared_ptr<std::vector<std::string>> text;
  std::map<std::string, std::shared_ptr<std::set<line_no_type>>> word_map;
};

#endif
