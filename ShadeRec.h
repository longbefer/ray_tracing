#pragma once
#include "Vector3D.h"
#include "World.h"
class ShadeRec//ShadeREC用于保存与物体相交时的数据
{
public:
	ShadeRec(World& wr)
		:hit_an_object(false),
		hit_point(),
		depth(0),t(0.0),
		material_ptr(NULL),
		//local_hit_point(),
		normal(),dir(),ray(),
		color(wr.background_color),u(0),v(0),
		w(wr),img(NULL) {}
	ShadeRec(const ShadeRec& sr);
	~ShadeRec(void);

public:
	bool hit_an_object;//是否击中物体
	//Point3D local_hit_point;
	Point3D hit_point;
	Vector3D normal;//击中点的法向量
	RGBColor color;//击中点颜色

	//
	Ray ray;
	int depth;
	Vector3D dir;//for area lights
	float t;
	//
	Material* material_ptr;

	//纹理贴图
	double u, v;
	Texture* img;


	World& w;
};

