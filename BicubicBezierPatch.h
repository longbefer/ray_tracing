#pragma once
#include "T2.h"
#include "Vector3D.h"
#include "Ray.h"
class ShadeRec;
class CBicubicBezierPatch
{
public:
	CBicubicBezierPatch(CP3 P[4][4]);
	virtual ~CBicubicBezierPatch(void);
	bool Hit(const Ray& ray, ShadeRec& sr)const;//绘制双三次贝塞尔曲面
private:
	void ReadCurvedVertex(Point3D** V, double tStep)const;//保存双三次Bezier曲面片上的点信息
	void LeftMultiplyMatrix(double M[][4], CP3 P[][4])const;//左乘顶点矩阵
	void RightMultiplyMatrix(CP3 P[][4], double M[][4])const;//右乘顶点矩阵
	void TransposeMatrix(double M[][4])const;//转置矩阵
public:
	CP3 P[4][4];//三维控制点
};

