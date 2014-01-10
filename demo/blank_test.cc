#include "base\test\unit_test.h"
#include <math.h>
#include <string>
// 辗转相除法求最大公约数
int gcd(int m,int n) {
  if(m % n == 0) 
    return n;
  else
    return gcd(n, m % n);
}

// 由于系统的pow不支持x<0，y 为小数的计算，修改使支持
double UnSignedPow(const double x, const double y) {
  if (x >= 0)
    return ::pow(x,y);

  double pow_y = y;
  // y = y_m / y_n;
  int y_m = (int)pow_y;
  int y_n = 1;

  // 程序浮点误差
  while(::fabs(pow_y) >= 0.000001) {
    pow_y = (pow_y - (int)pow_y) * 10;
    // 由于浮点原因出现数值溢出
    if (y_n * 10 < 0 ||
      (int)(::fabs(y) * y_n * 10) < 0)
      break;

    y_n *= 10;
    y_m = (int)(y * y_n);
  }

  int c = gcd(y_m, y_n);// 最大公约数
  y_m /= c;
  y_n /= c;

  return ::pow(-x,y) * ((y_m + y_n) % 2 ? -1 : 1);
}

struct {
 double x;
 double y;
}kData[] = {
  {32,3}, {-2.1, 4.3323127}, {-2.4, -2.4334},{-2.4, -2},
};

UNIT_TEST(blank_test) {
  double d = 0;
  for(int i = 0; i < _countof(kData); i ++) {
    d = UnSignedPow(kData[i].x, kData[i].y);
  }

  std::string bb = "hello word , orwq vb ";
  std::string strsrc = "or";
  std::string strdst = "orl";
  std::string::size_type pos = 0;
  std::string::size_type srclen = strsrc.size();
  std::string::size_type dstlen = strdst.size();
  const char* p = bb.c_str() + 4;
  while( (pos=bb.find(strsrc, pos)) != std::string::npos )
  {
    bb.replace( pos, srclen, strdst);
    pos += dstlen;
  }
}