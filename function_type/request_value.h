#pragma once
namespace function_type {

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

}