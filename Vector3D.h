#pragma once
#include "Point3D.h"
#include "T2.h"
class Vector3D
{
public:
	Vector3D(void);
	virtual ~Vector3D(void);
	Vector3D(double x, double y, double z);
	Vector3D(const Point3D&);
	Vector3D(const Point3D&, const Point3D&);
	double Mag();//矢量的模
	Vector3D Normalize();//单位矢量
	friend Vector3D operator +(const Vector3D&, const Vector3D&);//运算符重载
	friend Vector3D operator -(const Vector3D&, const Vector3D&);
	friend Vector3D operator *(const Vector3D&, double);
	friend Vector3D operator *(double, const Vector3D&);
	friend Vector3D operator /(const Vector3D&, double);
	friend Vector3D operator+=(Vector3D&, Vector3D&);
	friend Vector3D operator-=(Vector3D&, Vector3D&);
	friend Vector3D operator*=(Vector3D&, Vector3D&);
	friend Vector3D operator/=(Vector3D&, double);
	friend double  Dot(const Vector3D&, const Vector3D&);  //矢量点积
	friend Vector3D Cross(const Vector3D&, const Vector3D&);//矢量叉积
public:
	double x, y, z;
};




class PointVector {//点矢量
public:
	PointVector(void) :point(Point3D()), vector(Vector3D()) {};
	PointVector(Point3D p, Vector3D v) :point(p), vector(v) {};
	~PointVector(void) {};
public:
	Point3D point;
	Vector3D vector;
	CT2 t;
};