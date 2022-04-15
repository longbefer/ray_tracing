#pragma once
#include "GeometricObject.h"
class SmoothTriangle : public GeometricObject//仅测试用途，用于三角形拼成的物体的平滑效果
{
	//利用alpha和beta的插值
public:
	SmoothTriangle(void);//使用方法，需要确定三角形三个点的矢量
	SmoothTriangle(PointVector& v0, PointVector& v1, PointVector& v2);
	bool hit(const Ray& ray, float& tmin, ShadeRec& sr)const;
	bool shadow_hit(const Ray& ray, float& tmin)const;//是否处于阴影之中
public:
	PointVector v0, v1, v2;
	//Vector3D normal;
};

