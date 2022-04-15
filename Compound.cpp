#include "pch.h"
#include "Compound.h"
#include "ShadeRec.h"
Compound::Compound()
{
}

Compound::~Compound()
{
	//É¾³ýÎïÌå
	int size = (int)objects.size();
	for (int i = 0; i < size; i++)
		if (objects[i]) {
			delete objects[i];
			objects[i] = NULL;
		}
	objects.erase(objects.begin(), objects.end());
	objects.shrink_to_fit();
}

void Compound::set_material(Material* material_ptr)
{
	int num_objects = (int)objects.size();
	for (int j = 0; j < num_objects; j++)
		objects[j]->set_material(material_ptr);
}

void Compound::add_object(GeometricObject* object_ptr)
{
	objects.push_back(object_ptr);
}

bool Compound::hit(const Ray& ray, double& tmin, ShadeRec& sr)const
{
	float t;
	Vector3D normal;
	Point3D local_hit_point;
	bool hit = false;
	tmin = 1e20;
	int num_objects = (int)objects.size();
	for (int j = 0; j < num_objects; j++)
		if (objects[j]->hit(ray, t, sr) && (t < tmin)) {
			hit = true;
			tmin = t;
			normal = sr.normal;
			local_hit_point = sr.hit_point;
			sr.material_ptr = objects[j]->get_material();
		}
	if (hit) {
		sr.t = (float)tmin;
		sr.normal = normal;
		sr.hit_point = local_hit_point;
	}
	return hit;
}

bool Compound::shadow_hit(const Ray& ray, double& tmin) const
{
	float t;
	Vector3D normal;
	Point3D local_hit_point;
	bool hit = false;
	tmin = 1e20;
	int num_objects = (int)objects.size();
	for (int j = 0; j < num_objects; j++)
		if (objects[j]->shadow_hit(ray, t) && (t < tmin)) {
			hit = true;
			tmin = t;
		}
	return hit;
}
