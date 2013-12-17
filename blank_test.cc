#include "base\test\unit_test.h"
template<typename T > 
struct Helper {
  typedef char typsse;
  static void fun(T ){}
};

template<typename T>
 void fun(T){Helper<T>::typsse t = 90;};

UNIT_TEST(blank_test) {
  fun(32);
}