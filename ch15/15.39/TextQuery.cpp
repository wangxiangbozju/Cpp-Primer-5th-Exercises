#include <sstream>
#include <cctype>  // ispunct, tolower
#include "TextQuery.h"
#include "QueryResult.h"

//过滤文本中标点符号 并且全部小写
std::string TextQuery::filter_str(const std::string &s) const {
  std::string ret;
  for (const auto &ch : s)
    if(!std::ispunct(ch))
      ret += std::tolower(ch);
  return ret;
}

TextQuery::TextQuery(std::ifstream &in)
    : text(new StrVec), word_map() {
  //每次从ifstream中读入一行
  for (std::string line; std::getline(in, line); text->push_back(line)) {
    std::istringstream iss(line);
    //一行中读入各个单词 过滤后加入
    for (std::string word; iss >> word; ) {
      auto &lns = word_map[filter_str(word)];
      if (!lns)
        lns.reset(new std::set<line_no_type>);
      lns->insert(text->size());
    }
  }
}

QueryResult TextQuery::query(const std::string &word) const {
  static std::shared_ptr<std::set<line_no_type>>
      nodata(new std::set<line_no_type>);
  auto filter_word = filter_str(word);
  auto it = word_map.find(filter_word);
  if (it == word_map.end())
    return QueryResult(word, 0, nodata, text);
  line_no_type total = 0;
  for (const auto &ln : *(it->second)) {
    std::istringstream iss(*(text->begin() + ln));
    for (std::string wd; iss >> wd; )
      if (filter_str(wd) == filter_word)
        ++total;
  }
  return QueryResult(word, total, it->second, text);
}
