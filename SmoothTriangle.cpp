#include "pch.h"
#include "SmoothTriangle.h"
#include "ShadeRec.h"
SmoothTriangle::SmoothTriangle(void)
{
	this->v0 = PointVector(Point3D(0, 0, 0), Vector3D(0, 1, 0));
	this->v1 = PointVector(Point3D(0, 0, 40), Vector3D(0, 1, 0));
	this->v2 = PointVector(Point3D(-30, 0, 0), Vector3D(0, 1, 0));
	//this->normal = (v0.vector + v1.vector + v2.vector) / 3;
}

SmoothTriangle::SmoothTriangle(PointVector& v0, PointVector& v1, PointVector& v2)
{
	this->v0 = v0;
	this->v1 = v1;
	this->v2 = v2;
	//this->normal = (v0.vector + v1.vector + v2.vector) / 3;
}

bool SmoothTriangle::hit(const Ray& ray, float& tmin, ShadeRec& sr) const
{
	double kEpsilon = 1e-4;
	double a = v0.point.x - v1.point.x, b = v0.point.x - v2.point.x, c = ray.d.x, d = v0.point.x - ray.o.x;
	double e = v0.point.y - v1.point.y, f = v0.point.y - v2.point.y, g = ray.d.y, h = v0.point.y - ray.o.y;
	double i = v0.point.z - v1.point.z, j = v0.point.z - v2.point.z, k = ray.d.z, l = v0.point.z - ray.o.z;

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

	sr.normal = ((1 - beta - gamma) * v0.vector + beta * v1.vector + gamma * v2.vector).Normalize();
	
	//纹理
	CT2 uv;
	uv = (1 - beta - gamma) * v0.t + beta * v1.t + gamma * v2.t;//这个uv是0-1之间的，需要在绘制时稍微转换以下哦
	sr.u = uv.u; sr.v = uv.v;

	//sr.normal = normal;

	//判断是否可见
	if (Dot(-1 * ray.d, sr.normal) <= 0.0)
		sr.normal = -1 * sr.normal;//如果视线（主光线）与面片矢量小于90°，即该面矢量应该反向

	Vector3D distance = tmin * ray.d;
	sr.hit_point = ray.o + Point3D(distance.x, distance.y, distance.z);
	return true;

}

bool SmoothTriangle::shadow_hit(const Ray& ray, float& tmin) const
{
	double kEpsilon = 1e-4;
	double a = v0.point.x - v1.point.x, b = v0.point.x - v2.point.x, c = ray.d.x, d = v0.point.x - ray.o.x;
	double e = v0.point.y - v1.point.y, f = v0.point.y - v2.point.y, g = ray.d.y, h = v0.point.y - ray.o.y;
	double i = v0.point.z - v1.point.z, j = v0.point.z - v2.point.z, k = ray.d.z, l = v0.point.z - ray.o.z;

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
