#include "base\test\unit_test.h"
template<typename T > 
struct Helper {
  typedef char typsse;
  static void fun(T ){}
};

template<typename T>
 void fun(T){Helper<T>::typsse t = 90;};

 template<typename T>
 struct Fuc {
   static int fun(){return 1;}
 };

 template<>
 struct Fuc<void> {
  static int fun(){return 2;}
 };

UNIT_TEST(blank_test) {

  int x = Fuc<void>::fun();
  x = Fuc<char>::fun();
  fun(32);
}