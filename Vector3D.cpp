#include "pch.h"
#include "Vector3D.h"

Vector3D::Vector3D()//z轴正向
{
	x = 0.0;
	y = 0.0;
	z = 1.0;
}

Vector3D::~Vector3D()
{

}

Vector3D::Vector3D(const Point3D& p)
{
	x = p.x;
	y = p.y;
	z = p.z;
}

Vector3D::Vector3D(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3D::Vector3D(const Point3D& p0, const Point3D& p1)
{
	x = p1.x - p0.x;
	y = p1.y - p0.y;
	z = p1.z - p0.z;
}

double Vector3D::Mag()//矢量的模
{
	return sqrt(x * x + y * y + z * z);
}

Vector3D Vector3D::Normalize()//归一化到单位矢量
{
	Vector3D vector;
	double Mag = sqrt(x * x + y * y + z * z);
	if (fabs(Mag) < 1e-6)
		Mag = 1.0;
	vector.x = x / Mag;
	vector.y = y / Mag;
	vector.z = z / Mag;
	return vector;
}

Vector3D operator +(const Vector3D& v0, const Vector3D& v1)//矢量的和
{
	Vector3D vector;
	vector.x = v0.x + v1.x;
	vector.y = v0.y + v1.y;
	vector.z = v0.z + v1.z;
	return vector;
}

Vector3D operator -(const Vector3D& v0, const Vector3D& v1)//矢量的差
{
	Vector3D vector;
	vector.x = v0.x - v1.x;
	vector.y = v0.y - v1.y;
	vector.z = v0.z - v1.z;
	return vector;
}

Vector3D operator *(const Vector3D& v, double k)//矢量和常量的积
{
	Vector3D vector;
	vector.x = v.x * k;
	vector.y = v.y * k;
	vector.z = v.z * k;
	return vector;
}

Vector3D operator *(double k, const Vector3D& v)//矢量和常量的积
{
	Vector3D vector;
	vector.x = v.x * k;
	vector.y = v.y * k;
	vector.z = v.z * k;
	return vector;
}

Vector3D operator /(const Vector3D& v, double k)//矢量数除
{
	if (fabs(k) < 1e-6)
		k = 1.0;
	Vector3D vector;
	vector.x = v.x / k;
	vector.y = v.y / k;
	vector.z = v.z / k;
	return vector;
}

Vector3D operator +=(Vector3D& v0, Vector3D& v1)//+=运算符重载
{
	v0.x = v0.x + v1.x;
	v0.y = v0.y + v1.y;
	v0.z = v0.z + v1.z;
	return v1;
}

Vector3D operator -=(Vector3D& v0, Vector3D& v1)//-=运算符重载
{
	v0.x = v0.x - v1.x;
	v0.y = v0.y - v1.y;
	v0.z = v0.z - v1.z;
	return v0;
}

Vector3D operator *=(Vector3D& v0, Vector3D& v1)//*=运算符重载
{
	v0.x = v0.x * v1.x;
	v0.y = v0.y * v1.y;
	v0.z = v0.z * v1.z;
	return v0;
}

Vector3D operator /=(Vector3D& v, double k)///=运算符重载
{
	v.x = v.x / k;
	v.y = v.y / k;
	v.z = v.z / k;
	return v;
}

double Dot(const Vector3D& v0, const Vector3D& v1)//矢量的点积
{
	return(v0.x * v1.x + v0.y * v1.y + v0.z * v1.z);
}

Vector3D Cross(const Vector3D& v0, const Vector3D& v1)//矢量的叉积
{
	Vector3D vector;
	vector.x = v0.y * v1.z - v0.z * v1.y;
	vector.y = v0.z * v1.x - v0.x * v1.z;
	vector.z = v0.x * v1.y - v0.y * v1.x;
	return vector;
}
