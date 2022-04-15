#pragma once
#include "ViewPlane.h"
#include "Tracer.h"
#include "GeometricObject.h"
#include "Light.h"
#include "Material.h"
#include "Camera.h"
#include "Texture.h"
#include "T2.h"
class ShadeRec;
class Camera;
class World
{
public:
	World(void);
	~World(void);
	void bulid(void);
	
	ShadeRec hit_objects(const Ray& ray);

	//ShadeRec hit_objects(const Ray& ray, float& tmin)const;

	//void render_perspective(void)const;

	//void render_sence(void)const;
	void render_world(void);

	RGBColor max_to_one(const RGBColor& c)const;
	RGBColor clamp_to_color(const RGBColor& c)const;
	void display_pixel(const int row, const int column, const RGBColor& pixel_color) const;

	//void SetGeometric();//���ü�������
	//void SetLightORMaterial();//���ù��ղ���

	void GetWindowsSize(int Width, int Height);
public:
	ViewPlane vp;//��ƽ�棬��¼���ڴ�С

	Camera* camera_ptr;
	
	Tracer* tracer_ptr;
	
	RGBColor background_color;//����ɫ
	
	Light* ambient_ptr;//����Ĭ�ϵĻ�����

	//����ͼ��
	//1������pDC;
	//2��������
	CDC* pDC;
	//Sphere sp1;

	//����ͼ��2�����������Ⱦ
	std::vector<GeometricObject*> objects;//ͼ�ζ���
	std::vector<Light*> lights;//���ն���

	Texture* img;

private:
	int Width, Height;
	//Point3D eye;//�ӵ�
};

