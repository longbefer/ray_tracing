#pragma once
#include "T2.h"
#include "Ray.h"
#include "Vector3D.h"
class ShadeRec;
class SmoothBezier
{
public:
	SmoothBezier(void);
	SmoothBezier(CP3 V[4][4], int ReNumber);
	virtual ~SmoothBezier();
	//void ReadControlPoint(const CP3 V[4][4], int ReNumber)const;//读入16个控制点和递归深度
	BOOL Hit(const Ray& ray, ShadeRec& sr)const;//绘制曲面
private:
	BOOL Recursion(const Ray& ray, ShadeRec& sr, int ReNumber, CT2* pRect)const;//递归函数
	void Tessellation(CT2* pT, PointVector* P)const;//计算曲面的细分点
	BOOL DrawFacet(const Ray& ray, ShadeRec& sr, PointVector* P)const;//绘制四边形
	void LeftMultiplyMatrix(double M[4][4], CP3 V[4][4])const;//左乘顶点矩阵
	void RightMultiplyMatrix(CP3 V[4][4], double M[4][4])const;//右乘顶点矩阵
	void TransposeMatrix(double M[4][4])const;//转置矩阵
private:
	int ReNumber;//递归次数
	CP3 V3[16];//三维控制点
};

