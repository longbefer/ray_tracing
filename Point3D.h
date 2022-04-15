#pragma once
#include "RGBColor.h"
#include <cmath>

class Point3D
{
public:
	Point3D(void);
	Point3D(double x, double y);
	Point3D(double x, double y, double z);
	virtual ~Point3D(void);
	Point3D(double x, double y, double z, RGBColor c);
	friend Point3D operator +(const Point3D&, const Point3D&);//‘ÀÀ„∑˚÷ÿ‘ÿ
	friend Point3D operator -(const Point3D&, const Point3D&);
	friend Point3D operator *(const Point3D&, const double);
	friend Point3D operator *(const double, const Point3D&);
	friend Point3D operator /(const Point3D&, double);
	friend Point3D operator+=(Point3D&, Point3D&);
	friend Point3D operator-=(Point3D&, Point3D&);
	friend Point3D operator*=(Point3D&, double);
	friend Point3D operator/=(Point3D&, double);
public:
	double x, y, z;
	double w;
	RGBColor color;
};
typedef Point3D CP3;
