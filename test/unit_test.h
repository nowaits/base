/*
简单的测试模板
用法：
UNIT_TEST(test_case_name) {
.....
}
*/

#pragma once
#include <list>
//////////////////////////////////////////////////////////////////////////
class UnitCase {
public:
  virtual ~UnitCase() {};
  virtual void TestBody() = 0;
};

#define GTEST_DISALLOW_COPY_AND_ASSIGN(type)\
  type(const type &);\
  void operator=(const type &)

#define RUN_ALL_TEST() ::UnitList<UnitCase>::GetInstance()->Run()

#define UNIT_TEST(unit_case_name)\
class unit_case_name##_Test : public UnitCase {\
public:\
  unit_case_name##_Test() {;\
  ::UnitList<UnitCase>::GetInstance()->Add(this);\
}\
  ~unit_case_name##_Test() {;\
  ::UnitList<UnitCase>::GetInstance()->Remove(this);\
}\
  virtual void TestBody();\
private:\
  static const unit_case_name##_Test* test_init_;\
  GTEST_DISALLOW_COPY_AND_ASSIGN(unit_case_name##_Test);\
};\
  const unit_case_name##_Test* unit_case_name##_Test::test_init_ \
  = new unit_case_name##_Test;\
  void unit_case_name##_Test::TestBody()

template <typename T>
class UnitList {
public:
  UnitList(){}
  ~UnitList(){ Clear(); }

  static UnitList<T>* GetInstance() {
    static UnitList<T> instance;
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

  void Clear() {
    std::list<T*>::const_iterator it  = list_.begin();
    std::list<T*>::const_iterator end = list_.end();

    while(it != end) {
      delete *it;
      it  = list_.begin();
    }
  }
private:
  std::list<T*> list_;
};