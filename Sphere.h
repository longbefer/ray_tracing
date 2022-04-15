#pragma once
#include "GeometricObject.h"
class Sphere:public GeometricObject
{
public:
	Sphere(void);
	Sphere(Point3D& center, double radius, RGBColor& color=red)
		:center(center),radius(radius),color(color){}

	//Sphere(Point3D& center, double radius, Material* material);

	~Sphere(void);
	bool hit(const Ray& ray, float& tmin, ShadeRec& sr)const;
	bool shadow_hit(const Ray& ray, float& tmin)const;//�Ƿ�����Ӱ֮��

	//�Է���
	//virtual Point3D sample(void);
	//virtual float pdf(ShadeRec& sr);
	//virtual Vector3D get_normal(const Point3D& p);

public:
	Point3D center;//����
	double radius;//�뾶
	RGBColor color;//��ɫ
};

