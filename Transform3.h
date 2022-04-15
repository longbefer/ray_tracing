#pragma once
#include "Point3D.h"

class CTransform3
{
public:
	CTransform3(void);
	~CTransform3(void);
	void SetMatrix(CP3* P, int ptNumber);
	void Identity();//��λ����
	void Translate(double tx, double ty, double tz);//ƽ�Ʊ任����
	void RotateX(double beta);//��X����ת�任����
	void RotateY(double beta);//��Y����ת�任����
	void RotateZ(double beta);//��ת�任����
	void MultiplyMatrix();//�������
public:
	double T[4][4];
	CP3*   P;
	int    ptNumber;
};

