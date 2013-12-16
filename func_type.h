#pragma once

template<typename T, typename R, T* v = 0>
class FunType{
  typedef char Type1[1];
  typedef char Type2[2];
  typedef char Type3[3];
  typedef char Type4[4];
  typedef char Type5[5];
  typedef char Type6[6];
  typedef char Type7[7];
  typedef char Type8[8];
  typedef char Type9[9];

  static Type1& Check(...);

  static Type2& Check(R(*)());

  template<typename T1>
  static Type3& Check(R(*)(T1));

  template<typename T1, typename T2>
  static Type4& Check(R(*)(T1, T2));

  template<typename T1, typename T2, typename T3>
  static Type5& Check(R(*)(T1, T2, T3));

  template<typename T1, typename T2, typename T3, typename T4>
  static Type6& Check(R(*)(T1, T2, T3, T4));

  template<typename T1, typename T2, typename T3, typename T4, typename T5>
  static Type7& Check(R(*)(T1, T2, T3, T4, T5));

  template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
  static Type8& Check(R(*)(T1, T2, T3, T4, T5, T6));

  template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
  static Type9& Check(R(*)(T1, T2, T3, T4, T5, T6, T7));

public:
  static const bool value = sizeof(Check(*v)) != sizeof(Type1);
  static const int arg_num = sizeof(Check(*v)) - 2;
};

template<typename T>
bool IsVoidFun(const T&){
  return FunType<T, void>::value;
}

template<typename T>
int FunArgNum(const T&){
  return FunType<T, void>::arg_num;
}

template<typename T>
bool IsIntFun(const T&){
  return FunType<T, int>::value;
}

template<typename R = void>
struct FuncTypeDect {

  template<typename T>
  static bool IsVoidFun(const T&){
    return FunType<T, R>::value;
  }

  template<typename T>
  static int FunArgNum(const T&){
    return FunType<T, R>::arg_num;
  }

  template<typename T>
  static bool IsFunction(const T&){
    return FunType<T, R>::arg_num >= 0;
  }
};