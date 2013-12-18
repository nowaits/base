#include "base\bind.h"
#include "base\memory\weak_ptr.h"
#include "base\test\unit_test.h"
#include "base\basictypes.h"
#include "base\function_type\type.h"
#include "base\bind_helpers.h"
#include <string>

// UNIT_TEST(WeakPtrTest)
class WeakPtrTest {
public:
  WeakPtrTest() : weak_ref_(this){}

  ~WeakPtrTest(){}

  base::WeakPtr<WeakPtrTest> AsWeakPtr() { 
    return weak_ref_.GetWeakPtr();
  }

  void WeakCallBackTestA(int x) {
  }

  const std::string WeakCallBackTestB() {
    static std::string s = "DFDF";
    return s;
  }

  const std::string WeakCallBackTestC(const std::string& x) {
    return x;
  }

private:
  base::WeakPtrFactory<WeakPtrTest> weak_ref_;
};

UNIT_TEST(supply_a_default_ret_value_for_weak_call) {
/*
  1.只有非void函数才可以调用base::DefaultResult，
  2.在weak_ptr对象调用时，且函数为非void时，只能调用base::DefaultResult
*/
  std::string ret;
  const std::string defalut_value = "defalut_value";

  scoped_ptr<WeakPtrTest> ptr(new WeakPtrTest);

#if 0 // comple_error
  base::Bind(base::DefaultResult(&WeakPtrTest::WeakCallBackTestA, 32), base::Unretained(ptr.get()));
  base::Bind(&WeakPtrTest::WeakCallBackTestB, ptr->AsWeakPtr());
  
#else
  base::Bind(base::DefaultResult(&WeakPtrTest::WeakCallBackTestB, defalut_value), base::Unretained(ptr.get()));

  base::Callback<const std::string (const std::string&)> call_back =
    base::Bind(base::DefaultResult(&WeakPtrTest::WeakCallBackTestC, defalut_value), ptr->AsWeakPtr());

  assert(call_back.Run("hello") == "hello");

  ptr.reset();

  assert(call_back.Run("hello") == defalut_value);
#endif

  ptr.reset(new WeakPtrTest);

  base::Callback<const std::string ()> call_back_void =
    base::Bind(base::DefaultResult(&WeakPtrTest::WeakCallBackTestC, defalut_value), ptr->AsWeakPtr(), "hello");

  assert(call_back_void.Run() == "hello");

  ptr.reset();

  assert(call_back.Run("hello") == defalut_value);
}