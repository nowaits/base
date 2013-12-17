#include "base\function_type\type.h"
#include "base\basictypes.h"
#include "base\test\unit_test.h"
#include <assert.h>

namespace function_type {
  //////////////////////////////////////////////////////////////////////////
class TestClass {
public:
  int MethodA() {return 0;}
  void MethodB() {return;}

  static int WeakFun(int, bool, char){return true;}
};

int ReturnFuc(int, bool, char){return 0;}
void VoidFuc(TestClass weak, int default){;}

COMPILE_ASSERT(IS_OBJ_FUN(&TestClass::WeakFun) == false, weak_fun_should_be_a_global_function);
COMPILE_ASSERT(IS_OBJ_FUN(&TestClass::MethodA) == true,  methodA_fun_should_be_a_member_function);

COMPILE_ASSERT(IS_EQUE_TYPE(0.3, float(0.3)) == false, ok);
COMPILE_ASSERT(IS_EQUE_TYPE(&TestClass::WeakFun, ReturnFuc), ok);

COMPILE_ASSERT(IS_RET_TYPE_OK(&TestClass::WeakFun, int(0)), ok);
COMPILE_ASSERT(IS_RET_TYPE_OK(&TestClass::MethodA, int(0)), ok);
COMPILE_ASSERT(!IS_VOID_TYPE(&TestClass::MethodA), this_case_need_a_return_type_function);
COMPILE_ASSERT(REQUEST_DEFAULT_VALUE(&TestClass::MethodA), request_default_value);
COMPILE_ASSERT(!REQUEST_DEFAULT_VALUE(&TestClass::MethodB), request_default_value);

UNIT_TEST(TestClass) {
  assert(NeedDefaultRetValue(ReturnFuc) == true);
  assert(NeedDefaultRetValue(VoidFuc) == false);
}

}