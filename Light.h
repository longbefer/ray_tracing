#pragma once
#include "Ray.h"
class ShadeRec;
class Light
{
public:
	Light(void);
	virtual Vector3D
		get_direction(ShadeRec& sr) = 0;//光源到点的距离
	virtual bool in_shadow(const Ray& ray, const ShadeRec& sr)const = 0;
	virtual RGBColor L(ShadeRec& sr) = 0;//辐射度*颜色
	void set_shadows(bool shadows);//是否添加阴影
	bool casts_shadows();//是否添加阴影
protected:
	bool shadows;
};

class Ambient : public Light
{
public:
	inline Ambient(void):
	Light(),
	ls(1.0),
	color(white){}

	virtual Vector3D
		get_direction(ShadeRec& sr);

	virtual RGBColor
		L(ShadeRec& sr);
public:
	void set_radiance(float ls) { this->ls = ls; };
	void set_color(RGBColor color) { this->color = color; };
	bool in_shadow(const Ray& ray, const ShadeRec& sr)const;
private:
	float ls;
	RGBColor color;
};

class PointLight : public Light {
public:
	virtual Vector3D
		get_direction(ShadeRec& sr);
	virtual RGBColor
		L(ShadeRec& sr);
public:
	void set_location(Point3D location);
	void set_radiance(float ls);
	void set_color(RGBColor color);
	bool in_shadow(const Ray& ray, const ShadeRec& sr)const;
private:
	float ls;
	RGBColor color;
	Point3D location;
};

//class AreaLighting :public Light {//区域光源可见
//public:
//	virtual Vector3D get_direction(ShadeRec& sr);
//	virtual bool in_shadow(const Ray& ray, const ShadeRec& sr)const;
//	virtual RGBColor L(ShadeRec& sr);
//	virtual float G(const ShadeRec& sr)const;
//	virtual float pdf(const ShadeRec& sr)const;
//private:
//	GeometricObject* object_ptr;
//	Material* material_ptr;
//	Point3D sample_point;
//	Vector3D light_normal;
//	Vector3D wi;
//};
