#include <Windows.h>
#include <WinError.h>
#include "base\test\unit_test.h"
#include <fstream>

void SaveImgToFile(const std::string& file_name, int w, int h, int bit_count, const unsigned char* bytes) {
  BITMAPINFO bi = {0}; 
  bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bi.bmiHeader.biWidth = w;
  bi.bmiHeader.biHeight = h;
  bi.bmiHeader.biPlanes = 1;
  bi.bmiHeader.biBitCount = bit_count;

  BITMAPFILEHEADER bh = {0};
  bh.bfType = 0x4d42;  //bitmap  
  bh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
  bh.bfSize = bh.bfOffBits + ((bi.bmiHeader.biWidth * bi.bmiHeader.biHeight) * bi.bmiHeader.biBitCount / 8);

  {
   std::ofstream fs(file_name, std::ios::binary|std::ios::trunc);
   fs.write((const char*)&bh, sizeof(BITMAPFILEHEADER));
   fs.write((const char*)&(bi.bmiHeader), sizeof(BITMAPINFOHEADER));
   fs.write((const char*)bytes, (bi.bmiHeader.biWidth * bi.bmiHeader.biHeight) * bi.bmiHeader.biBitCount / 8);
  }
}

void CaptureScreenToFile(const std::string& file_name) {
  HDC hScrDC = ::GetDC(NULL);
  HDC hMemDC = NULL;

  const unsigned char *lpBitmapBits = NULL; 

  int nWidth = GetSystemMetrics(SM_CXSCREEN);
  int nHeight = GetSystemMetrics(SM_CYSCREEN); 

  hMemDC = ::CreateCompatibleDC(hScrDC); 

  CURSORINFO hCursorInfo = {sizeof(CURSORINFO), 0, 0, 0};
  GetCursorInfo(&hCursorInfo);
  ICONINFO IconInfo = {0};
  if(GetIconInfo(hCursorInfo.hCursor, &IconInfo))
  {
    hCursorInfo.ptScreenPos.x -= IconInfo.xHotspot;
    hCursorInfo.ptScreenPos.y -= IconInfo.yHotspot;
    if(NULL != IconInfo.hbmMask)
      DeleteObject(IconInfo.hbmMask);
    if(NULL != IconInfo.hbmColor)
      DeleteObject(IconInfo.hbmColor);
  }

  BITMAPINFO bi = {0}; 
  bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bi.bmiHeader.biWidth = nWidth;
  bi.bmiHeader.biHeight = nHeight;
  bi.bmiHeader.biPlanes = 1;
  bi.bmiHeader.biBitCount = 24;

  HBITMAP bitmap = ::CreateDIBSection(hMemDC, &bi, DIB_RGB_COLORS, (LPVOID*)&lpBitmapBits, NULL, 0);
  HGDIOBJ oldbmp = ::SelectObject(hMemDC, bitmap); 

  ::BitBlt(hMemDC, 0, 0, nWidth, nHeight, hScrDC, 0, 0, SRCCOPY|CAPTUREBLT);
  ::DrawIconEx(hMemDC, hCursorInfo.ptScreenPos.x, hCursorInfo.ptScreenPos.y, 
    hCursorInfo.hCursor, 0, 0, 0, NULL, DI_NORMAL );

  SaveImgToFile(file_name, nWidth, nHeight, 24, lpBitmapBits);

  ::SelectObject(hMemDC, oldbmp);
  ::DeleteObject(bitmap);
  ::DeleteObject(hMemDC);
  ::ReleaseDC(NULL, hScrDC);
}

UNIT_TEST(save_to_bmp) {
  std::string file_name = "c:\\b.bmp";
  CaptureScreenToFile(file_name);
}