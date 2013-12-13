#include "base\memory\singleton.h"
#include "base\at_exit.h"
#include "base\compiler_specific.h"

class Test {
public:
  Test() {}
  static Test* GetInstance(){
    return Singleton<Test>::get();
  };
  
private:
  char x_[29];
};

/*
          1.POD数据类型
          2.内存对齐
          ALIGNAS(4096) uint8 data;
          assert(((int)&data & (int)(4096 - 1)) == 0);
*/

int main() {
  base::AtExitManager s;
  Test* t = Test::GetInstance();
  
  return 0;
}