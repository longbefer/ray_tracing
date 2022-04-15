#pragma once
#include "Ray.h"
#include "Material.h"
#include "Sampler.h"
#include "Texture.h"
class ShadeRec;
class GeometricObject
{
public:
	~GeometricObject(void);
public:
	virtual bool hit(const Ray& ray, float& tmin, ShadeRec& sr)const = 0;//�Ƿ��й���
	virtual bool shadow_hit(const Ray& ray, float& tmin)const = 0;//�Ƿ�����Ӱ֮��

	//��������
	void set_grid_material(Material* ptr_grid) { this->is_grid = true; this->p_gird_Material = ptr_grid; };
	bool is_grid = false;
	//
	//virtual bool in_object(Point3D& point)const = 0;//���Ƿ�������

	//���󷢹�
	//void set_sampler(Sampler* sampler);
	//virtual Point3D sample(void) = 0;
	//virtual float pdf(ShadeRec& sr) = 0;
	//virtual Vector3D get_normal(const Point3D& p) = 0;
	void set_img(Texture* img) { this->img = img; };
	void set_material(Material* p_Material) { this->p_Material = p_Material; };
	Material* get_material(void);

	Material* get_grid_material(void);

	Texture* get_img(void);
protected:
	RGBColor color;
	Material* p_Material;

	Material* p_gird_Material = NULL;

	//������
	Texture* img = NULL;

	//Sampler* sampler_ptr;
};

