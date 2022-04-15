#include "pch.h"
#include "Triangle.h"
#include "ShadeRec.h"
Triangle::Triangle(void)
{
	this->v0 = Point3D(0, 0, 0);
	this->v1 = Point3D(0, 0, 40);
	this->v2 = Point3D(-30, 0, 0);
	this->normal = Vector3D(0, 1, 0);
}

Triangle::Triangle(Point3D& v0, Point3D& v1, Point3D& v2)
{
	this->v0 = v0;
	this->v1 = v1;
	this->v2 = v2;
	this->normal = Cross(Vector3D(v2 - v0), Vector3D(v1 - v0)).Normalize();
}

bool Triangle::hit(const Ray& ray, float& tmin, ShadeRec& sr) const
{
	if (!shadow_hit(ray, tmin))
		return false;


	sr.normal = normal;

	//判断是否可见
	if (Dot(1 * ray.d, sr.normal) >= 0.0)
		sr.normal = -1 * sr.normal;//如果视线（主光线）与面片矢量小于90°，即该面矢量应该反向


	Vector3D distance = tmin * ray.d;
	sr.hit_point = ray.o + Point3D(distance.x, distance.y, distance.z);
	return true;

}

bool Triangle::shadow_hit(const Ray& ray, float& tmin) const
{
	double kEpsilon = 1e-4;
	double a = v0.x - v1.x, b = v0.x - v2.x, c = ray.d.x, d = v0.x - ray.o.x;
	double e = v0.y - v1.y, f = v0.y - v2.y, g = ray.d.y, h = v0.y - ray.o.y;
	double i = v0.z - v1.z, j = v0.z - v2.z, k = ray.d.z, l = v0.z - ray.o.z;

	double m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
	double q = g * i - e * k, r = e * l - h * i, s = e * j - f * i;

	double temp = (a * m + b * q + c * s);
	if (temp == 0) return false;

	double inv_denom = 1.0 / temp;

	double e1 = d * m - b * n - c * p;
	double beta = e1 * inv_denom;

	if (beta < 0.0)
		return false;

	double e2 = a * n + d * q + c * r;
	double gamma = e2 * inv_denom;

	if (gamma < 0.0)
		return false;
	if (beta + gamma > 1.0)
		return false;

	double e3 = a * p - b * r + d * s;
	double t = e3 * inv_denom;

	if (t < kEpsilon)
		return false;

	tmin = (float)t;
	return true;
}
