#pragma once
#include "GeometricObject.h"
namespace myRect {
	class Rectangle :public GeometricObject
	{
	public:
		Rectangle(void);
		Rectangle(Point3D& point, Vector3D& width_vector, Vector3D& height_vector);
		~Rectangle(void);
		bool hit(const Ray& ray, float& tmin, ShadeRec& sr)const;
		bool shadow_hit(const Ray& ray, float& tmin)const;//是否处于阴影之中
	public:
		Point3D point;
		Vector3D width_vector, height_vector;
	};
}

