#include "pch.h"
#include "Circle.h"
#include "ShadeRec.h"
Circle::Circle(void)
{
	point = Point3D(0, 0, 0);
	radius = 100.0;
	normal = Vector3D(0, 0, -1);
}

Circle::Circle(Point3D& point, double radius, Vector3D& normal)
{
	this->point = point;
	this->normal = normal;
	this->radius = radius;
}

Circle::~Circle()
{
}

bool Circle::hit(const Ray& ray, float& tmin, ShadeRec& sr) const
{
	//double kEpsilon = 1e-4;
	//float t = Dot((point - ray.o), normal) / Dot(ray.d, normal);
	//if (t <= kEpsilon)
	//	return false;

	//Point3D p = ray.o + Point3D(t * ray.d.x, t * ray.d.y, t * ray.d.z);
	//double d = Vector3D(p - point).Mag();
	//if (d > radius)
	//	return false;
	//tmin = t;
	if (!shadow_hit(ray, tmin))
		return false;
	sr.normal = normal;
	sr.hit_point = ray.o + Point3D(tmin * ray.d.x, tmin * ray.d.y, tmin * ray.d.z);
	sr.color = color;
	return true;
}

bool Circle::shadow_hit(const Ray& ray, float& tmin)const
{
	double kEpsilon = 1e-4;
	float t = (float)(Dot((point - ray.o), normal) / Dot(ray.d, normal));
	if (t <= kEpsilon)//²»ÔÚÔ²ÉÏ
		return false;

	Point3D p = ray.o + Point3D(t * ray.d.x, t * ray.d.y, t * ray.d.z);
	double d = Vector3D(p - point).Mag();
	if (d > radius)
		return false;
	tmin = t;
	return true;
}
