#include "pch.h"
#include "Light.h"
#include "ShadeRec.h"
Vector3D Ambient::get_direction(ShadeRec& sr)
{
	return Vector3D(0,0,0);
}

RGBColor Ambient::L(ShadeRec& sr)
{
	return RGBColor(ls*color);
}

bool Ambient::in_shadow(const Ray& ray, const ShadeRec& sr) const
{
	return false;
}

Vector3D PointLight::get_direction(ShadeRec& sr)
{
	//Point3D direction_p = location - sr.hit_point;
	//Vector3D dir(direction_p.x, direction_p.y, direction_p.z);
	//dir = dir.Normalize();
	//return dir;

	float r = 3.0;
	Point3D new_location;
	new_location.x = location.x + r * (2 * rand_float() - 1.0);
	new_location.y = location.y + r * (2 * rand_float() - 1.0);
	new_location.z = location.z + r * (2 * rand_float() - 1.0);

	return Vector3D(new_location - sr.hit_point).Normalize();;

}

RGBColor PointLight::L(ShadeRec& sr)
{
	return RGBColor(ls*color);
}

void PointLight::set_location(Point3D location)
{
	this->location = location;
}

void PointLight::set_radiance(float ls)
{
	this->ls = ls;
}

void PointLight::set_color(RGBColor color)
{
	this->color = color;
}

bool PointLight::in_shadow(const Ray& ray, const ShadeRec& sr) const
{
	float t;
	int num_object = (int)sr.w.objects.size();
	float d = (float)Vector3D(location, ray.o).Mag();//光照光线到点的位置

	for (int j = 0; j < num_object; j++)
		if (sr.w.objects[j]->shadow_hit(ray, t) && (t < d))
			return true;
	return false;
}

Light::Light(void)
{
	this->shadows = true;
}

void Light::set_shadows(bool shadows)
{
	this->shadows = shadows;
}

bool Light::casts_shadows()
{
	return shadows;
	//return true;
}

//Vector3D AreaLighting::get_direction(ShadeRec& sr)
//{
//	sample_point = object_ptr->sample();
//	light_normal = object_ptr->get_normal(sample_point);
//
//	wi = sample_point - sr.hit_point;
//	wi.Normalize();
//	return wi;
//}
//
//bool AreaLighting::in_shadow(const Ray& ray, const ShadeRec& sr) const
//{
//	float t;
//	int num_objects = sr.w.objects.size();
//	float ts = (float)Dot((sample_point - ray.o), ray.d);
//
//	for (int j = 0; j < num_objects; j++)
//		if (sr.w.objects[j]->shadow_hit(ray, t) && (t < ts))
//			return true;
//
//	return false;
//}
//
//RGBColor AreaLighting::L(ShadeRec& sr)
//{
//	float ndotd = Dot(-1 * light_normal, wi);
//
//	if (ndotd > 0.0)
//		return material_ptr->get_Le(sr);
//	else return black;
//}
//
//float AreaLighting::G(const ShadeRec& sr) const
//{
//	float ndotd = Dot(-1 * light_normal, wi);
//	float d2 = (float)Vector3D(sample_point, sr.hit_point).Mag();
//	return ndotd / d2;
//}
//
//float AreaLighting::pdf(const ShadeRec& sr) const
//{
//	return object_ptr->pdf(sr);
//}
