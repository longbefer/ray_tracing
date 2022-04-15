#pragma once
#include "Vector3D.h"
#include "World.h"
class ShadeRec//ShadeREC���ڱ����������ཻʱ������
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
	bool hit_an_object;//�Ƿ��������
	//Point3D local_hit_point;
	Point3D hit_point;
	Vector3D normal;//���е�ķ�����
	RGBColor color;//���е���ɫ

	//
	Ray ray;
	int depth;
	Vector3D dir;//for area lights
	float t;
	//
	Material* material_ptr;

	//������ͼ
	double u, v;
	Texture* img;


	World& w;
};

