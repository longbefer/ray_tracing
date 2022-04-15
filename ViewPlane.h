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
	int hres;//ˮƽ
	int vres;//��ֱ
	float s;//���ش�С
	int num_samples;//������=n*n�����ڲ�ͬλ�õ��ڣ��������s��һ�������ڶ��ȡֵ��֮��ȡƽ��ֵ�����ԱȽ�׼ȷ�ķ�Ӧ��ɫ
	//�ݶ�
	float gamma;
	float inv_gamma;
	int max_depth;//������
	//�ж���ɫ�Ƿ����
	bool show_out_of_gamut;

	Sampler* sampler_ptr;
};

