#include "pch.h"
#include "Tracer.h"
#include "ShadeRec.h"
Tracer::Tracer(void)
{
	world_ptr = NULL;
}

Tracer::Tracer(World* w_ptr)
{
	this->world_ptr = w_ptr;
}

Tracer::~Tracer(void)
{
}

//ShadeRec Tracer::trace_ray(const Ray& ray)const//NOT USE
//{
//	ShadeRec sr(*world_ptr);
//	float t;
//	float tmin = 1e20f;//返回最小的t值
//	//合适的物体
//	Vector3D normal;
//	RGBColor color;
//	Point3D hitPoint;
//
//	int num_object = world_ptr->objects.size();
//	for (int i = 0; i < num_object; i++) {
//		if (world_ptr->objects[i]->hit(ray, t, sr)&&(t<tmin)) {
//			//记得在几何体中给击中点和向量赋值还有颜色
//			//Vector3D distance = tmin * ray.d;
//			//sr.local_hit_point = ray.o + Point3D(distance.x, distance.y, distance.z);
//			sr.hit_an_object = true;
//			sr.material_ptr = world_ptr->objects[i]->get_material();
//			hitPoint = sr.hit_point;
//			normal = sr.normal;
//			color = sr.color;
//			tmin = t;//改变tmin，检测多个物体
//		}
//		//else return world_ptr->background_color;
//	} 
//	//选择最近的物体点和法向量与颜色
//	if(sr.hit_an_object)
//	{
//		sr.hit_point = hitPoint;
//		sr.normal = normal;
//		sr.color = color;
//	}
//	
//	return sr;
//	//return world_ptr->background_color;//当没有物体时，返回背景色
//}

RGBColor Tracer::trace_ray(const Ray& ray, const int depth) const
{
	ShadeRec sr(world_ptr->hit_objects(ray));
	if (sr.hit_an_object) {
		sr.ray = ray;
		//if(sr.material_ptr)
		return sr.material_ptr->shade(sr);
	}
	else return world_ptr->background_color;
}

RGBColor Tracer::trace_ray(const Ray& ray, float& tmin, const int depth) const
{
	return RGBColor();
}

Whitted::Whitted(void)
{
	world_ptr = NULL;
}

Whitted::Whitted(World* w_ptr)
{
	this->world_ptr = w_ptr;
}

Whitted::~Whitted(void)
{
}

RGBColor Whitted::trace_ray(const Ray& ray, const int depth) const
{
	if (depth > world_ptr->vp.max_depth)
		return world_ptr->background_color;
	ShadeRec sr(world_ptr->hit_objects(ray));
	if (sr.hit_an_object) {
		sr.depth = depth;
		sr.ray = ray;
		return sr.material_ptr->shade(sr);
	}
	else return world_ptr->background_color;
}

RGBColor Whitted::trace_ray(const Ray& ray, float& tmin, const int depth) const
{
	return RGBColor();
}
