// UNIT_TEST(VNCSigslotTest)
// UNIT_TEST(postfix)
// UNIT_TEST(placement_new)
// UNIT_TEST(POD)
// UNIT_TEST(SingletonTest)
// UNIT_TEST(Log2Floor)
// UNIT_TEST(IsFinite)
// UNIT_TEST(is_class)
// UNIT_TEST(LazyInstance)
// UNIT_TEST(Tuple)
// UNIT_TEST(ObserverListThreadSafe)
// UNIT_TEST(ObserverListTest)
// UNIT_TEST(HashingMRUCache) 
// UNIT_TEST(ThreadLocalPointerTest)
// UNIT_TEST(LinkedList)
#include <sstream>
#include <iostream>

#include "base\memory\singleton.h"
#include "base\at_exit.h"
#include "base\compiler_specific.h"
#include "base\memory\weak_ptr.h"
#include "base\bind.h"
#include "base\bind_helpers.h"
#include "base\test\unit_test.h"
#include "base\bits.h"
#include "base\float_util.h"
#include "base\lazy_instance.h"
#include "base\tuple.h"
#include "base\template_util.h"
#include "base\observer_list.h"
#include "base\memory\aligned_memory.h"
#include "base\observer_list_threadsafe.h"
#include "base\memory\mru_cache.h"
#include "base\atomic_sequence_num.h"
#include "base\threading\thread_local.h"
#include "base\linked_list.h"
#include "base\function_type\func_type.h"
#include "base\function_type\type.h"
#include "base\sigslot.h"
#include <fstream>
//////////////////////////////////////////////////////////////////////////
struct REFERANCE_TYPE {
  REFERANCE_TYPE(int& x) : x_(x){};
  int& x_;
};
UNIT_TEST(referance_type) {

  int x = 9;
  REFERANCE_TYPE t(x);
  x = 32;

  assert(t.x_ == 32);
  t.x_ = 323;
  assert(x == 323);
  assert(&x == &t.x_);
}
//////////////////////////////////////////////////////////////////////////

class _log 
  : public std::stringstream{
public:
  _log(std::fstream& file_steam, bool force_to_file = false)
    : file_steam_(file_steam), force_to_file_(force_to_file) {
    SYSTEMTIME tm;
    ::GetLocalTime(&tm);
    logStream()<<"["<<tm.wHour<<":"<<tm.wMinute<<":"<<tm.wSecond<<"]:<";
  }

  ~_log() {
    logStream()<<std::endl;

    if (force_to_file_ || 
      ::IsDebuggerPresent() == FALSE) 
      file_steam_<<str();
    else
      ::OutputDebugStringA(str().c_str());
  }

  std::iostream& logStream() {
    return *this;
  }

private:
  bool            force_to_file_;
  std::fstream&   file_steam_;
};

std::fstream file_steam("SLOG.log", std::ios::trunc|std::ios::out);

//////////////////////////////////////////////////////////////////////////
#define SVLOG (_log(file_steam, true).logStream()<<__FUNCTION__<<":("<<__LINE__<<")>::")

#define SLOG (_log(file_steam).logStream()<<__FUNCTION__<<":("<<__LINE__<<")>::")
UNIT_TEST(SLOG) {
  SLOG<<"hello";
  SLOG<<"hellso";
}

//////////////////////////////////////////////////////////////////////////
#include "base\third\circularbuffer.h"
#include <stdlib.h>

UNIT_TEST(CircularBuffer) {
  vncjingle::CircularBuffer buffer(100);
  char bu[] = "If you need to make more extensive configuration changes - more than one or two";
  char buf[100] = {0};
  assert(buffer.Length() == 0);

  assert(buffer.Read(buf, 10, false, false) == false);

  buffer.Write(bu, _countof(bu));

  assert(buffer.Read(buf, 10, false, false) == true);
  assert(buffer.AvailableCapacity() == 100 - _countof(bu) + 10);
}
//////////////////////////////////////////////////////////////////////////
const char* g_buildTime = "Built on " __DATE__ " at " __TIME__;

UNIT_TEST(g_buildTime) {
  const char* fun1 = __FUNCDNAME__; 
  const char* fun2 = __FUNCTION__;
  const char* fun3 = __FUNCSIG__;
}
//////////////////////////////////////////////////////////////////////////
class VNCSigslotTest : public sigslot::has_slots<> {
public:
  void Fun(int a, char c) {
  }
};

UNIT_TEST(VNCSigslotTest) {

  scoped_ptr<VNCSigslotTest> p(new VNCSigslotTest);
  sigslot::signal2<int, char> SignalJingleInfo;
  SignalJingleInfo.connect(p.get(), &VNCSigslotTest::Fun);
  SignalJingleInfo.emit(3, 'a');
  SignalJingleInfo(5, 's');
  SignalJingleInfo.disconnect(p.get());

  SignalJingleInfo.emit(3, 'a');
  SignalJingleInfo.emit(4, 'b');
}
//////////////////////////////////////////////////////////////////////////
class Number 
{
public:
  Number& operator++ ()     // prefix ++
  {
    // Do work on this.
    return *this;
  }

  // You want to make the ++ operator work like the standard operators
  // The simple way to do this is to implement postfix in terms of prefix.
  //
  Number  operator++ (int)  // postfix ++
  {
    Number result(*this);   // make a copy for result
    ++(*this);              // Now use the prefix version to do the work
    return result;          // return the copy (the old) value.
  }
}; 

UNIT_TEST(postfix) {
  Number n;
  n++;
}
//////////////////////////////////////////////////////////////////////////
class PODClass {
public:
  PODClass(int x = 100, std::string text = "pod")
    : x_(x), text_(text){}
  ~PODClass(){}
  const std::string& text(){return text_;}
private:
  int x_;
  std::string text_;
};

UNIT_TEST(placement_new) {
  uint8 data[sizeof(PODClass)] = {0};

  PODClass* pod =  new(data) PODClass(20, "data");

  assert(pod->text() == "data");
  pod->PODClass::~PODClass();

  assert(sizeof(*pod) == sizeof(PODClass));
}

UNIT_TEST(POD) {
  //   1.POD数据类型
  //     2.内存对齐
  ALIGNAS(4096) uint8 data;
  assert(((int)&data & (int)(4096 - 1)) == 0);

  static base::AlignedMemory<sizeof(PODClass), ALIGNOF(PODClass)> my_class;

  assert((reinterpret_cast<uintptr_t>(my_class.void_data()) & (ALIGNOF(PODClass) - 1)) ==  0u);
  // ... at runtime:
  new(my_class.void_data()) PODClass;

  // ... use it:
  PODClass* mc = my_class.data_as<PODClass>();

  // ... later, to destruct my_class:
  my_class.data_as<PODClass>()->PODClass::~PODClass();

}
//////////////////////////////////////////////////////////////////////////
class SingletonTest {
public:
  SingletonTest() {}
  ~SingletonTest(){}

  static SingletonTest* GetInstance(){
    return Singleton<SingletonTest>::get();
  };

  int GetNext() {    return member_.GetNext();  }
private:
  base::AtomicSequenceNumber   member_;
};

UNIT_TEST(SingletonTest) {
  SingletonTest* t = SingletonTest::GetInstance();
  assert(t->GetNext() == 0);
  assert(t->GetNext() == 1);
  assert(t->GetNext() == 2);
  assert(t->GetNext() == 3);
}

//////////////////////////////////////////////////////////////////////////
UNIT_TEST(Log2Floor) {
  uint32 x = 800;
  int log_2_f = base::bits::Log2Floor(x);
  int log_2_c = base::bits::Log2Ceiling(x);

  uint32 result = 1<<log_2_f;

  assert(result <= x && x <= result<<1);
}

UNIT_TEST(IsFinite) {
  assert(base::IsFinite(std::numeric_limits<double>::max()));
}

//////////////////////////////////////////////////////////////////////////
struct NULLStruct{
  NULLStruct(){}
  ~NULLStruct(){}
};

UNIT_TEST(is_class) {
  const bool a = base::is_class<NULLStruct>::value;
  const bool b = base::is_class<char>::value;
  assert(a == true);
  assert(b == false);
}

UNIT_TEST(LazyInstance) {

  base::LazyInstance<NULLStruct>::Leaky my_leaky_lazy_instance = LAZY_INSTANCE_INITIALIZER;

  assert(my_leaky_lazy_instance == NULL);
  NULLStruct &null_struct = my_leaky_lazy_instance.Get();

  assert(!(my_leaky_lazy_instance == NULL)); 
}
//////////////////////////////////////////////////////////////////////////
struct Tuple_Class{ 
  void SomeMeth(int a, std::string s) {
    std::cout<<s<<std::endl;
  }
  static void Method(int a, std::string s){
    std::cout<<s<<std::endl;
  }
};
#if 0

template<typename F, typename TUPLE, typename OBJ>
void DisplatchMethod(F fun, TUPLE tuple, OBJ obj = NULL) {
  COMPILE_ASSERT(IS_OBJ_FUN(fun) == true, sdfsfd);
  if (IS_OBJ_FUN(fun)){
    if (obj == NULL) {return;}
    DispatchToMethod(obj, fun, tuple);
  }
  else {
    DispatchToFunction(fun, tuple);
  }
}

UNIT_TEST(Tuple) {
  Tuple_Class t;
  int x = 1;
  
  DisplatchMethod(&Tuple_Class::SomeMeth, MakeTuple(x, std::string("ddd")), &t);
}
#else

UNIT_TEST(Tuple) {
  Tuple_Class t;
  int x = 1;
  if (FuncTypeDetect<>::IsObjFunction(&Tuple_Class::SomeMeth))
    DispatchToMethod(&t, &Tuple_Class::SomeMeth, MakeTuple(1, std::string("ddd")));
  else
  DispatchToFunction(&Tuple_Class::Method, MakeTuple(1, std::string("ddd")));
}
#endif
//////////////////////////////////////////////////////////////////////////
class ObserverListTest{
public:
  class Observer {
  public:
    virtual void OnFoo(const std::string& s) = 0;
  };
  void AddObserver(Observer* obs) {
    observer_list_.AddObserver(obs);
  }
  void RemoveObserver(Observer* obs) {
    observer_list_.RemoveObserver(obs);
  }
  void Notify(const std::string& s) {
    FOR_EACH_OBSERVER(Observer, observer_list_, OnFoo(s));
  }
private:
  ObserverList<Observer> observer_list_;
};

class ObserverTest 
  : public ObserverListTest::Observer {
public:
  virtual void OnFoo(const std::string& s) override {
    std::cout<<s<<std::endl;
  }
};

UNIT_TEST(ObserverListThreadSafe) {
  ObserverListThreadSafe<ObserverTest>* s = new ObserverListThreadSafe<ObserverTest>;
  scoped_ptr<ObserverTest> test(new ObserverTest);
  s->AddObserver(test.get());
  s->Notify(&ObserverListTest::Observer::OnFoo, std::string("ObserverListThreadSafe"));
  int x = 200;
}

UNIT_TEST(ObserverListTest) {
  scoped_ptr<ObserverListTest> observer(new ObserverListTest);
  scoped_ptr<ObserverTest> test(new ObserverTest);
  observer->AddObserver(test.get());
  observer->Notify("ObserverListTest");
  observer->RemoveObserver(test.get());
}
//////////////////////////////////////////////////////////////////////////
UNIT_TEST(HashingMRUCache) {
  size_t max_size = 10;
  base::MRUCache<int, char> mru_cache(max_size);
  int key = 0;
  char playload = 'a';

  for(int i = 0; i < max_size; i ++) {
    mru_cache.Put(key + i, playload + i);
  }

  assert(mru_cache.begin()->first == 9  && mru_cache.begin()->second == 'a' + 9);
  assert(mru_cache.Get(9)->first == 9   && mru_cache.Get(9)->second == 'a' + 9); 
  assert(mru_cache.begin()->first == 9  && mru_cache.begin()->second == 'a' + 9);
}
//////////////////////////////////////////////////////////////////////////
template <typename T>
class ThreadLocalPointerTest : public base::ThreadLocalPointer<T> {
public:
  static ThreadLocalPointerTest<T>* GetInstance(){
    return Singleton<ThreadLocalPointerTest<T>>::get();
  }
};
class TestClass {
public:
  TestClass::TestClass() {
    ThreadLocalPointerTest<TestClass>::GetInstance()->Set(this);
  }
  TestClass::~TestClass() {
    ThreadLocalPointerTest<TestClass>::GetInstance()->Set(NULL);
  }
  static TestClass* TestClass::current() {
    return ThreadLocalPointerTest<TestClass>::GetInstance()->Get();
  }
};

UNIT_TEST(ThreadLocalPointerTest) {
  assert(TestClass::current() == NULL);
  {
    scoped_ptr<TestClass> t(new TestClass);
    assert(TestClass::current() == t.get());
  }
  assert(TestClass::current() == NULL);
}

//////////////////////////////////////////////////////////////////////////
class LinkNodeTest
  : public base::LinkNode<LinkNodeTest> {
public:
  LinkNodeTest(const std::string& text): text_(text) {}
  ~LinkNodeTest(){RemoveFromList();}
  const std::string& text(){return text_;}
  std::string& text()const {static std::string s;return s;}
private:
  std::string text_;
};

UNIT_TEST(LinkedList) {

  base::LinkedList<LinkNodeTest> list;
  { 
    scoped_ptr<LinkNodeTest> n1(new LinkNodeTest("aaa"));
    scoped_ptr<LinkNodeTest> n2(new LinkNodeTest("bbb"));
    scoped_ptr<LinkNodeTest> n3(new LinkNodeTest("ccc"));

    list.Append(n1.get());
    list.Append(n3.get());
    n2->InsertBefore(n3.get());

    for (base::LinkNode<LinkNodeTest>* node = list.head();
      node != list.end();
      node = node->next()) {
        std::cout<<node->value()->text()<<std::endl;
    }

    for (base::LinkNode<LinkNodeTest>* node = list.tail();
      node != list.end();
      node = node->previous()) {
        std::cout<<node->value()->text()<<std::endl;
    }
  }

  assert(list.head()->value() == 0);
  assert(list.head() == 0 && list.tail() == 0);
}