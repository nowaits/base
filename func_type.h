#pragma once

template<typename T, typename R, T* v = 0>
class FunType{
  typedef char InvalidType[1];

  typedef char GFunc0[2];
  typedef char GFunc1[3];
  typedef char GFunc2[4];
  typedef char GFunc3[5];
  typedef char GFunc4[6];
  typedef char GFunc5[7];
  typedef char GFunc6[8];
  typedef char GFunc7[9];

  typedef char MFunc0[20];
  typedef char MFunc1[21];
  typedef char MFunc2[22];
  typedef char MFunc3[23];
  typedef char MFunc4[24];
  typedef char MFunc5[25];
  typedef char MFunc6[26];
  typedef char MFunc7[27];

  static InvalidType& Check(...);

  // global function
  static GFunc0& Check(R(*)());

  template<typename T1>
  static GFunc1& Check(R(*)(T1));

  template<typename T1, typename T2>
  static GFunc2& Check(R(*)(T1, T2));

  template<typename T1, typename T2, typename T3>
  static GFunc3& Check(R(*)(T1, T2, T3));

  template<typename T1, typename T2, typename T3, typename T4>
  static GFunc4& Check(R(*)(T1, T2, T3, T4));

  template<typename T1, typename T2, typename T3, typename T4, typename T5>
  static GFunc5& Check(R(*)(T1, T2, T3, T4, T5));

  template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
  static GFunc6& Check(R(*)(T1, T2, T3, T4, T5, T6));

  template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
  static GFunc7& Check(R(*)(T1, T2, T3, T4, T5, T6, T7));

  // member function
  template<typename OBJ>
  static MFunc0& Check(R(OBJ::*)());

  template<typename OBJ, typename T1>
  static MFunc1& Check(R(OBJ::*)(T1));

  template<typename OBJ, typename T1, typename T2>
  static MFunc2& Check(R(OBJ::*)(T1, T2));

  template<typename OBJ, typename T1, typename T2, typename T3>
  static MFunc3& Check(R(OBJ::*)(T1, T2, T3));

  template<typename OBJ, typename T1, typename T2, typename T3, typename T4>
  static MFunc4& Check(R(OBJ::*)(T1, T2, T3, T4));

  template<typename OBJ, typename T1, typename T2, typename T3, typename T4, typename T5>
  static MFunc5& Check(R(OBJ::*)(T1, T2, T3, T4, T5));

  template<typename OBJ, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
  static MFunc6& Check(R(OBJ::*)(T1, T2, T3, T4, T5, T6));

  template<typename OBJ, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
  static MFunc7& Check(R(OBJ::*)(T1, T2, T3, T4, T5, T6, T7));

private:
  static const int check_type     = sizeof(Check(*v));

public:
  static const int invalid_value  = -1;
  static const bool value         = check_type != sizeof(InvalidType);
  static const bool is_obj_fun    = check_type >= sizeof(MFunc0) && check_type <= sizeof(MFunc7);
  static const int arg_num        = !value ? invalid_value : check_type - (is_obj_fun ? sizeof(MFunc0) : sizeof(GFunc0));
};

template<typename R = void>
struct FuncTypeDetect {

  template<typename T>
  static bool IsVaildType(const T&){
    return FunType<T, R>::arg_num >= 0;
  }

  template<typename T>
  static bool IsGivenRetType(const T&){
    return FunType<T, R>::value;
  }

  template<typename T>
  static bool IsObjFunction(const T&){
    return FunType<T, R>::is_obj_fun;
  }

  template<typename T>
  static int ArgsNum(const T&){
    return FunType<T, R>::arg_num;
  }

  // for test
  static bool default_report_result(bool is_vaild_type, bool is_given_type, bool is_obj_fuc, bool args_num) {
    return is_vaild_type && is_given_type && is_obj_fuc && args_num;
  }

  template<typename T>
  static bool check(const T& t, bool is_vaild_type, bool is_given_type, bool is_obj_fuc, int args_num, bool(*check_result )(bool, bool, bool, bool) = default_report_result) {
    bool is_vaild_type_right  = IsVaildType(t) == is_vaild_type;
    bool is_given_type_right  = IsGivenRetType(t) == is_given_type;
    bool is_obj_fuc_right     = IsObjFunction(t) == is_obj_fuc;
    bool args_num_right       = ArgsNum(t) == args_num;

    return check_result(is_vaild_type_right, is_given_type_right, is_obj_fuc_right, args_num_right);
  }
};