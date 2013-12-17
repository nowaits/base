#pragma once

template <bool request_default_value>
struct RequestDefaultValueHelper;
typedef char Yes[1];
typedef char No[2];

template <>
struct RequestDefaultValueHelper<false> {
  template <typename T>
  static No& RequestDefaultValue(T t);
};

template <>
struct RequestDefaultValueHelper<true> {
  template <typename T>
  static Yes& RequestDefaultValue(T t);
};

#define REQUEST_DEFAULT_VALUE(fun) (\
  sizeof(RequestDefaultValueHelper<!IS_VOID_TYPE(fun)>::RequestDefaultValue(fun)) == \
  sizeof(Yes))
