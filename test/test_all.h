/*
简单的测试模板
用法：
TEST(test_case_name) {
.....
}
*/

#pragma once
#include <list>

class TestBase {
public:
  virtual void TestBody() = 0;
};

template <typename T>
class TestList {
public:
  static TestList<T>* GetInstance() {
    static TestList<T> instance;
    return &instance;
  }

  void Add(T* test_body) {
    list_.push_back(test_body);
  }

  void Remove(T* test_body) {
    list_.remove(test_body);
  }
  void Run() {
    std::list<T*>::const_iterator it  = list_.begin();
    std::list<T*>::const_iterator end = list_.end();

    while(it != end) {
      (*it)->TestBody();
      it ++;
    }
  }
private:
  std::list<T*> list_;
};

#define GTEST_DISALLOW_COPY_AND_ASSIGN(type)\
  type(const type &);\
  void operator=(const type &)

#define RUN_ALL_TEST() TestList<TestBase>::GetInstance()->Run()

#define TEST(test_case_name)\
class test_case_name##_Test : public TestBase {\
public:\
  test_case_name##_Test() {;\
  TestList<TestBase>::GetInstance()->Add(this);\
}\
  ~test_case_name##_Test() {;\
  TestList<TestBase>::GetInstance()->Remove(this);\
}\
  virtual void TestBody();\
private:\
  static const test_case_name##_Test test_init_;\
  GTEST_DISALLOW_COPY_AND_ASSIGN(test_case_name##_Test);\
};\
  const test_case_name##_Test test_case_name##_Test::test_init_;\
  void test_case_name##_Test::TestBody()