#pragma once
#include "GeometricObject.h"
class Plane :public GeometricObject
{
public:
	Plane();
	Plane(Point3D& point, Vector3D& n, int width = 500, int height = 500, RGBColor color = white)
		:point(point), normal(n), color(color), width(width), height(height) {};
	void set_normal(Vector3D& n);
	void set_point(Point3D& point);
	void set_color(RGBColor& color);
public:
	bool hit(const Ray& ray, float& tmin, ShadeRec& sr)const;
	bool shadow_hit(const Ray& ray, float& tmin)const;//是否处于阴影之中

	//自发光
	//virtual Point3D sample(void);
	//virtual float pdf(ShadeRec& sr);
	//virtual Vector3D get_normal(const Point3D& p);

public:
	Vector3D normal;//平面的向量
	Point3D point;//平面上一点(中心点)
	RGBColor color;
	int width, height;
};

