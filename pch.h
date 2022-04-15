// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

// 添加要在此处预编译的标头
#include "framework.h"

#endif //PCH_H
#include <math.h>
#include <cmath>
#define ROUND(a) int(a+0.5)
const double PI = 3.14159265358979323846;   // pi
const double invPI = 0.318309886183790671538;  // 1/pi
#define     EQN_EPS     1e-90

#define	IsZero(x)	((x) > -EQN_EPS && (x) < EQN_EPS)

#ifndef CBRT
#define     cbrt(x)  ((x) > 0.0 ? pow((double)(x), 1.0/3.0) : \
			  		 ((x) < 0.0 ? -pow((double)-(x), 1.0/3.0) : 0.0))
#endif

int SolveQuadric(double c[3], double s[2]);
int SolveCubic(double c[4], double s[3]);
int SolveQuartic(double c[5], double s[4]);