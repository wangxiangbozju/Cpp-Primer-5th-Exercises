#include <string>
#include <set>
#include <iostream>

/** Message.h **/

class Message {
  friend class Folder;
  friend void swap(Message &, Message &);
  friend void swap(Folder &, Folder &);
public:
  explicit Message(const std::string &s = "") : contents(s), folders() {}

  Message(const Message &);
  Message &operator=(const Message &);
  ~Message();

  void save(Folder &);
  void remove(Folder &);

  void print();

private:
  std::string contents;
  std::set<Folder *> folders;

  void add_to_folders(const Message &); //把只读的参数 加入到当前message的folder中
  void remove_from_folders(); //this的所有folder中移除this message

  void addFdr(Folder *);
  void remFdr(Folder *);
};

void swap(Message &, Message &); //因为swap Message的时候我们需要对它们所对应的folder也进行改变 所以需要定义自己的swap 先断开Message所对应的folder对于message 的连接
//然后交换message 然后建立folder对message的新连接

/** Folder.h **/

class Folder {
  friend class Message;
  friend void swap(Message &, Message &);
  friend void swap(Folder &, Folder &);
public:
  Folder() = default;

  Folder(const Folder &);
  Folder &operator=(const Folder &);
  ~Folder();

  void save(Message &);
  void remove(Message &);

  void print();

private:
  std::set<Message *> messages;

  void addMsg(Message *);
  void remMsg(Message *);
};

void swap(Folder &, Folder &);

/** Message.cpp **/

void swap(Message &lhs, Message &rhs) {
  using std::swap; 
  //断开lhs和rhs对应的folder连接message的单向连接
  for (auto &fp : lhs.folders)
    fp->remMsg(&lhs);
  for (auto &fp : rhs.folders)
    fp->remMsg(&rhs);
  //交换message
  swap(lhs.contents, rhs.contents);
  swap(lhs.folders, rhs.folders);
  //建立folder和message的新连接
  for (auto &fp : lhs.folders)
    fp->addMsg(&lhs);
  for (auto &fp : rhs.folders)
    fp->addMsg(&rhs);
}

void Message::addFdr(Folder *fp) {
  //在this message的set中插入一个fp指针
  folders.insert(fp);
}

void Message::remFdr(Folder *fp) {
  //在this message的set中移除一个fp指针
  folders.erase(fp);
}
  
//建立this message和参数--一个Folder的对象f的双向链接
void Message::save(Folder &f) {

  //先在this message的folders set中插入这个Folder的对象f
  folders.insert(&f);//&是取地址符 传入的是Folder对象的指针
  //再在这个这个Folder的对象f中加入this message
  f.addMsg(this);
}

//移除this message和参数--一个Folder的对象f的双向链接
void Message::remove(Folder &f) {
   //先在this message的folders set中删除这个Folder的对象f
  folders.erase(&f);
   //再在这个这个Folder的对象f中删除this message
  f.remMsg(this);
}

//对一个右值Message对象的folder set中的每一个folder添加this message
void Message::add_to_folders(const Message &m) {  
  // This function can be without parameter.
  for (auto &fp : m.folders) //fp是Folder* 可以用类指针的取成员运算符 ->
    fp->addMsg(this);
}

//在this message的folder set中把所有folder指向this message的单向链接取消
void Message::remove_from_folders() {  
  for (auto &fp : folders)
    fp->remMsg(this);
  folders.clear(); // This statement is not needed.
}

//拷贝构造函数 不仅需要复制m的数据 还需要在m的folder set中的每一个Folder添加指向this message的单向链接
//和拷贝赋值运算符比较 少了remove_from_folders（） 因为它是一个构造函数 左值为空还没有对应到Folder
Message::Message(const Message &m) : contents(m.contents), folders(m.folders) {
  add_to_folders(m);
}

//拷贝赋值运算符 
Message &Message::operator=(const Message &rhs) {
  remove_from_folders();   //在左值的Message对应的所有Folder中移除this message
  contents = rhs.contents;  //this-> contents = rhs.contents;
  folders = rhs.folders;  //this->folders = rhs.folders;
  add_to_folders(rhs);     //this->add_to_folders(rhs);  rhs的Folder set的每一个Folder中加入this message
  return *this;
}

//析构时 this message的所有Folder set中移除它
Message::~Message() {
  remove_from_folders();
}

void Message::print() {
  std::cout << "Message <" << this << ">:<" << contents << "> in "
            << folders.size() << " folders:\n";
  for (const auto &fp : folders)
    std::cout << "\t<" << fp << ">\n";
}

/** Folder.cpp **/

void swap(Folder &lhs, Folder &rhs){
  using std::swap;
  //先让lhs的Message* set中的每个message断开和folder lhs的连接 
  for (auto &mp : lhs.messages)
    mp->remFdr(&lhs); //取地址符号 
  for (auto &mp : rhs.messages)
    mp->remFdr(&rhs);
  swap(lhs.messages, rhs.messages);
   // lhs的Message* set中的每个message建立和folder lhs的连接 
  for (auto &mp : lhs.messages)
    mp->addFdr(&lhs);
  for (auto &mp : rhs.messages)
    mp->addFdr(&rhs);
}

//在this folder 的message set中插入mp
void Folder::addMsg(Message *mp) {
  messages.insert(mp);
}
//在this folder 的message set中删除mp
void Folder::remMsg(Message *mp) {
  messages.erase(mp);
}

//this folder中存入Message m
void Folder::save(Message &m) {
  //先this folder的message set中插入m
  messages.insert(&m);  //取地址符
  //再Message m中插入this folder
  m.addFdr(this); 
}
//this folder中删除Message m
void Folder::remove(Message &m) {
  //先this folder的message set中删除m (注意传入的是指针）
  messages.erase(&m);
  //再Message m中删除this folder
  m.remFdr(this);
}

//拷贝构造函数，先拷贝右边Folder的message set
//然后 拷贝来的 的message set中的每个message 需要链接到this folder
Folder::Folder(const Folder &f) : messages(f.messages) {
  for (auto &mp : messages)
    mp->addFdr(this);
}


//拷贝赋值运算符 
//和拷贝构造函数相比 只多了this folder(也是是赋值时候的左值）存储的message set(messages)中的每个message要先断开和this folder的链接
Folder & Folder::operator= (const Folder &rhs) {
   //左值的folder的message set(messages)的每个message要先断开和this folder的链接
  for (auto &mp : messages)
    mp->remFdr(this);
  messages = rhs.messages;
  for (auto &mp : messages)
    mp->addFdr(this);
  return *this;
}

//析构函数 this folder存储的message set(messages)的每个message要先断开和this folder的链接
Folder::~Folder() {
  for (auto &mp : messages)
    mp->remFdr(this);
}

void Folder::print() {
  std::cout << "Folder <" << this << "> contains " << messages.size()
            << " messages:\n";
  for (const auto &mp : messages)
    std::cout << "\t<" << mp << ">:<" << mp->contents << ">\n";
}

/** Test.cpp **/

int main() {
  Folder f1, f2;
  Message m1("m1"), m2("m2"), m3("m3"), m4("m4"), m5("m5");

  m1.save(f1); m2.save(f1); m5.save(f1);
  m3.save(f2); m4.save(f2); m5.save(f2);
  std::cout << "\n===== Message save =====\n";
  f1.print(); f2.print(); std::cout << std::endl;
  m1.print(); m2.print(); m3.print(); m4.print(); m5.print();

  {
    Message m6(m5);
    std::cout << "\n===== Message copy-constructor =====\n";
    f1.print(); f2.print(); std::cout << std::endl;
    m5.print(); m6.print();

    m6 = m1;
    std::cout << "\n===== Message copy-assignment operator =====\n";
    f1.print(); f2.print(); std::cout << std::endl;
    m5.print(); m6.print();
  }
  std::cout << "\n===== Message destructor =====\n";
  f1.print(); f2.print(); std::cout << std::endl;
  m5.print();

  swap(m1, m3);
  std::cout << "\n===== Message swap =====\n";
  f1.print(); f2.print(); std::cout << std::endl;
  m1.print(); m3.print();
  swap(m1, m3);  // swap back

  swap(m1, m1);
  std::cout << "\n===== Message swap self =====\n";
  f1.print(); f2.print(); std::cout << std::endl;
  m1.print();

  m1.remove(f1); m2.remove(f1); m5.remove(f1);
  m3.remove(f2); m4.remove(f2); m5.remove(f2);
  std::cout << "\n===== Message remove =====\n";
  f1.print(); f2.print(); std::cout << std::endl;
  m1.print(); m2.print(); m3.print(); m4.print(); m5.print();

  f1.save(m1); f1.save(m2); f1.save(m5);
  f2.save(m3); f2.save(m4); f2.save(m5);
  std::cout << "\n===== Folder save =====\n";
  f1.print(); f2.print(); std::cout << std::endl;
  m1.print(); m2.print(); m3.print(); m4.print(); m5.print();

  {
    Folder f3(f1);
    std::cout << "\n===== Folder copy-constructor =====\n";
    f1.print(); f2.print(); f3.print(); std::cout << std::endl;
    m1.print(); m2.print(); m3.print(); m4.print(); m5.print();

    f3 = f2;
    std::cout << "\n===== Folder copy-assignment operator =====\n";
    f1.print(); f2.print(); f3.print(); std::cout << std::endl;
    m1.print(); m2.print(); m3.print(); m4.print(); m5.print();
  }
  std::cout << "\n===== Folder destructor =====\n";
  f1.print(); f2.print(); std::cout << std::endl;
  m1.print(); m2.print(); m3.print(); m4.print(); m5.print();

  swap(f1, f2);
  std::cout << "\n===== Folder swap =====\n";
  f1.print(); f2.print(); std::cout << std::endl;
  m1.print(); m2.print(); m3.print(); m4.print(); m5.print();
  swap(f1, f2);  // swap back

  swap(f1, f1);
  std::cout << "\n===== Folder swap self =====\n";
  f1.print(); f2.print(); std::cout << std::endl;
  m1.print(); m2.print(); m3.print(); m4.print(); m5.print();

  f1.remove(m1); f1.remove(m2); f1.remove(m5);
  f2.remove(m3); f2.remove(m4); f2.remove(m5);
  std::cout << "\n===== Folder remove =====\n";
  f1.print(); f2.print(); std::cout << std::endl;
  m1.print(); m2.print(); m3.print(); m4.print(); m5.print();

  return 0;
}
