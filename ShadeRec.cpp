#include "pch.h"
#include "ShadeRec.h"
//´«World¼û.h
ShadeRec::ShadeRec(const ShadeRec& sr)
	: hit_an_object(sr.hit_an_object),
	material_ptr(sr.material_ptr),
	hit_point(sr.hit_point),t(sr.t),
	depth(sr.depth),dir(sr.dir),ray(sr.ray),
	//local_hit_point(sr.local_hit_point),
	normal(sr.normal),
	color(sr.color),img(sr.img), u(sr.u), v(sr.v),
	w(sr.w)
{
}

ShadeRec::~ShadeRec(void)
{
	//if (material_ptr) {
	//	delete material_ptr;
	//	material_ptr = NULL;
	//}
}
