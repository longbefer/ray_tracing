#include "pch.h"
#include "Rectangle.h"
#include "ShadeRec.h"
using namespace myRect;
Rectangle::Rectangle(void)
{
	point = Point3D(0, 0, 0);
	width_vector = Vector3D(1, 0, 0);
	height_vector = Vector3D(0, 1, 0);
}

Rectangle::Rectangle(Point3D& point, Vector3D& width_vector, Vector3D& height_vector)
{
	this->point = point;
	this->width_vector = width_vector;
	this->height_vector = height_vector;
}

Rectangle::~Rectangle(void)
{
}

bool Rectangle::hit(const Ray& ray, float& tmin, ShadeRec& sr) const
{
	if (!shadow_hit(ray, tmin))
		return false;
	Vector3D normal = Cross(width_vector, height_vector).Normalize();
	Point3D p = ray.o + Point3D(tmin * ray.d.x, tmin * ray.d.y, tmin * ray.d.z);
	sr.normal = normal;
	if (Dot(-1 * ray.d, sr.normal) <= 0.0)
		sr.normal = -1 * sr.normal;//如果视线（主光线）与面片矢量小于90°，即该面矢量应该反向
	sr.hit_point = p;
	sr.color = color;
	return true;
}

bool Rectangle::shadow_hit(const Ray& ray, float& tmin) const
{
	double kEpsilon = 1e-4;
	Vector3D normal = Cross(width_vector, height_vector);
	float t = (float)(Dot((point - ray.o), normal) / Dot(ray.d, normal));
	if (t <= kEpsilon)
		return false;
	//以上判断是否处于矩形所在的平面内

	Point3D p = ray.o + Point3D(t * ray.d.x, t * ray.d.y, t * ray.d.z);
	Vector3D d = Vector3D(p - point);

	Vector3D temp_width = width_vector;
	float width = (float)temp_width.Mag();
	Vector3D temp_height = height_vector;
	float height = (float)temp_height.Mag();
	
	float ddota = (float)Dot(d, width_vector);//d向量投影到width_vector向量上
	if (ddota<0.0f || ddota>width* width)
		return false;
	float ddotb = (float)Dot(d, height_vector);//d向量投影到height_vector向量上
	if (ddotb<0.0f || ddotb>height* height)
		return false;

	tmin = t;
	return true;
}
