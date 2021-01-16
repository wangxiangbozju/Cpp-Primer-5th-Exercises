#include <sstream>
#include "TextQuery.h" //因为include .h文件了 所以.h文件中include的string vector 等已经被加入进来
#include "QueryResult.h"

TextQuery::TextQuery(std::ifstream &in)
    //: text(std::make_shared<std::vector<std::string>>()), word_map() {
    : text(new std::vector<std::string>), word_map() {
  //从ifstream流中通过getline得到每一行 并且存入数组中
  for (std::string line; std::getline(in, line); text->push_back(line)) {  //, ++ln
    std::istringstream iss(line);
    //对每一行的每个单词 在其对应的set中插入行号
    for (std::string word; iss >> word; ) { 
      auto &lns = word_map[word];//要改变指针lns的指向（reset) 所以需要引用
      if (!lns)
        lns.reset(new std::set<line_no_type>);
      //这样操作后指针一定非空
      lns->insert(text->size());
    }
  }
}

//QueryResult 的参数为（待查单词，出现总次数，指向出现的行的set的指针，查找的全文本的指针）
//不能简单用set::set()得到出现次数 因为一行可能出现多次 只能出现的行再分解比对
QueryResult TextQuery::query(const std::string &word) const {
  //多次调用静态局部变量只初始化一次 nodata指向空set的指针
  static std::shared_ptr<std::set<line_no_type>>
      nodata(new std::set<line_no_type>);
  auto it = word_map.find(word);
    
  //找不到
  if (it == word_map.end())
    //return QueryResult(word);
    return QueryResult(word, 0, nodata, text);
    
  //找到 
  //it是指向了一个键值对 
  line_no_type total = 0;
  for (const auto &ln : *(it->second)) {  //*(it->second)是出现的行号set
    std::istringstream iss((*text)[ln]);
    for (std::string wd; iss >> wd; )
      if (wd == word)
        ++total;
  }
  return QueryResult(word, total, it->second, text);
}
