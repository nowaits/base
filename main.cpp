#include "base\memory\singleton.h"
#include "base\at_exit.h"
#include "base\compiler_specific.h"
#include "base\memory\weak_ptr.h"
#include "base\bind.h"
#include "bind_helpers.h"

class SingletonTest {
public:
  SingletonTest() {}
  ~SingletonTest(){}

  static SingletonTest* GetInstance(){
    return Singleton<SingletonTest>::get();
  };
};

/*
          1.POD数据类型
          2.内存对齐
          ALIGNAS(4096) uint8 data;
          assert(((int)&data & (int)(4096 - 1)) == 0);
*/

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

int main() {
  base::AtExitManager g_exit;
  SingletonTest* t = SingletonTest::GetInstance();
  WeakPtrTest* ptr = new WeakPtrTest;

  base::Callback<int()> call_back = 
  base::Bind(&WeakPtrTest::WeakFun, ptr->AsWeakPtr(), 32);
  assert(call_back.Run() != 32);
  delete ptr;
  assert(call_back.Run() == 32);
  return 0;
}