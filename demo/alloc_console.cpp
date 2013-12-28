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

    // �ض����׼������������µĿ���̨����
    hStd = GetStdHandle(STD_INPUT_HANDLE);
    fd = _open_osfhandle(reinterpret_cast<intptr_t>(hStd), _O_TEXT); // �ı�ģʽ
    file = _fdopen(fd, "r");
    setvbuf(file, NULL, _IONBF, 0); // �޻���
    *stdin = *file;

    // �ض����׼�����������µĿ���̨����
    hStd = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD size;
    size.X = ConsoleWidth;
    size.Y = ConsoleHeight;
    SetConsoleScreenBufferSize(hStd, size);
    fd = _open_osfhandle(reinterpret_cast<intptr_t>(hStd), _O_TEXT); //�ı�ģʽ
    file = _fdopen(fd, "w");
    setvbuf(file, NULL, _IONBF, 0); // �޻���
    *stdout = *file;

    // �ض����׼������������µĿ���̨����
    hStd = GetStdHandle(STD_ERROR_HANDLE);
    fd = _open_osfhandle(reinterpret_cast<intptr_t>(hStd), _O_TEXT); // �ı�ģʽ
    file = _fdopen(fd, "w");
    setvbuf(file, NULL, _IONBF, 0); // �޻���
    *stderr = *file;

  }
};

UNIT_TEST(Console) {
  std::auto_ptr<Console> console(new Console);

  std::cout<<"new";
}