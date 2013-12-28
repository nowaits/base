#include "base\test\unit_test.h"

#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <windows.h>
#include <iostream>
class Console {
public:
  Console() {
    AllocConsole();
    Attach(300, 80);
  }
  Console(LPCTSTR lpszTitle, 
    SHORT ConsoleHeight = 300, SHORT ConsoleWidth = 80) {
    AllocConsole();
    SetConsoleTitle(lpszTitle);
    Attach(ConsoleHeight, ConsoleWidth);
  }
  ~Console() {
      FreeConsole();
  }

private:

  void Console::Attach(SHORT ConsoleHeight, SHORT ConsoleWidth)   {
    HANDLE  hStd;
    int     fd;
    FILE    *file;

    // 重定向标准输入流句柄到新的控制台窗口
    hStd = GetStdHandle(STD_INPUT_HANDLE);
    fd = _open_osfhandle(reinterpret_cast<intptr_t>(hStd), _O_TEXT); // 文本模式
    file = _fdopen(fd, "r");
    setvbuf(file, NULL, _IONBF, 0); // 无缓冲
    *stdin = *file;

    // 重定向标准输出流句柄到新的控制台窗口
    hStd = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD size;
    size.X = ConsoleWidth;
    size.Y = ConsoleHeight;
    SetConsoleScreenBufferSize(hStd, size);
    fd = _open_osfhandle(reinterpret_cast<intptr_t>(hStd), _O_TEXT); //文本模式
    file = _fdopen(fd, "w");
    setvbuf(file, NULL, _IONBF, 0); // 无缓冲
    *stdout = *file;

    // 重定向标准错误流句柄到新的控制台窗口
    hStd = GetStdHandle(STD_ERROR_HANDLE);
    fd = _open_osfhandle(reinterpret_cast<intptr_t>(hStd), _O_TEXT); // 文本模式
    file = _fdopen(fd, "w");
    setvbuf(file, NULL, _IONBF, 0); // 无缓冲
    *stderr = *file;

  }
};

UNIT_TEST(Console) {
  std::auto_ptr<Console> console(new Console);

  std::cout<<"new";
}