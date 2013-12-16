/*
简单的测试模板
用法：
UNIT_TEST(test_case_name) {
.....
}
*/

#pragma once
#include <queue>
//////////////////////////////////////////////////////////////////////////
#define GTEST_DISALLOW_COPY_AND_ASSIGN(type)\
  type(const type &);\
  void operator=(const type &)

class Unitqueue {
public:
  class Delegate {
  public:
    virtual ~Delegate() {};
    virtual void TestBody() = 0;
  };

  Unitqueue(){}
  ~Unitqueue(){ Clear(); }

  static Unitqueue* GetInstance() {
    static Unitqueue instance;
    return &instance;
  }

  void push(Delegate* test_body) {
    unit_queque_.push(test_body);
  }

  void Run() {
    while (!unit_queque_.empty()) {
      unit_queque_.front()->TestBody();
      delete unit_queque_.front();

      unit_queque_.pop();
    }
  }

  void Clear() {
    while (!unit_queque_.empty()) {
      delete unit_queque_.front();
      unit_queque_.pop();
    }
  }
private:
  std::queue<Delegate*> unit_queque_;

  GTEST_DISALLOW_COPY_AND_ASSIGN(Unitqueue);
};

#define RUN_ALL_TEST() ::Unitqueue::GetInstance()->Run()

#define UNIT_TEST(unit_case_name)\
class unit_case_name##_Test : public Unitqueue::Delegate {\
public:\
  unit_case_name##_Test() {;\
  ::Unitqueue::GetInstance()->push(this);\
}\
  virtual void TestBody();\
private:\
  static const unit_case_name##_Test* test_init_;\
  GTEST_DISALLOW_COPY_AND_ASSIGN(unit_case_name##_Test);\
};\
  const unit_case_name##_Test* unit_case_name##_Test::test_init_ \
  = new unit_case_name##_Test;\
  void unit_case_name##_Test::TestBody()
