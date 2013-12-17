#include "base\bind.h"
#include "base\memory\weak_ptr.h"
#include "base\test\unit_test.h"
#include "base\basictypes.h"
#include "base\function_type\type.h"

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

  void WeakCallBackTestB() {
    return;
  }

private:
  base::WeakPtrFactory<WeakPtrTest> weak_ref_;
};

UNIT_TEST(WeakPtrTestModify) {
  scoped_ptr<WeakPtrTest> ptr(new WeakPtrTest);
  base::Callback<int()> call_back = 
    base::Bind(&WeakPtrTest::WeakCallBackTestA, 32, ptr->AsWeakPtr());
  call_back.Run();
}