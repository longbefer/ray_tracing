#include "pch.h"
#include "Plane.h"
#include "ShadeRec.h"
Plane::Plane()
{
	normal = Vector3D(0, 0, 1);
	point = Point3D(0, 0, -500);
	color = white;
	width = height = 500;
}

void Plane::set_normal(Vector3D& n)
{
	this->normal = n;
}

void Plane::set_point(Point3D& point)
{
	this->point = point;
}

void Plane::set_color(RGBColor& color)
{
	this->color = color;
}

bool Plane::hit(const Ray& ray, float& tmin, ShadeRec& sr)const
{
	if (!shadow_hit(ray, tmin))
		return false;
	sr.normal = normal;
	sr.hit_point = ray.o + Point3D(tmin * ray.d.x, tmin * ray.d.y, tmin * ray.d.z);
	sr.color = color;
	return true;
	//double t = Dot(Vector3D(ray.o, point), normal) / (Dot(ray.d, normal));
	//double kEpsilon = 1e-4;
	//if (t > kEpsilon) {
	//	tmin = (float)t;
	//	sr.normal = normal;
	//	sr.hit_point = ray.o + Point3D(t * ray.d.x, t * ray.d.y, t * ray.d.z);
	//	sr.color = color;
	//	return true;
	//}
	//return false;
}

bool Plane::shadow_hit(const Ray& ray, float& tmin)const
{
	double t = Dot(Vector3D(ray.o, point), normal) / (Dot(ray.d, normal));
	double kEpsilon = 1e-4;
	if (t > kEpsilon) {
		tmin = (float)t;
		return true;
	}
	return false;
}

//Point3D Plane::sample(void)
//{
//	/*Point3D sample_point = sampler_ptr->sample_unit_square();
//	return point + Point3D(sample_point.x * width, sample_point.y * height);*/
//	return Point3D();
//}
//
//float Plane::pdf(ShadeRec& sr)
//{
//	return 0.0f;
//}
//
//Vector3D Plane::get_normal(const Point3D& p)
//{
//	return normal;
//}
