#pragma once
#include "GeometricObject.h"
class Circle : public GeometricObject
{
public:
	Circle(void);
	Circle(Point3D& point, double radius, Vector3D& normal);
	~Circle();
	bool hit(const Ray& ray, float& tmin, ShadeRec& sr)const;
	bool shadow_hit(const Ray& ray, float& tmin)const;//�Ƿ�����Ӱ֮��
public:
	Point3D point;
	Vector3D normal;
	double radius;
};

