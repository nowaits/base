#pragma once

namespace function_type {

typedef char TypeUnMatch[1];
typedef char TypeEque[2];
typedef char ObjYes[3];
typedef char GlobYes[4];

template <typename A, typename B>
TypeUnMatch& RetTypeCheck(A, B);

template <typename A>
TypeEque& RetTypeCheck(A, A);

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

//////////////////////////////////////////////////////////////////////////

template <typename A>
GlobYes& RetTypeCheck(A&(*)(), A);

template <typename A, typename T1>
GlobYes& RetTypeCheck(A&(*)(T1), A);

template <typename A, typename T1, typename T2>
GlobYes& RetTypeCheck(A&(*)(T1, T2), A);

template <typename A, typename T1, typename T2, typename T3>
GlobYes& RetTypeCheck(A&(*)(T1, T2, T3), A);

template <typename A, typename T1, typename T2, typename T3, typename T4>
GlobYes& RetTypeCheck(A&(*)(T1, T2, T3, T4), A);

template <typename A, typename T1, typename T2, typename T3, typename T4, typename T5>
GlobYes& RetTypeCheck(A&(*)(T1, T2, T3, T4, T5), A);

template <typename A, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
GlobYes& RetTypeCheck(A&(*)(T1, T2, T3, T4, T5, T6), A);

template <typename A, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
GlobYes& RetTypeCheck(A&(*)(T1, T2, T3, T4, T5, T6, T7), A);

template <typename A, typename OBJ>
ObjYes& RetTypeCheck(A&(OBJ::*)(), A);

template <typename A, typename OBJ, typename T1>
ObjYes& RetTypeCheck(A&(OBJ::*)(T1), A);

template <typename A, typename OBJ, typename T1, typename T2>
ObjYes& RetTypeCheck(A&(OBJ::*)(T1, T2), A);

template <typename A, typename OBJ, typename T1, typename T2, typename T3>
ObjYes& RetTypeCheck(A&(OBJ::*)(T1, T2, T3), A);

template <typename A, typename OBJ, typename T1, typename T2, typename T3, typename T4>
ObjYes& RetTypeCheck(A&(OBJ::*)(T1, T2, T3, T4), A);

template <typename A, typename OBJ, typename T1, typename T2, typename T3, typename T4, typename T5>
ObjYes& RetTypeCheck(A&(OBJ::*)(T1, T2, T3, T4, T5), A);

template <typename A, typename OBJ, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
ObjYes& RetTypeCheck(A&(OBJ::*)(T1, T2, T3, T4, T5, T6), A);

template <typename A, typename OBJ, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
ObjYes& RetTypeCheck(A&(OBJ::*)(T1, T2, T3, T4, T5, T6, T7), A);

//////////////////////////////////////////////////////////////////////////

template <typename A>
GlobYes& RetTypeCheck(const A(*)(), A);

template <typename A, typename T1>
GlobYes& RetTypeCheck(const A(*)(T1), A);

template <typename A, typename T1, typename T2>
GlobYes& RetTypeCheck(const A(*)(T1, T2), A);

template <typename A, typename T1, typename T2, typename T3>
GlobYes& RetTypeCheck(const A(*)(T1, T2, T3), A);

template <typename A, typename T1, typename T2, typename T3, typename T4>
GlobYes& RetTypeCheck(const A(*)(T1, T2, T3, T4), A);

template <typename A, typename T1, typename T2, typename T3, typename T4, typename T5>
GlobYes& RetTypeCheck(const A(*)(T1, T2, T3, T4, T5), A);

template <typename A, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
GlobYes& RetTypeCheck(const A(*)(T1, T2, T3, T4, T5, T6), A);

template <typename A, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
GlobYes& RetTypeCheck(const A(*)(T1, T2, T3, T4, T5, T6, T7), A);

template <typename A, typename OBJ>
ObjYes& RetTypeCheck(const A(OBJ::*)(), A);

template <typename A, typename OBJ, typename T1>
ObjYes& RetTypeCheck(const A(OBJ::*)(T1), A);

template <typename A, typename OBJ, typename T1, typename T2>
ObjYes& RetTypeCheck(const A(OBJ::*)(T1, T2), A);

template <typename A, typename OBJ, typename T1, typename T2, typename T3>
ObjYes& RetTypeCheck(const A(OBJ::*)(T1, T2, T3), A);

template <typename A, typename OBJ, typename T1, typename T2, typename T3, typename T4>
ObjYes& RetTypeCheck(const A(OBJ::*)(T1, T2, T3, T4), A);

template <typename A, typename OBJ, typename T1, typename T2, typename T3, typename T4, typename T5>
ObjYes& RetTypeCheck(const A(OBJ::*)(T1, T2, T3, T4, T5), A);

template <typename A, typename OBJ, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
ObjYes& RetTypeCheck(const A(OBJ::*)(T1, T2, T3, T4, T5, T6), A);

template <typename A, typename OBJ, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
ObjYes& RetTypeCheck(const A(OBJ::*)(T1, T2, T3, T4, T5, T6, T7), A);
}