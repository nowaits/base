#include "base\memory\singleton.h"
#include "base\at_exit.h"
#include "base\compiler_specific.h"
#include "base\memory\weak_ptr.h"
#include "base\bind.h"
#include "bind_helpers.h"
#include "base\test\test_all.h"

class SingletonTest {
public:
  SingletonTest() {}
  ~SingletonTest(){}

  static SingletonTest* GetInstance(){
    return Singleton<SingletonTest>::get();
  };
};

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

TEST(SingletonTest) {
  SingletonTest* t = SingletonTest::GetInstance();
}

TEST(POD) {
  //   1.POD数据类型
  //     2.内存对齐
  ALIGNAS(4096) uint8 data;
  assert(((int)&data & (int)(4096 - 1)) == 0);
}

TEST(WeakPtrTest) {
  scoped_ptr<WeakPtrTest> ptr(new WeakPtrTest);

  base::Callback<int()> call_back = 
    base::Bind(&WeakPtrTest::WeakFun, ptr->AsWeakPtr(), 32);
  assert(call_back.Run() != 32);
  ptr.reset();
  assert(call_back.Run() == 32);
}