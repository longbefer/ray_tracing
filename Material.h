#pragma once
#include "BRDF.h"
class ShadeRec;
class Material
{
public:
	virtual ~Material();
	virtual RGBColor
		shade(ShadeRec& sr) = 0;
	//virtual RGBColor
	//	area_light_shade(ShadeRec& sr) = 0;
	//virtual RGBColor get_Le(ShadeRec& sr)const;
	//virtual RGBColor path_shade(ShadeRec& sr);
};

class Matte : public Material {
public:
	Matte(void);
	virtual ~Matte(void);
	void set_ka(const float k);
	void set_kd(const float k);
	//void set_cd(const RGBColor& c);
	void set_ambient(RGBColor& c);
	void set_diffuse(RGBColor& c);

	virtual RGBColor shade(ShadeRec& sr);
	//virtual RGBColor area_light_shade(ShadeRec& sr);

	//virtual RGBColor get_Le(ShadeRec& sr)const;
private:
	Lambertian* ambient_brdf;
	Lambertian* diffuse_brdf;
};

class Phong : public Material {
public:
	Phong(void);
	virtual ~Phong(void);

	void set_ka(const float k);//设置ambient的系数
	void set_kd(const float k);//设置diffuse的系数
	void set_ks(const float k);//设置specular的系数
	void set_ambient(RGBColor& c);//设置 颜色
	void set_diffuse(RGBColor& c);//设置 颜色
	void set_specular(RGBColor& c);//设置镜面反射的颜色
	void set_specular_e(const double e);//设置反射的高光指数
	virtual RGBColor shade(ShadeRec& s);
	//virtual RGBColor area_light_shade(ShadeRec& s);

	//virtual RGBColor get_Le(ShadeRec& sr)const;
protected:

	Lambertian* ambient_brdf;
	Lambertian* diffuse_brdf;
	GlossySpecular* specular_brdf;
};

//class Emissive :public Material {
//private:
//	float ls;//辐射度系数
//	RGBColor ce;//颜色
//public:
//	void scale_radiance(const float _ls);
//	void set_ce(const float r, const float g, const float b);
//	virtual RGBColor get_Le(ShadeRec& sr)const;
//	virtual RGBColor shade(ShadeRec& sr);
//	virtual RGBColor area_light_shade(ShadeRec& sr);
//};

class Reflective : public Phong
{
public:
	Reflective(void);
	virtual ~Reflective();
public:
	void set_kr(const float k);
	void set_reflective(RGBColor& c);
	virtual RGBColor shade(ShadeRec& sr);
private:
	PerfectSpecular* reflective_brdf;
};

class GlossyReflective : public Reflective // 注意，自创
{
public:
	GlossyReflective(void);
	virtual ~GlossyReflective();
public:
	void set_kr(const float k);
	void set_reflective(RGBColor& c);
	void set_ks(float ks);
	void set_cs(RGBColor cs);
	void set_exp(double exp);
	virtual RGBColor shade(ShadeRec& sr);
private:
	GlossySpecular* reflective_brdf;
};

class Transparent :public Phong
{
public:
	Transparent(void);
	virtual ~Transparent(void);

	virtual CRGBA shade(ShadeRec& sr);
public:
	void set_kr(double k); 
	void set_cr(CRGBA c);

	//设置折射率
	void set_ior(double ior);
	//设置折射度
	void set_kt(double kt);
private:
	PerfectSpecular* p_reflect;
	NormalTransmitter* p_btdf;
};

inline Transparent::Transparent(void)
	:p_reflect(new PerfectSpecular),
	p_btdf(new NormalTransmitter)
{

}

inline Transparent::~Transparent(void)
{
	if (p_reflect) {
		delete p_reflect;
		p_reflect = nullptr;
	}
	if (p_btdf) {
		delete p_btdf;
		p_btdf = nullptr;
	}
}

inline void Transparent::set_kr(double k)
{
	p_reflect->set_kr(static_cast<float>(k));
}
inline	void Transparent::set_cr(CRGBA c)
{
	p_reflect->set_cr(c);
}
inline	void Transparent::set_ior(double ior)
{
	p_btdf->set_ior(ior);
}
inline	void Transparent::set_kt(double kt)
{
	p_btdf->set_kt(kt);
}