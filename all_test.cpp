#include "base\memory\singleton.h"
#include "base\at_exit.h"
#include "base\compiler_specific.h"
#include "base\memory\weak_ptr.h"
#include "base\bind.h"
#include "bind_helpers.h"
#include "base\test\unit_test.h"
#include "base\bits.h"
#include "base\float_util.h"
#include "base\lazy_instance.h"
#include "base\tuple.h"
#include <iostream>
#include <string>
#include "base\template_util.h"
#include "base\observer_list.h"
#include "base\memory\aligned_memory.h"
#include "base\observer_list_threadsafe.h"
 
class SingletonTest {
public:
  SingletonTest() {}
  ~SingletonTest(){}

  static SingletonTest* GetInstance(){
    return Singleton<SingletonTest>::get();
  };
};

UNIT_TEST(SingletonTest) {
  SingletonTest* t = SingletonTest::GetInstance();
}

//////////////////////////////////////////////////////////////////////////
class WeakPtrTest {
public:
  WeakPtrTest() : weak_ref_(this){}

  ~WeakPtrTest(){}

  base::WeakPtr<WeakPtrTest> AsWeakPtr() { 
    return weak_ref_.GetWeakPtr();
  }

  int WeakCallBackTestA() {
    return 43;
  }

  static int WeakFun(base::WeakPtr<WeakPtrTest> weak, int default){
    if (weak.get())
      return weak->WeakCallBackTestA();
    else 
      return default;
  }

  void WeakCallBackTestB() {
    return;
  }
private:
  base::WeakPtrFactory<WeakPtrTest> weak_ref_;
};

class PODClass {
public:
  PODClass(): x(100){}
  ~PODClass(){}
private:
  int x;
  char data[79];
};

UNIT_TEST(WeakPtrTest) {
  scoped_ptr<WeakPtrTest> ptr(new WeakPtrTest);

  base::Callback<int()> call_back = 
    base::Bind(&WeakPtrTest::WeakFun, ptr->AsWeakPtr(), 32);
  assert(call_back.Run() != 32);
  ptr.reset();
  assert(call_back.Run() == 32);
}

//////////////////////////////////////////////////////////////////////////
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
UNIT_TEST(Log2Floor) {
  uint32 x = 800;
  int log_2_f = base::bits::Log2Floor(x);
  int log_2_c = base::bits::Log2Ceiling(x);

  uint32 result = 1<<log_2_f;

  assert(result <= x && x <= result<<1);
}

UNIT_TEST(IsFinite) {
  assert(base::IsFinite(0));
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

UNIT_TEST(Tuple) {
  Tuple_Class t;
  DispatchToMethod(&t, &Tuple_Class::SomeMeth, MakeTuple(1, std::string("ddd")));  DispatchToFunction(&Tuple_Class::Method, MakeTuple(1, std::string("ddd")));
}
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

