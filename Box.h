#pragma once
#include "GeometricObject.h"
class Box :public GeometricObject
{
public:
	Box(void);
	Box(Point3D& p1, Point3D& p2);
	~Box(void);
	bool hit(const Ray& ray, float& tmin, ShadeRec& sr)const;
	bool shadow_hit(const Ray& ray, float& tmin)const;//是否处于阴影之中
	Vector3D get_normal(const int face_hit)const;
public:
	Point3D _bottom_dowm;
	Point3D _top_up;
};

