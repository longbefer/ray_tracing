#pragma once
#include "Ray.h"
class World;
class ShadeRec;
class Tracer
{
public:
	Tracer(void);
	Tracer(World* w_ptr);
	~Tracer(void);
	//virtual ShadeRec trace_ray(const Ray& ray)const;
	virtual RGBColor trace_ray(const Ray& ray, const int depth)const;
	virtual RGBColor trace_ray(const Ray& ray, float& tmin, const int depth)const;
protected:
	World* world_ptr;
};

class Whitted :public Tracer
{
public:
	Whitted(void);
	Whitted(World* w_ptr);
	~Whitted(void);

	virtual RGBColor trace_ray(const Ray& ray, const int depth)const;
	virtual RGBColor trace_ray(const Ray& ray, float& tmin, const int depth)const;
};

