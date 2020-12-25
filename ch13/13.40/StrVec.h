#ifndef STRVEC_H
#define STRVEC_H

#include <string>
#include <memory>  // allocator, uninitialized_copy
#include <initializer_list>

class StrVec {
public:
  typedef size_t size_type;  //1.typedef 
  StrVec() : first_elem(nullptr), first_free(nullptr), cap(nullptr) { } //2.默认构造函数
  StrVec(std::initializer_list<std::string>); //3.重构构造函数

  StrVec(const StrVec &);//拷贝构造函数
  ~StrVec();//析构函数
  StrVec &operator=(const StrVec &);//拷贝赋值运算符

  void push_back(const std::string &s);//把一个string压入StrVec的末尾
  void pop_back();//从strVec的末尾弹出string

  void reserve(size_type);
  void resize(size_type, const std::string & = std::string());
  // default value for parameter put in declaration

  bool empty() const { return cbegin() == cend(); }
  size_type size() const { return first_free - first_elem; }
  size_type capacity() const { return cap - first_elem; }
  
 // const std::string * 表明返回值是一个指向常量字符串的指针 不能通过*begin()改变其值 但是返回值可以指向其他内容 begin()++可以的
  std::string *begin() { return first_elem; }
  std::string *end() { return first_free; }
  
  const std::string *begin() const { return first_elem; }  
  const std::string *end() const { return first_free; }
  const std::string *cbegin() const { return begin(); }
  const std::string *cend() const { return end(); }

private:
  static std::allocator<std::string> alloc;

  std::string *first_elem;
  std::string *first_free;
  std::string *cap;

  void chk_n_alloc() { if (size() == capacity()) reallocate(); }
  std::pair<std::string *, std::string *>
  alloc_n_copy(const std::string *, const std::string *);
  void reallocate();
  void free();
};

#endif
