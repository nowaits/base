#pragma once
typedef char TypeUnMatch[1];
typedef char ObjYes[2];
typedef char GlobYes[3];

template <typename A, typename B>
TypeUnMatch& RetTypeCheck(A, B);

template <typename A>
GlobYes& RetTypeCheck(A(*)(), A);

template <typename A, typename T1>
GlobYes& RetTypeCheck(A(*)(T1), A);

template <typename A, typename T1, typename T2>
GlobYes& RetTypeCheck(A(*)(T1, T2), A);

template <typename A, typename T1, typename T2, typename T3>
GlobYes& RetTypeCheck(A(*)(T1, T2, T3), A);

template <typename A, typename T1, typename T2, typename T3, typename T4>
GlobYes& RetTypeCheck(A(*)(T1, T2, T3, T4), A);

template <typename A, typename T1, typename T2, typename T3, typename T4, typename T5>
GlobYes& RetTypeCheck(A(*)(T1, T2, T3, T4, T5), A);

template <typename A, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
GlobYes& RetTypeCheck(A(*)(T1, T2, T3, T4, T5, T6), A);

template <typename A, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
GlobYes& RetTypeCheck(A(*)(T1, T2, T3, T4, T5, T6, T7), A);

template <typename A, typename OBJ>
ObjYes& RetTypeCheck(A(OBJ::*)(), A);

template <typename A, typename OBJ, typename T1>
ObjYes& RetTypeCheck(A(OBJ::*)(T1), A);

template <typename A, typename OBJ, typename T1, typename T2>
ObjYes& RetTypeCheck(A(OBJ::*)(T1, T2), A);

template <typename A, typename OBJ, typename T1, typename T2, typename T3>
ObjYes& RetTypeCheck(A(OBJ::*)(T1, T2, T3), A);

template <typename A, typename OBJ, typename T1, typename T2, typename T3, typename T4>
ObjYes& RetTypeCheck(A(OBJ::*)(T1, T2, T3, T4), A);

template <typename A, typename OBJ, typename T1, typename T2, typename T3, typename T4, typename T5>
ObjYes& RetTypeCheck(A(OBJ::*)(T1, T2, T3, T4, T5), A);

template <typename A, typename OBJ, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
ObjYes& RetTypeCheck(A(OBJ::*)(T1, T2, T3, T4, T5, T6), A);

template <typename A, typename OBJ, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
ObjYes& RetTypeCheck(A(OBJ::*)(T1, T2, T3, T4, T5, T6, T7), A);

#define IS_RET_TYPE_OK(fun, x) (sizeof(RetTypeCheck(fun, x)) != sizeof(TypeUnMatch))