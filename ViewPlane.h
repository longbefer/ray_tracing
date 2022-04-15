#pragma once
#include "Sampler.h"
class ViewPlane
{
public:
	ViewPlane(void);
	ViewPlane(const ViewPlane& vp);
	virtual ~ViewPlane(void);
public:
	void set_sampler(Sampler* sp);
	void set_samples(const int n);
public:
	int hres;//水平
	int vres;//垂直
	float s;//像素大小
	int num_samples;//采样率=n*n，即在不同位置但在（即上面的s）一个像素内多次取值，之后取平均值，可以比较准确的反应颜色
	//暂定
	float gamma;
	float inv_gamma;
	int max_depth;//最大深度
	//判断颜色是否溢出
	bool show_out_of_gamut;

	Sampler* sampler_ptr;
};

