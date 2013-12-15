#include "base\test\test_all.h"
#include "base\at_exit.h"

int main() {
  base::AtExitManager g_exit;
  RUN_ALL_TEST();
  return 0;
}