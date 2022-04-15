#pragma once
#include "GeometricObject.h"
class Torus: public GeometricObject
{
public:
	Torus(void);
	Torus(Point3D& point, double inner, double outer);
	~Torus(void);
	Vector3D compute_normal(Point3D& P)const;
	bool hit(const Ray& ray, float& tmin, ShadeRec& sr)const;
	bool shadow_hit(const Ray& ray, float& tmin)const;//是否处于阴影之中
public:
	Point3D point;
	double inner_radius, outer_radius;
};

