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
	//void ReadControlPoint(const CP3 V[4][4], int ReNumber)const;//����16�����Ƶ�͵ݹ����
	BOOL Hit(const Ray& ray, ShadeRec& sr)const;//��������
private:
	BOOL Recursion(const Ray& ray, ShadeRec& sr, int ReNumber, CT2* pRect)const;//�ݹ麯��
	void Tessellation(CT2* pT, PointVector* P)const;//���������ϸ�ֵ�
	BOOL DrawFacet(const Ray& ray, ShadeRec& sr, PointVector* P)const;//�����ı���
	void LeftMultiplyMatrix(double M[4][4], CP3 V[4][4])const;//��˶������
	void RightMultiplyMatrix(CP3 V[4][4], double M[4][4])const;//�ҳ˶������
	void TransposeMatrix(double M[4][4])const;//ת�þ���
private:
	int ReNumber;//�ݹ����
	CP3 V3[16];//��ά���Ƶ�
};

