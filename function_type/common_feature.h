#pragma once
namespace function_type {

typedef char InvalidType[1];

//sizeof(type)
typedef char GFunc0[2];
typedef char GFunc1[3];
typedef char GFunc2[4];
typedef char GFunc3[5];
typedef char GFunc4[6];
typedef char GFunc5[7];
typedef char GFunc6[8];
typedef char GFunc7[9];

typedef char GFunc00[22];
typedef char GFunc01[23];
typedef char GFunc02[24];
typedef char GFunc03[25];
typedef char GFunc04[26];
typedef char GFunc05[27];
typedef char GFunc06[28];
typedef char GFunc07[29];

typedef char MFunc0[40];
typedef char MFunc1[41];
typedef char MFunc2[42];
typedef char MFunc3[43];
typedef char MFunc4[44];
typedef char MFunc5[45];
typedef char MFunc6[46];
typedef char MFunc7[47];

typedef char MFunc00[60];
typedef char MFunc01[61];
typedef char MFunc02[62];
typedef char MFunc03[63];
typedef char MFunc04[64];
typedef char MFunc05[65];
typedef char MFunc06[66];
typedef char MFunc07[67];

static InvalidType& Check(...);

// global function
template<typename R>
static GFunc0& Check(R(*)());

template<typename R, typename T1>
static GFunc1& Check(R(*)(T1));

template<typename R, typename T1, typename T2>
static GFunc2& Check(R(*)(T1, T2));

template<typename R, typename T1, typename T2, typename T3>
static GFunc3& Check(R(*)(T1, T2, T3));

template<typename R, typename T1, typename T2, typename T3, typename T4>
static GFunc4& Check(R(*)(T1, T2, T3, T4));

template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
static GFunc5& Check(R(*)(T1, T2, T3, T4, T5));

template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
static GFunc6& Check(R(*)(T1, T2, T3, T4, T5, T6));

template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
static GFunc7& Check(R(*)(T1, T2, T3, T4, T5, T6, T7));

// global function
static GFunc00& Check(void(*)());

template<typename T1>
static GFunc01& Check(void(*)(T1));

template<typename T1, typename T2>
static GFunc02& Check(void(*)(T1, T2));

template<typename R, typename T1, typename T2, typename T3>
static GFunc03& Check(void(*)(T1, T2, T3));

template<typename R, typename T1, typename T2, typename T3, typename T4>
static GFunc04& Check(void(*)(T1, T2, T3, T4));

template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
static GFunc05& Check(void(*)(T1, T2, T3, T4, T5));

template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
static GFunc06& Check(void(*)(T1, T2, T3, T4, T5, T6));

template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
static GFunc07& Check(void(*)(T1, T2, T3, T4, T5, T6, T7));

// member function
template<typename OBJ, typename R>
static MFunc0& Check(R(OBJ::*)());

template<typename OBJ, typename R, typename T1>
static MFunc1& Check(R(OBJ::*)(T1));

template<typename OBJ, typename R, typename T1, typename T2>
static MFunc2& Check(R(OBJ::*)(T1, T2));

template<typename OBJ, typename R, typename T1, typename T2, typename T3>
static MFunc3& Check(R(OBJ::*)(T1, T2, T3));

template<typename OBJ, typename R, typename T1, typename T2, typename T3, typename T4>
static MFunc4& Check(R(OBJ::*)(T1, T2, T3, T4));

template<typename OBJ, typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
static MFunc5& Check(R(OBJ::*)(T1, T2, T3, T4, T5));

template<typename OBJ, typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
static MFunc6& Check(R(OBJ::*)(T1, T2, T3, T4, T5, T6));

template<typename OBJ, typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
static MFunc7& Check(R(OBJ::*)(T1, T2, T3, T4, T5, T6, T7));

// member function
template<typename OBJ>
static MFunc00& Check(void(OBJ::*)());

template<typename OBJ, typename T1>
static MFunc01& Check(void(OBJ::*)(T1));

template<typename OBJ, typename T1, typename T2>
static MFunc02& Check(void(OBJ::*)(T1, T2));

template<typename OBJ, typename T1, typename T2, typename T3>
static MFunc03& Check(void(OBJ::*)(T1, T2, T3));

template<typename OBJ, typename T1, typename T2, typename T3, typename T4>
static MFunc04& Check(void(OBJ::*)(T1, T2, T3, T4));

template<typename OBJ, typename T1, typename T2, typename T3, typename T4, typename T5>
static MFunc05& Check(void(OBJ::*)(T1, T2, T3, T4, T5));

template<typename OBJ, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
static MFunc06& Check(void(OBJ::*)(T1, T2, T3, T4, T5, T6));

template<typename OBJ, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
static MFunc07& Check(void(OBJ::*)(T1, T2, T3, T4, T5, T6, T7));

}