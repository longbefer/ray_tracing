#include "pch.h"
#include "Ray.h"

Ray::Ray(void)
{
	o = Point3D();
	d = Vector3D();
}

Ray::Ray(const Ray& ray)
{
	this->o = ray.o;
	this->d = ray.o;
}

Ray::~Ray(void)
{
}

