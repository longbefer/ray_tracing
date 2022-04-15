#pragma once
#include "GeometricObject.h"
class Cylinder ://有问题，待修复
	public GeometricObject
{
public:
	Cylinder(void);
	Cylinder(Point3D& top_point, Point3D& bottom_point, const double radius);
	~Cylinder(void);
public:
	bool hit(const Ray& ray, float& tmin, ShadeRec& sr)const;
	bool shadow_hit(const Ray& ray, float& tmin)const;//是否处于阴影之中
public:
	Point3D top_point;
	Point3D bottom_point;
	double radius;
};

