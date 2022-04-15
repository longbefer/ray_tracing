#include "pch.h"
#include "Cylinder.h"
#include "ShadeRec.h"
#include "Box.h"
Cylinder::Cylinder(void)
{
	this->bottom_point = Point3D(0, 0, 0);
	this->top_point = Point3D(0, 100, 0);
	this->radius = 50.0f;
}

Cylinder::Cylinder(Point3D& top_point, Point3D& bottom_point, const double radius)
{
	this->bottom_point = bottom_point;
	this->top_point = top_point;
	this->radius = radius;
}

Cylinder::~Cylinder(void)
{
}
//有问题，待修复
bool Cylinder::hit(const Ray& ray, float& tmin, ShadeRec& sr) const
{
	//通过包围盒判定是否击中，减少计算量
	//Ray temp_ray = Ray(ray.o, ray.d);
	//float t = 1e20f;
	//Point3D right_top = Point3D(top_point.x + 2 * radius, top_point.y, top_point.z + 2 * radius);
	//Point3D left_bottom = Point3D(bottom_point.x - 2 * radius, bottom_point.y, bottom_point.z - 2 * radius);
	//Box* box = new Box(right_top, left_bottom);
	//if (!(box->shadow_hit(temp_ray, t))) {
	//	delete box; box = NULL;
	//	return false;
	//}
	//delete box; box = NULL;

	Point3D newp = ray.o/* - (top_point + bottom_point) / 2*/;
	Ray rays = Ray(newp, ray.d);

	double a = rays.d.x * rays.d.x + rays.d.z * rays.d.z;
	double b = 2 * (rays.o.x * rays.d.x + rays.o.z * rays.d.z);
	double c = rays.o.x * rays.o.x + rays.o.z * rays.o.z - radius * radius;
	double det = (b * b - 4 * a * c);
	const double eps = 1e-4;
	if (det < 0)
		return false;
	double dets = sqrt(det);
	double denom = 2 * a;
	double res = ((-1 * b - dets) / denom);
	if (res > eps) {
		tmin = (float)res;
		Vector3D distance = tmin * rays.d;
		Point3D hit_point= rays.o + Point3D(distance.x, distance.y, distance.z);
		if (hit_point.y > top_point.y || hit_point.y < bottom_point.y)
			return false;
		sr.hit_point = hit_point;
		Point3D center = Point3D(sr.hit_point.x / radius, 0, sr.hit_point.z / radius);
		sr.normal = Vector3D(center, sr.hit_point).Normalize();//矢量
		sr.color = this->color;//先默认为单一颜色
		return true;
	}
	res = ((-1 * b + dets) / denom);
	if (res > eps) {
		tmin = (float)res;
		Vector3D distance = tmin * rays.d;
		Point3D hit_point = rays.o + Point3D(distance.x, distance.y, distance.z);
		if (hit_point.y > top_point.y || hit_point.y < bottom_point.y)
			return false;
		sr.hit_point = hit_point;
		Point3D center = Point3D(sr.hit_point.x / radius, 0, sr.hit_point.z / radius);
		sr.normal = Vector3D(center, sr.hit_point).Normalize();//矢量
		sr.color = this->color;//先默认为单一颜色
		return true;
	}
	return false;
}

bool Cylinder::shadow_hit(const Ray& ray, float& tmin) const
{
	//通过包围盒判定是否击中，减少计算量
	Ray temp_ray = Ray(ray.o, ray.d);
	float t = 1e20f;
	Point3D right_top = Point3D(top_point.x + 2 * radius, top_point.y, top_point.z + 2 * radius);
	Point3D left_bottom = Point3D(bottom_point.x - 2 * radius, bottom_point.y, bottom_point.z - 2 * radius);
	Box* box = new Box(right_top, left_bottom);
	if (!(box->shadow_hit(temp_ray, t))) {
		delete box; box = NULL;
		return false;
	}
	delete box; box = NULL;

	Point3D newp = ray.o - (top_point + bottom_point) / 2;
	Ray rays = Ray(newp, ray.d);

	double a = rays.d.x * rays.d.x + rays.d.z * rays.d.z;
	double b = 2 * (rays.o.x * rays.d.x + rays.o.z * rays.d.z);
	double c = rays.o.x * rays.o.x + rays.o.z * rays.o.z - radius * radius;
	double det = (b * b - 4 * a * c);
	const double eps = 1e-4;
	if (det < 0)
		return false;
	double dets = sqrt(det);
	double denom = 2 * a;
	double res = ((-1 * b - dets) / denom);
	if (res > eps) {
		tmin = (float)res;
		return true;
	}
	res = ((-1 * b + dets) / denom);
	if (res > eps) {
		tmin = (float)res;
		return true;
	}
	return false;
}
