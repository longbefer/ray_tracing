#include "pch.h"
#include "World.h"
#include "Ray.h"
#include "ShadeRec.h"
#include "Camera.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "Triangle.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Torus.h"
#include "Cylinder.h"
#include "TeaPot.h"
World::World(void)
{
	Width = Height = 600;
	//eye = Point3D(0.0, 0.0, 600.0);//视点
	pDC = NULL;
	background_color = black;
	vp = ViewPlane();
	tracer_ptr = NULL;
	ambient_ptr = NULL;
	camera_ptr = NULL;
	img = NULL;
}
World::~World(void)
{
	delete tracer_ptr;
	tracer_ptr = NULL;
	delete ambient_ptr;
	ambient_ptr = NULL;
	delete camera_ptr;
	camera_ptr = NULL;
	//删除物体
	int size = (int)objects.size();
	for (int i = 0; i < size; i++)
		if (objects[i]) {
			delete objects[i];
			objects[i] = NULL;
		}
	objects.erase(objects.begin(), objects.end());
	objects.shrink_to_fit();
	//删除光照
	int size1 = (int)lights.size();
	for (int i = 0; i < size1; i++)
		if (lights[i]) {
			delete lights[i];
			lights[i] = NULL;
		}
	lights.erase(lights.begin(), lights.end());
	lights.shrink_to_fit();
	delete vp.sampler_ptr; vp.sampler_ptr = NULL;
}

ShadeRec World::hit_objects(const Ray& ray) 
{
	ShadeRec sr(*this);
	float t;
	Vector3D normal;
	Point3D hitPoint;
	//Material* material_temp;
	float tmin = 1e20f;
	int num_objects = (int)objects.size();

	for (int j = 0; j < num_objects;j++)
		if (objects[j]->hit(ray, t, sr) && (t < tmin)) {
			sr.hit_an_object = true;
			tmin = t;
			normal = sr.normal;
			hitPoint = sr.hit_point;
			{
				//material gird set
				if (!objects[j]->is_grid)//no set gird
					sr.material_ptr = objects[j]->get_material();
				else if (((int)floor(sr.hit_point.x / 200) + (int)floor(sr.hit_point.y / 200) + (int)floor(sr.hit_point.z / 200)) % 2 == 0)
					sr.material_ptr = objects[j]->get_material();
				else sr.material_ptr = objects[j]->get_grid_material();
			}
			sr.img = objects[j]->get_img();
			//material_temp
		}
	if (sr.hit_an_object) {
		sr.t = tmin;
		sr.normal = normal;
		sr.hit_point = hitPoint;
	}

	return sr;
}

void World::render_world(void)
{
	//camera_ptr->set_eye(Point3D(0, 18, -20.5));
	//camera_ptr->set_lookat(Point3D(0, 0.5, 0));
	//camera_ptr->computer_uvw();
	camera_ptr->render_scene(*this);

}

RGBColor World::max_to_one(const RGBColor& c) const
{
	double max_value = max(c.R, max(c.G, c.B));
	if (max_value > 1.0)
		return c / max_value;
	else return c;
}

RGBColor World::clamp_to_color(const RGBColor& c) const
{
	RGBColor color = c;
	if (c.R > 1.0 || c.G > 1.0 || c.B > 1.0) {
		color.R = 1.0; color.G = 1.0; color.B = 1.0;
	}
	return color;
}

void World::display_pixel(const int row, const int column, const RGBColor& pixel_color) const//显示图片
{
	RGBColor mapped_color;
	
	if (vp.show_out_of_gamut)
		mapped_color = clamp_to_color(pixel_color);
	else mapped_color = max_to_one(pixel_color);
	//RGBColor clr = max_to_one(pixel_color);
	if (vp.gamma != 1.0f)
		mapped_color = mapped_color.pow_color(vp.inv_gamma);
	pDC->SetPixel(column, vp.hres - row, RGB(255 * mapped_color.R, 255 * mapped_color.G, 255 * mapped_color.B));
}


void World::bulid(void)
{
	//设置视区
	vp.hres = Width;
	vp.vres = Height;
	vp.num_samples = 16;//为1时有较多噪点，建议1以上，当为1时，采用均匀采样
	vp.s = 1.0;
	vp.gamma = 1.0f;
	vp.inv_gamma = 1 / vp.gamma;
	vp.show_out_of_gamut = false;
	vp.max_depth = 3;
	vp.set_sampler(new MultiJittered(vp.num_samples));

	//设置背景颜色
	background_color = black;

	//创建光线跟踪器
	tracer_ptr = new Whitted(this);

	//设置world的默认环境光
	Ambient* ambient1_ptr = new Ambient;
	ambient1_ptr->set_radiance(1.0f);
	ambient1_ptr->set_color(white);
	ambient_ptr = ambient1_ptr;//set ambient

	//设置默认视图
	Pinhole* pinhole_ptr = new Pinhole; // about 30s
	//ThreadCamera* pinhole_ptr = new ThreadCamera(); // about 36s and more
	pinhole_ptr->set_eye(Point3D(0, 300, 100));
	pinhole_ptr->set_lookat(Point3D(0, 0, 0));
	pinhole_ptr->set_view_distance(850.0);
	pinhole_ptr->computer_uvw();
	camera_ptr = pinhole_ptr;//set camera

	//设置环境中的光源
	PointLight* light_ptr = new PointLight;
	light_ptr->set_location(Point3D(-100, 100, 50));
	light_ptr->set_color(RGBColor(1.0, 1.0, 1.0));
	light_ptr->set_radiance(5.5);
	light_ptr->set_shadows(true);

	//PointLight* light_ptr1 = new PointLight;
	//light_ptr1->set_location(Point3D(0, 0, 500));
	//light_ptr1->set_color(RGBColor(1.0, 1.0, 1.0));
	//light_ptr1->set_radiance(5.5);
	//light_ptr1->set_shadows(true);

	lights.push_back(light_ptr);//set light
	//lights.push_back(light_ptr1);

	//设置物体材质
	//Matte* matte_ptr1 = new Matte;
	//matte_ptr1->set_ka(0.25f);
	//matte_ptr1->set_kd(0.35f);
	////matte_ptr1->set_cd(RGBColor(1, 1, 0));
	//matte_ptr1->set_ambient(RGBColor(0.8, 0.8, 0.9));
	//matte_ptr1->set_diffuse(RGBColor(0.8, 0.8, 0.9));

	Phong* phong_ptr1 = new Phong;
	phong_ptr1->set_ka(0.3f);
	phong_ptr1->set_kd(0.3f);
	phong_ptr1->set_ks(0.2f);
	phong_ptr1->set_ambient(RGBColor(0.8, 0.13, 0.12));
	phong_ptr1->set_diffuse(RGBColor(0.8, 0.13, 0.13));
	phong_ptr1->set_specular(RGBColor(0.6, 0.23, 0.3));
	phong_ptr1->set_specular_e(3);//set_exp

	Reflective* reflective_ptr01 = new Reflective;
	reflective_ptr01->set_ka(0.25f);
	reflective_ptr01->set_kd(0.5f);
	reflective_ptr01->set_ks(0.15f);
	reflective_ptr01->set_ambient(RGBColor(0.8, 0.8, 0.8));
	reflective_ptr01->set_diffuse(RGBColor(0.8, 0.7, 0.7));
	reflective_ptr01->set_specular(RGBColor(0.8, 0.8, 0.8));
	reflective_ptr01->set_specular_e(100);
	reflective_ptr01->set_kr(0.15f);
	reflective_ptr01->set_reflective(white);

	Reflective* reflective_ptr02 = new Reflective;
	reflective_ptr02->set_ka(0.25f);
	reflective_ptr02->set_kd(0.5f);
	reflective_ptr02->set_ks(0.15f);
	reflective_ptr02->set_ambient(RGBColor(0.3, 0.3, 0.2));
	reflective_ptr02->set_diffuse(RGBColor(0.3, 0.3, 0.3));
	reflective_ptr02->set_specular(RGBColor(0.4, 0.3, 0.3));
	reflective_ptr02->set_specular_e(100);
	reflective_ptr02->set_kr(0.15f);
	reflective_ptr02->set_reflective(white);

	Reflective* reflective_ptr1 = new Reflective;
	reflective_ptr1->set_ka(0.25f);
	reflective_ptr1->set_kd(0.5f);
	reflective_ptr1->set_ks(1.0f);
	reflective_ptr1->set_ambient(RGBColor(0.8, 0.13, 0.12));
	reflective_ptr1->set_diffuse(RGBColor(0.8, 0.13, 0.13));
	reflective_ptr1->set_specular(RGBColor(0.6, 0.23, 0.3));
	reflective_ptr1->set_specular_e(100);
	reflective_ptr1->set_kr(0.50f);
	reflective_ptr1->set_reflective(white);

	Reflective* reflective_ptr2 = new Reflective;
	reflective_ptr2->set_ka(0.25f);
	reflective_ptr2->set_kd(0.5f);
	reflective_ptr2->set_ks(1.0f);
	reflective_ptr2->set_ambient(RGBColor(0.8, 0.13, 0.12));
	reflective_ptr2->set_diffuse(RGBColor(0.8, 0.13, 0.13));
	reflective_ptr2->set_specular(RGBColor(0.6, 0.23, 0.3));
	reflective_ptr2->set_specular_e(100);
	reflective_ptr2->set_kr(1.0f);
	reflective_ptr2->set_reflective(white);

	//设置物体
	//TeaPot* teapot_ptr = new TeaPot(Point3D(0,-30,0));
	//teapot_ptr->SetRecursiveNumber(3);//垃圾三次递归
	//teapot_ptr->SetMultiple(20);
	//teapot_ptr->set_material(reflective_ptr1);
	//teapot_ptr->set_img(NULL);//没有纹理可以设成NULL，目前纹理仅适用于茶壶，其他物体还未设置uv，故没有纹理
	Sphere* sphere = new Sphere(Point3D(-30, 0, 0), 50);
	sphere->set_material(reflective_ptr1);
	sphere->set_img(NULL);

	//Torus* torus_ptr1 = new Torus(Point3D(60, -40, 30), 10, 50);
	//torus_ptr1->set_material(reflective_ptr2);
	Cylinder* cylinder_ptr = new Cylinder(Point3D(0, 20, 0), Point3D(0, -40, 0), 50);
	cylinder_ptr->set_material(phong_ptr1);

	Plane* plane_ptr1 = new Plane(Point3D(0, -50, 0), Vector3D(0, 1, 0));
	plane_ptr1->set_material(reflective_ptr01);
	plane_ptr1->set_grid_material(reflective_ptr02);
	plane_ptr1->set_img(NULL);

	//objects.push_back(sphere);
	//objects.push_back(teapot_ptr);
	objects.push_back(cylinder_ptr);
	objects.push_back(plane_ptr1);
	//objects.push_back(rectangle_ptr2);
	//objects.push_back(rectangle_ptr3);
	//objects.push_back(rectangle_ptr4);
	//objects.push_back(rectangle_ptr5);
	//objects.push_back(plane_ptr2);
	//objects.push_back(rectangle_ptr6);
	//SetGeometric();
}

void World::GetWindowsSize(int Width, int Height)
{
	this->Width = Width;
	this->Height = Height;
}