#include "base\func_type.h"
#include "base\test\unit_test.h"
#include <string>
#include <assert.h>

class TestClass {
public:
  static void funa(){};
  void funb(){};
};

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

typedef char Type1[1];
typedef char Type2[2];

Type1& Check(...);
Type2& Check(void(*)());

UNIT_TEST(funtype) {

  const bool a = IsVoidFun(fun);
  const int  aa = FunArgNum(fun);
  const bool b = IsVoidFun(funb);
  const int  bb = FunArgNum(funb);
  const bool c = IsVoidFun(funcc);
  const int  cc = FunArgNum(funcc);

  const int cdd = IsVoidFun(fun);
  const int d = IsVoidFun(fun1);

  const int e = sizeof(Check(fun));
  const int f = sizeof(Check(fun1));

  const bool g = FuncTypeDect<>::IsVoidFun(fun);
  const bool h = FuncTypeDect<int>::IsVoidFun(funff);
  const bool j = FuncTypeDect<bool>::IsVoidFun(funff);

  assert(FuncTypeDect<>::IsVoidFun(fun) == true && FuncTypeDect<>::FunArgNum(fun) == 0);
  assert(FuncTypeDect<>::IsFunction(a) == false);
  assert(FuncTypeDect<int>::IsVoidFun(funff) == false);
  assert(FuncTypeDect<bool>::IsVoidFun(funff) == true && FuncTypeDect<bool>::FunArgNum(funff) == 3);
}