#include "pch.h"
#include "Point3D.h"

Point3D::Point3D(void)
{
	this->x = this->y = this->z = 0.0;
	this->w = 1.0;
	this->color = RGBColor();
}

Point3D::Point3D(double x, double y)
{
	this->x = x;
	this->y = y;
	this->z = 0.0;
	this->color = RGBColor();
	this->w = 1;
}

Point3D::Point3D(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = 1;
	this->color = RGBColor();
}

Point3D::~Point3D(void)
{
}

Point3D::Point3D(double x, double y, double z, RGBColor c)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = 1;
	this->color = c;
}

Point3D operator+(const Point3D& p1, const Point3D& p2)
{
	Point3D p;
	p.x = p1.x + p2.x;
	p.y = p1.y + p2.y;
	p.z = p1.z + p2.z;
	return p;
}

Point3D operator-(const Point3D& p1, const Point3D& p2)
{
	Point3D p;
	p.x = p1.x - p2.x;
	p.y = p1.y - p2.y;
	p.z = p1.z - p2.z;
	return p;
}

Point3D operator*(const Point3D& p, const double k)
{
	return Point3D(p.x * k, p.y * k, p.z * k);
}

Point3D operator*(const double k, const Point3D& p)
{
	return Point3D(p.x * k, p.y * k, p.z * k);
}

Point3D operator/(const Point3D& p1, double k)
{
	if (fabs(k) < 1e-6)
		k = 1.0;
	Point3D p;
	p.x = p1.x / k;
	p.y = p1.y / k;
	p.z = p1.z / k;
	return p;
}

Point3D operator+=(Point3D& p1, Point3D& p2)
{
	p1.x = p1.x + p2.x;
	p1.y = p1.y + p2.y;
	p1.z = p1.z + p2.z;
	return p1;
}

Point3D operator-=(Point3D& p1, Point3D& p2)
{
	p1.x = p1.x - p2.x;
	p1.y = p1.y - p2.y;
	p1.z = p1.z - p2.z;
	return p1;
}

Point3D operator*=(Point3D& p1, double k)
{
	p1.x = p1.x * k;
	p1.y = p1.y * k;
	p1.z = p1.z * k;
	return p1;
}

Point3D operator/=(Point3D& p1, double k)
{
	if (fabs(k) < 1e-6)
		k = 1.0;
	p1.x = p1.x / k;
	p1.y = p1.y / k;
	p1.z = p1.z / k;
	return p1;
}
