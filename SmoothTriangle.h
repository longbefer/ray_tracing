#pragma once
#include "GeometricObject.h"
class SmoothTriangle : public GeometricObject//��������;������������ƴ�ɵ������ƽ��Ч��
{
	//����alpha��beta�Ĳ�ֵ
public:
	SmoothTriangle(void);//ʹ�÷�������Ҫȷ���������������ʸ��
	SmoothTriangle(PointVector& v0, PointVector& v1, PointVector& v2);
	bool hit(const Ray& ray, float& tmin, ShadeRec& sr)const;
	bool shadow_hit(const Ray& ray, float& tmin)const;//�Ƿ�����Ӱ֮��
public:
	PointVector v0, v1, v2;
	//Vector3D normal;
};

