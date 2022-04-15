#pragma once
#include "Vector3D.h"
class Ray
{
public:
	Ray(void);
	inline Ray(const Point3D& origin, const Vector3D& dir)
		:o(origin), d(dir) {}
	Ray(const Ray& ray);
	~Ray(void);
public:
	Point3D o;//Դ��
	Vector3D d;//����
};

