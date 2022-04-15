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
	bool Hit(const Ray& ray, ShadeRec& sr)const;//����˫���α���������
private:
	void ReadCurvedVertex(Point3D** V, double tStep)const;//����˫����Bezier����Ƭ�ϵĵ���Ϣ
	void LeftMultiplyMatrix(double M[][4], CP3 P[][4])const;//��˶������
	void RightMultiplyMatrix(CP3 P[][4], double M[][4])const;//�ҳ˶������
	void TransposeMatrix(double M[][4])const;//ת�þ���
public:
	CP3 P[4][4];//��ά���Ƶ�
};

