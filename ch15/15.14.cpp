#include <string>
#include <iostream>
using std::string;
using std::ostream;

class base {
public:
  string name() { return basename; }
  virtual void print(ostream &os) { os << basename; }
private:
  string basename;
};

class derived : public base {
public:
  void print(ostream &os) { base::print(os); os << " " << i; }
private:
  int i = 0;
};

int main() {
  base bobj; base *bp1 = &bobj; base &br1 = bobj;
  derived dobj; base *bp2 = &dobj; base &br2 = dobj;
  
  // (a) base::print(); 
  bobj.print(std::cout);
  // (b) derived::print(); 不是指针或者引用
  dobj.print(std::cout);
  
  //name不是虚函数 都是从基类来的
  // (c) base::name();
  bp1->name();
  // (d) base::name();
  bp2->name();
  
  //br1 br2是指针或者引用 通过指针或引用调用虚函数print动态绑定
  // (e) base::print();
  br1.print(std::cout);
  // (f) derived::print();
  br2.print(std::cout);

  return 0;
}
