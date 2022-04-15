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

	//void SetGeometric();//设置几何物体
	//void SetLightORMaterial();//设置光照材质

	void GetWindowsSize(int Width, int Height);
public:
	ViewPlane vp;//视平面，记录窗口大小

	Camera* camera_ptr;
	
	Tracer* tracer_ptr;
	
	RGBColor background_color;//背景色
	
	Light* ambient_ptr;//世界默认的环境光

	//测试图形
	//1，测试pDC;
	//2，测试球
	CDC* pDC;
	//Sphere sp1;

	//测试图形2，多个对象渲染
	std::vector<GeometricObject*> objects;//图形对象
	std::vector<Light*> lights;//光照对象

	Texture* img;

private:
	int Width, Height;
	//Point3D eye;//视点
};

