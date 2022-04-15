#pragma once
#include "GeometricObject.h"
class Sphere:public GeometricObject
{
public:
	Sphere(void);
	Sphere(Point3D& center, double radius, RGBColor& color=red)
		:center(center),radius(radius),color(color){}

	//Sphere(Point3D& center, double radius, Material* material);

	~Sphere(void);
	bool hit(const Ray& ray, float& tmin, ShadeRec& sr)const;
	bool shadow_hit(const Ray& ray, float& tmin)const;//是否处于阴影之中

	//自发光
	//virtual Point3D sample(void);
	//virtual float pdf(ShadeRec& sr);
	//virtual Vector3D get_normal(const Point3D& p);

public:
	Point3D center;//球心
	double radius;//半径
	RGBColor color;//颜色
};

