#pragma once
#include "GeometricObject.h"
class Compound : public GeometricObject//组合体，用来判断组合对象的
{
public:
	Compound();
	~Compound();
public:
	virtual void set_material(Material* material_ptr);
	void add_object(GeometricObject* object_ptr);
	virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr)const;
	virtual bool shadow_hit(const Ray& ray, double& tmin)const;
protected:
	std::vector<GeometricObject*> objects;
};

