#include "base\bind.h"
#include "base\memory\weak_ptr.h"
#include "base\test\unit_test.h"
#include "base\basictypes.h"
#include "base\func_type_v.h"

// UNIT_TEST(WeakPtrTest)
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

UNIT_TEST(WeakPtrTest) {
  scoped_ptr<WeakPtrTest> ptr(new WeakPtrTest);

//   base::Callback<int()> call_back = 
//     base::Bind(&WeakPtrTest::WeakFun, ptr->AsWeakPtr(), 32);
//   assert(call_back.Run() != 32);
//   ptr.reset();
//   assert(call_back.Run() == 32);
}

UNIT_TEST(WeakPtrTestModify) {

 // scoped_ptr<WeakPtrTest> ptr(new WeakPtrTest);
  COMPILE_ASSERT(!IS_VOID_TYPE(&WeakPtrTest::WeakCallBackTestA),
    this_case_need_a_return_type_function);

  scoped_ptr<WeakPtrTest> ptr(new WeakPtrTest);

  base::Callback<int()> call_back = 

    base::Bind(&WeakPtrTest::WeakCallBackTestA, 32, ptr->AsWeakPtr());

  call_back.Run();
}