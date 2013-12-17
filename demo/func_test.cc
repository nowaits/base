#include "base\test\unit_test.h"
#include <string>
#include <assert.h>
#include "base\basictypes.h"
#include "base\function_type\type.h"
#include "base\function_type\func_type.h"

void fun(){*((char*)0) = 0;}
void funa(int){*((char*)0) = 0;}
void funaa(std::string){*((char*)0) = 0;}
void funb(int, int){*((char*)0) = 0;}
void funbb(int, std::string){*((char*)0) = 0;}
void func(int, int, int){*((char*)0) = 0;}
void funcc(std::string, int, std::string){*((char*)0) = 0;}

int fun1(){*((char*)0) = 0;return 0;}
bool fund(int){*((char*)0) = 0;return 0;}
bool fundd(std::string){*((char*)0) = 0;return 0;}
bool fune(int, int){*((char*)0) = 0;return 0;}
bool funee(int, std::string){*((char*)0) = 0;return 0;}
bool funf(int, int, int){*((char*)0) = 0;return 0;}
bool funff(std::string, std::string, int){*((char*)0) = 0;return 0;}

namespace sss {
  class TestClass {
  public:
    static bool& funa(int a, std::string d, int & y){static bool x;return x;};
    bool funb(int a, std::string d, int & y){return 0;};
  };

  void funa(int){}
  bool funb(int){return 0;}
}

UNIT_TEST(FuncTypeDetect) {
  assert(FuncTypeDetect<>::check(fun, true, true, false, 0));
  assert(FuncTypeDetect<>::check(funa, true, true, false, 1));
  assert(FuncTypeDetect<>::check(funaa, true, true, false, 1));
  assert(FuncTypeDetect<>::check(funb, true, true, false, 2));
  assert(FuncTypeDetect<>::check(funbb, true, true, false, 2));
  assert(FuncTypeDetect<>::check(func, true, true, false, 3));
  assert(FuncTypeDetect<>::check(funcc, true, true, false, 3));

  assert(FuncTypeDetect<int>::check(fun1, true, true, false, 0));
  assert(FuncTypeDetect<bool>::check(fund, true, true, false, 1));
  assert(FuncTypeDetect<bool>::check(fundd, true, true, false, 1));
  assert(FuncTypeDetect<bool>::check(fune, true, true, false, 2));
  assert(FuncTypeDetect<bool>::check(funee, true, true, false, 2));
  assert(FuncTypeDetect<bool>::check(funf, true, true, false, 3));
  assert(FuncTypeDetect<bool>::check(funff, true, true, false, 3));

  assert(FuncTypeDetect<>::check(sss::funa, true, true, false, 1));
  assert(FuncTypeDetect<bool>::check(sss::funb, true, true, false, 1));
  assert(FuncTypeDetect<bool&>::check(sss::TestClass::funa, true, true, false, 3));
  assert(FuncTypeDetect<bool>::check(&sss::TestClass::funb, true, true, true, 3));
}

template<typename T> 
void Obj_fun(const T& t) {
  COMPILE_ASSERT(IS_VOID_TYPE(t), you_need_suply_a_default_result);
}

template<typename T1, typename T2> 
void Obj_fun(const T1& t, const T2&) {
}

UNIT_TEST(BIND_FUN_TEST) {
  Obj_fun(fun1, 32);
}