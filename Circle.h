#pragma once
#include "GeometricObject.h"
class Circle : public GeometricObject
{
public:
	Circle(void);
	Circle(Point3D& point, double radius, Vector3D& normal);
	~Circle();
	bool hit(const Ray& ray, float& tmin, ShadeRec& sr)const;
	bool shadow_hit(const Ray& ray, float& tmin)const;//是否处于阴影之中
public:
	Point3D point;
	Vector3D normal;
	double radius;
};

