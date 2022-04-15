#include "pch.h"
#include "GeometricObject.h"

GeometricObject::~GeometricObject(void)
{
	if (p_Material)
	{
		delete p_Material;
		p_Material = NULL;
	}
	if (p_gird_Material) {
		delete p_gird_Material;
		p_gird_Material = NULL;
	}
}

bool GeometricObject::shadow_hit(const Ray& ray, float& tmin)const
{
	return false;
}

//void GeometricObject::set_sampler(Sampler* sampler)
//{
//	this->sampler_ptr = sampler;
//}

Material* GeometricObject::get_material()
{
	return (this->p_Material);
}

Material* GeometricObject::get_grid_material(void)
{
	return this->p_gird_Material;
}

Texture* GeometricObject::get_img(void)
{
	return this->img;
}
