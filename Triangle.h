#pragma once
#include "GeometricObject.h"
class Triangle : public GeometricObject
{
public:
	Triangle(void);
	Triangle(Point3D& v0, Point3D& v1, Point3D& v2);
	bool hit(const Ray& ray, float& tmin, ShadeRec& sr)const;
	bool shadow_hit(const Ray& ray, float& tmin)const;//是否处于阴影之中
public:
	Point3D v0, v1, v2;
	Vector3D normal;

};

