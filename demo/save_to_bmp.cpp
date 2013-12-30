#include <Windows.h>
#include <WinError.h>
#include "base\test\unit_test.h"
#include <fstream>
#include <assert.h>

struct BMPINFO{
  BITMAPINFOHEADER    bmiHeader;
  RGBQUAD             bmiColors[256];
};

void SaveImgToFile(const std::string& file_name, BITMAPINFO* lpbi, const unsigned char* bytes) {
  unsigned long data_size = 
    std::abs((lpbi->bmiHeader.biWidth * lpbi->bmiHeader.biHeight))
    * lpbi->bmiHeader.biBitCount / 8;

  BITMAPFILEHEADER bh = {0};
  bh.bfType = 0x4d42;  //bitmap  
  bh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);
  bh.bfSize = bh.bfOffBits + data_size;

  {
    std::ofstream fs(file_name.c_str(), std::ios::binary|std::ios::trunc);
    fs.write((const char*)&bh, sizeof(BITMAPFILEHEADER));
    fs.write((const char*)(lpbi),
      sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD));
    fs.write((const char*)bytes, data_size);
  }
}

bool LoadBmp(const std::string& file_name, BITMAPFILEHEADER& bh,
             std::string& info_buffer, std::string& data_buffer) {

   std::ifstream ifs(file_name.c_str(), std::ios::binary);
   ifs.exceptions( std::ifstream::eofbit | std::ifstream::failbit | std::ifstream::badbit );  

   int current_pos = ifs.tellg();
   ifs.seekg(0, std::ios_base::end);
   unsigned long file_size = ifs.tellg();
   ifs.seekg(current_pos);

   ifs.read((char *)&bh, sizeof(BITMAPFILEHEADER));
   int i = ifs.gcount();

   if (bh.bfType != 0x4d42)
     return false;

   if (file_size != bh.bfSize)
     return false;

   info_buffer.resize(bh.bfOffBits - sizeof(BITMAPFILEHEADER));

   ifs.read(&info_buffer[0], (long)info_buffer.size());
  
   BITMAPINFOHEADER* bmp_header = reinterpret_cast<BITMAPINFOHEADER*>(&info_buffer[0]);

   data_buffer.resize(bmp_header->biWidth * bmp_header->biHeight * bmp_header->biBitCount / 8);

   ifs.seekg(bh.bfOffBits);

   ifs.read(&data_buffer[0], (long)data_buffer.size());
   return true;
}

bool GetDCSize(HDC dc, int& width, int& height) {

  do {
    HBITMAP hBitmap = (HBITMAP)::GetCurrentObject(dc, OBJ_BITMAP);
    if (!hBitmap)
      return false;

    BITMAP bitmaps = {0};
    if (::GetObject(hBitmap, sizeof(BITMAP), &bitmaps) != 0) {
      // layer´°¿Ú»áÊ§°Ü
      width  = bitmaps.bmWidth;
      height = bitmaps.bmHeight;
      break;
    }

    HWND hwnd = ::WindowFromDC(dc);

    if (hwnd == NULL || 
      (::GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_LAYERED) == 0)
      return false;

    RECT rc = {0};
    if (::GetWindowRect(hwnd, &rc) == FALSE)
      return false;

    width  = rc.right - rc.left;
    height = rc.bottom - rc.top;
  }while (false);
  return true;
}

bool MakeOpaqe(BITMAPINFO* lpbi, const unsigned char* bytes) {
  if (!lpbi || lpbi->bmiHeader.biBitCount != 32 || !bytes)
    return false;

  unsigned long row_bytes     = (lpbi->bmiHeader.biWidth << 2);
  unsigned long alpha_chanel  = (0xff<<24);
  unsigned long alpha_chanel_mask  = ~alpha_chanel;

  for(int i = 0; i < lpbi->bmiHeader.biHeight; i ++) {
    unsigned long* lp_row = (unsigned long*)(bytes + row_bytes * i);

    for(int j = 0; j < lpbi->bmiHeader.biWidth; j ++)
      lp_row[j]&=alpha_chanel_mask;
  }
  return true;
}

bool CaptureScreenToFile(const std::string& file_name, HDC hScrDC = ::GetDC(NULL)) {
  HDC hMemDC = NULL;

  const unsigned char *lpBitmapBits = NULL; 

  int width  = 0;
  int height = 0;

  if (!GetDCSize(hScrDC, width, height))
    return false;

  hMemDC = ::CreateCompatibleDC(hScrDC); 

  CURSORINFO hCursorInfo = {sizeof(CURSORINFO), 0, 0, 0};
  ::GetCursorInfo(&hCursorInfo);
  ICONINFO IconInfo = {0};
  if(::GetIconInfo(hCursorInfo.hCursor, &IconInfo))
  {
    hCursorInfo.ptScreenPos.x -= IconInfo.xHotspot;
    hCursorInfo.ptScreenPos.y -= IconInfo.yHotspot;
    if(NULL != IconInfo.hbmMask)
      ::DeleteObject(IconInfo.hbmMask);
    if(NULL != IconInfo.hbmColor)
      ::DeleteObject(IconInfo.hbmColor);
  }

  BMPINFO bmpinfo = {0};

  HBITMAP bitmap = NULL;
  {
    bitmap = CreateCompatibleBitmap(
      hScrDC, width, height);

    bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpinfo.bmiHeader.biBitCount = 0;

    int result = ::GetDIBits(hMemDC, bitmap, 0, 0, NULL, (LPBITMAPINFO)&bmpinfo, DIB_RGB_COLORS);

    if (!result)
      return false;

    if ((::GetDeviceCaps(hMemDC, RASTERCAPS) & RC_PALETTE) == 0) {//Õæ²Ê
      assert(bmpinfo.bmiHeader.biBitCount == 32);
    }

    result = ::GetDIBits(hMemDC, bitmap, 0, 0, NULL, (LPBITMAPINFO)&bmpinfo, DIB_RGB_COLORS);

    if (!result)
      return false;

    ::DeleteObject(bitmap);

    bitmap = 
      ::CreateDIBSection(hMemDC, (LPBITMAPINFO)&bmpinfo, DIB_RGB_COLORS, (LPVOID*)&lpBitmapBits, NULL, 0);
  }

  HGDIOBJ oldbmp = ::SelectObject(hMemDC, bitmap); 

#if 1
  HWND h = ::WindowFromDC(hScrDC);
  long ex_style = 0;
  if (::IsWindow(h) == TRUE && 
    ((ex_style = ::GetWindowLong(h, GWL_EXSTYLE)) & WS_EX_LAYERED)) {
    ::SetLayeredWindowAttributes(h, RGB(0, 0, 0), 0xFF, LWA_ALPHA|LWA_COLORKEY);
   // ::RedrawWindow(h, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
    ::PrintWindow(h, hMemDC, 0);
    ::SetWindowLong(h, GWL_EXSTYLE, ex_style&(~WS_EX_LAYERED));
    ::SetWindowLong(h, GWL_EXSTYLE, ex_style);
    ::RedrawWindow(h, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
  }
  else
    ::BitBlt(hMemDC, 0, 0, width, height,
    hScrDC, 0, 0, SRCCOPY|CAPTUREBLT);
#else
  BLENDFUNCTION bf = {0};
  bf.BlendOp = AC_SRC_OVER;
  bf.BlendFlags = 0;
  bf.AlphaFormat = 0;
  bf.SourceConstantAlpha = 255;

  ::AlphaBlend(hMemDC, 0, 0, width, height, hScrDC, 0, 0, width, height, bf);
#endif
  ::DrawIconEx(hMemDC, hCursorInfo.ptScreenPos.x, hCursorInfo.ptScreenPos.y, 
    hCursorInfo.hCursor, 0, 0, 0, NULL, DI_NORMAL );

//  MakeOpaqe((LPBITMAPINFO)&bmpinfo, lpBitmapBits);
  SaveImgToFile(file_name, (LPBITMAPINFO)&bmpinfo, lpBitmapBits);

  ::SelectObject(hMemDC, oldbmp);
  ::DeleteObject(bitmap);
  ::DeleteObject(hMemDC);

  return true;
}

bool SetDIBToSdevice(HDC dc, const std::string& file_name) {

  BITMAPFILEHEADER bh;
  LPBITMAPINFO lpbmi;
  std::string info_buffer;
  std::string data_buffer;

  if (!LoadBmp(file_name, bh, info_buffer, data_buffer))
    return false;

  if (info_buffer.empty() || data_buffer.empty())
    return false;

  lpbmi = reinterpret_cast<LPBITMAPINFO>(&info_buffer[0]);

  if (lpbmi->bmiHeader.biBitCount != 24 && lpbmi->bmiHeader.biBitCount != 32)
    return false;

  return ::SetDIBitsToDevice(dc, 0, 0,
    lpbmi->bmiHeader.biWidth, lpbmi->bmiHeader.biHeight,
    0, 0, 0, -(lpbmi->bmiHeader.biHeight), (LPVOID*)&data_buffer[0], 
    lpbmi, DIB_RGB_COLORS) != 0;
}

UNIT_TEST(save_to_bmp) {
  std::string file_name_a = "c:\\a.bmp";
  std::string file_name_b = "c:\\b.bmp";
 
  POINT point = {0};
  int width, height;

  assert(::GetCursorPos(&point) != FALSE);
  HWND hwnd = ::GetAncestor(::WindowFromPoint(point), GA_ROOT);

  HDC hScreen = ::GetDC(hwnd);
  assert(GetDCSize(hScreen, width, height));

  HDC hMemDc = ::CreateCompatibleDC(hScreen);

  HBITMAP bitmap =
    ::CreateCompatibleBitmap(
    hScreen, 
    width, height);
  HBITMAP old_bmp = (HBITMAP)::SelectObject(hMemDc, bitmap);

  assert(CaptureScreenToFile(file_name_a, hScreen));
  assert(SetDIBToSdevice(hMemDc, file_name_a));
  assert(CaptureScreenToFile(file_name_b, hMemDc));

  ::ReleaseDC(hwnd, hScreen);
}
//////////////////////////////////////////////////////////////////////////

void CentreWindow(HWND hwnd)
{
  RECT winrect, workrect;

  // Find how large the desktop work area is
  ::SystemParametersInfo(SPI_GETWORKAREA, 0, &workrect, 0);
  int workwidth = workrect.right -  workrect.left;
  int workheight = workrect.bottom - workrect.top;

  // And how big the window is
  ::GetWindowRect(hwnd, &winrect);
  int winwidth = winrect.right - winrect.left;
  int winheight = winrect.bottom - winrect.top;
  // Make sure it's not bigger than the work area
  winwidth = min(winwidth, workwidth);
  winheight = min(winheight, workheight);

  // Now centre it
  ::SetWindowPos(hwnd, 
    HWND_TOP,
    workrect.left + (workwidth-winwidth) / 2,
    workrect.top + (workheight-winheight) / 2,
    winwidth, winheight, 
    SWP_SHOWWINDOW);
  ::SetForegroundWindow(hwnd);
}

UNIT_TEST(CentreWindow) {
  CentreWindow(::GetForegroundWindow());
}