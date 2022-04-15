#pragma once
#include "T2.h"
#include "Ray.h"
#include "Vector3D.h"
class ShadeRec;
class SmoothBicubicBezierPatch
{
public:
	SmoothBicubicBezierPatch(CP3 P[4][4]);
	virtual ~SmoothBicubicBezierPatch(void);
	bool Hit(const Ray& ray, ShadeRec& sr)const;//����˫���α���������
private:
	void ReadCurvedVertex(PointVector** V, double tStep)const;//����˫����Bezier����Ƭ�ϵĵ���Ϣ
	void LeftMultiplyMatrix(double M[][4], CP3 P[][4])const;//��˶������
	void RightMultiplyMatrix(CP3 P[][4], double M[][4])const;//�ҳ˶������
	void TransposeMatrix(double M[][4])const;//ת�þ���
public:
	CP3 P[4][4];//��ά���Ƶ�
};

