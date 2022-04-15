#include "pch.h"
#include "ViewPlane.h"

ViewPlane::ViewPlane(void)
{
	hres = 400;
	vres = 400;
	s = 1.0;
	num_samples = 16;
	gamma = 1.0f;
	inv_gamma = 1.0f / gamma;
	show_out_of_gamut = false;
	sampler_ptr = NULL;
	max_depth = 3;
}

ViewPlane::ViewPlane(const ViewPlane& vp)
{
	this->gamma = vp.gamma;
	this->vres = vp.vres;
	this->hres = vp.hres;
	this->inv_gamma = vp.inv_gamma;
	this->num_samples = vp.num_samples;
	this->s = vp.s;
	this->sampler_ptr = vp.sampler_ptr;
	this->show_out_of_gamut = vp.show_out_of_gamut;
	this->max_depth = vp.max_depth;
}

ViewPlane::~ViewPlane(void)
{
	//if (sampler_ptr) {
	//	delete sampler_ptr;
	//	sampler_ptr = NULL;
	//}
}

void ViewPlane::set_sampler(Sampler* sp)
{
	if (sampler_ptr) {
		delete sampler_ptr;
		sampler_ptr = NULL;
	}
	num_samples = sp->get_num_samples();
	sampler_ptr = sp;
}

inline void ViewPlane::set_samples(const int n)
{
	num_samples = n;
	if (sampler_ptr) {
		delete sampler_ptr;
		sampler_ptr = NULL;
	}

	if (num_samples > 1)
		sampler_ptr = new Jittered(num_samples);
	else sampler_ptr = new Regular(1);
}


