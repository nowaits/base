#include "base\bind.h"
#include "base\memory\weak_ptr.h"
#include "base\test\unit_test.h"
#include "base\basictypes.h"
#include "base\function_type\type.h"
#include "base\bind_helpers.h"

// UNIT_TEST(WeakPtrTest)
class WeakPtrTest {
public:
  WeakPtrTest() : weak_ref_(this){}

  ~WeakPtrTest(){}

  base::WeakPtr<WeakPtrTest> AsWeakPtr() { 
    return weak_ref_.GetWeakPtr();
  }

  int WeakCallBackTestA(int x) {
    return x;
  }

private:
  base::WeakPtrFactory<WeakPtrTest> weak_ref_;
};

UNIT_TEST(WeakPtrTestModify) {

  scoped_ptr<WeakPtrTest> ptr(new WeakPtrTest);

  int defalut_value = 200;

  base::Callback<int(int)> call_back
      =
      base::Bind(base::DefaultResult(&WeakPtrTest::WeakCallBackTestA, defalut_value), ptr->AsWeakPtr());


    assert(call_back.Run(32) == ptr->WeakCallBackTestA(32));

    ptr.reset();

    assert(call_back.Run(32) == defalut_value);
}