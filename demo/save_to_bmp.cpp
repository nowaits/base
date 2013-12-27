#include <Windows.h>
#include <WinError.h>
#include "base\test\unit_test.h"
#include <fstream>

void SaveImgToFile(const std::string& file_name, int w, int h, int bit_count, const unsigned char* bytes) {
  unsigned long data_size = std::abs((w * h)) * bit_count / 8;

  BITMAPINFO bi = {0}; 
  bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bi.bmiHeader.biWidth = w;
  bi.bmiHeader.biHeight = h;
  bi.bmiHeader.biPlanes = 1;
  bi.bmiHeader.biBitCount = bit_count;

  BITMAPFILEHEADER bh = {0};
  bh.bfType = 0x4d42;  //bitmap  
  bh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
  bh.bfSize = bh.bfOffBits + data_size;

  {
    std::ofstream fs(file_name.c_str(), std::ios::binary|std::ios::trunc);
    fs.write((const char*)&bh, sizeof(BITMAPFILEHEADER));
    fs.write((const char*)&(bi.bmiHeader), sizeof(BITMAPINFOHEADER));
    fs.write((const char*)bytes, data_size);
  }
}

void CaptureScreenToFile(const std::string& file_name) {
  ::GdiFlush();
  HDC hScrDC = ::GetDC(NULL);
  HDC hMemDC = NULL;

  const unsigned char *lpBitmapBits = NULL; 

  RECT screenrect;
  screenrect.left		= GetSystemMetrics(SM_XVIRTUALSCREEN);
  screenrect.top		= GetSystemMetrics(SM_YVIRTUALSCREEN);
  screenrect.right	= screenrect.left + GetSystemMetrics(SM_CXVIRTUALSCREEN);
  screenrect.bottom	= screenrect.top + GetSystemMetrics(SM_CYVIRTUALSCREEN);

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

  BITMAPINFO* bi = (BITMAPINFO*)::malloc(sizeof(BITMAPINFOHEADER));

  HBITMAP bitmap = NULL;

  {
    bitmap = CreateCompatibleBitmap(
      hScrDC, 
      screenrect.right - screenrect.left, 
      screenrect.bottom - screenrect.top);

    bi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bi->bmiHeader.biBitCount = 0;

    int result = ::GetDIBits(hMemDC, bitmap, 0, 0, NULL, bi, DIB_RGB_COLORS);

    if (!result)
      return;

    if ((GetDeviceCaps(hMemDC, RASTERCAPS) & RC_PALETTE) == 0) {// Õæ²Ê
      if (bi->bmiHeader.biBitCount == 32) {
        bi = (BITMAPINFO*)::realloc(bi, sizeof(BITMAPINFOHEADER) +  3 * sizeof(RGBQUAD));
      }
    }

    result = ::GetDIBits(hMemDC, bitmap, 0, 0, NULL, bi, DIB_RGB_COLORS);

    if (!result)
      return;

    DeleteObject(bitmap);

    bitmap = 
      CreateDIBSection(hMemDC, bi, DIB_RGB_COLORS, (LPVOID*)&lpBitmapBits, NULL, 0);
  }

  HGDIOBJ oldbmp = ::SelectObject(hMemDC, bitmap); 

  ::BitBlt(hMemDC, 0, 0,
    screenrect.right - screenrect.left,
    screenrect.bottom - screenrect.top, 
    hScrDC, 0, 0, SRCCOPY|CAPTUREBLT);
  ::DrawIconEx(hMemDC, hCursorInfo.ptScreenPos.x, hCursorInfo.ptScreenPos.y, 
    hCursorInfo.hCursor, 0, 0, 0, NULL, DI_NORMAL );

  SaveImgToFile(file_name,
    screenrect.right - screenrect.left,
    screenrect.bottom - screenrect.top,
    bi->bmiHeader.biBitCount, lpBitmapBits);

  ::SelectObject(hMemDC, oldbmp);
  ::DeleteObject(bitmap);
  ::DeleteObject(hMemDC);
  ::ReleaseDC(NULL, hScrDC);
  ::free(bi);
}

UNIT_TEST(save_to_bmp) {
  std::string file_name = "c:\\sb.bmp";
  CaptureScreenToFile(file_name);
}