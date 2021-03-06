#pragma once

#include "common_feature.h"
#include "request_value.h"
#include "ret_type_math.h"

#define CHECK_TYPE(fun) sizeof(function_type::Check(fun))

#define IS_VOID_TYPE(fun) (\
  CHECK_TYPE(fun) >= sizeof(function_type::GFunc00) && CHECK_TYPE(fun) <= sizeof(function_type::GFunc07) || \
  CHECK_TYPE(fun) >= sizeof(function_type::MFunc00) && CHECK_TYPE(fun) <= sizeof(function_type::MFunc07))

#define IS_OBJ_FUN(fun) (\
  CHECK_TYPE(fun) >= sizeof(function_type::MFunc0) && CHECK_TYPE(fun) <= sizeof(function_type::MFunc7) || \
  CHECK_TYPE(fun) >= sizeof(function_type::MFunc00) && CHECK_TYPE(fun) <= sizeof(function_type::MFunc07))

#define ARG_NUM(fun) (\
  CHECK_TYPE(fun) >= sizeof(function_type::GFunc0) && CHECK_TYPE(fun) <= sizeof(function_type::GFunc7) ? CHECK_TYPE(fun) - sizeof(function_type::GFunc0) : (\
  CHECK_TYPE(fun) >= sizeof(function_type::GFunc00) && CHECK_TYPE(fun) <= sizeof(function_type::GFunc07) ? CHECK_TYPE(fun) - sizeof(function_type::GFunc00) : (\
  CHECK_TYPE(fun) >= sizeof(function_type::MFunc0) && CHECK_TYPE(fun) <= sizeof(function_type::MFunc7) ? CHECK_TYPE(fun) - sizeof(function_type::MFunc0) : (\
  CHECK_TYPE(fun) >= sizeof(function_type::MFunc00) && CHECK_TYPE(fun) <= sizeof(function_type::MFunc07) ? CHECK_TYPE(fun) - sizeof(function_type::MFunc00) : \
  -1)

//
#define REQUEST_DEFAULT_VALUE(fun) (\
  sizeof(function_type::RequestDefaultValueHelper<!IS_VOID_TYPE(fun)>::RequestDefaultValue(fun)) == \
  sizeof(function_type::Yes))

//
#define IS_EQUE_TYPE(a, b) (sizeof(function_type::RetTypeCheck(a, b)) == sizeof(function_type::TypeEque))
#define IS_RET_TYPE_OK(fun, x) (\
  sizeof(function_type::RetTypeCheck(fun, x)) != sizeof(function_type::TypeUnMatch) &&\
  sizeof(function_type::RetTypeCheck(fun, x)) != sizeof(function_type::TypeEque))


namespace function_type {

template<typename T, bool boos =  REQUEST_DEFAULT_VALUE(*((T*)0))>
struct NeedDefaultRetValueHelper;

template<typename T>
struct NeedDefaultRetValueHelper<T, true> {
  static Yes& NeedDefaultRetValue(T t);
};

template<typename T>
struct NeedDefaultRetValueHelper<T, false> {
  static No& NeedDefaultRetValue(T t);
};
}

template<typename T>
bool NeedDefaultRetValue(T t) {
  return sizeof(function_type::NeedDefaultRetValueHelper<T>::NeedDefaultRetValue(t)) == sizeof(function_type::Yes);
}