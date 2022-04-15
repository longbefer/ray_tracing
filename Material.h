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

	void set_ka(const float k);//����ambient��ϵ��
	void set_kd(const float k);//����diffuse��ϵ��
	void set_ks(const float k);//����specular��ϵ��
	void set_ambient(RGBColor& c);//���� ��ɫ
	void set_diffuse(RGBColor& c);//���� ��ɫ
	void set_specular(RGBColor& c);//���þ��淴�����ɫ
	void set_specular_e(const double e);//���÷���ĸ߹�ָ��
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
//	float ls;//�����ϵ��
//	RGBColor ce;//��ɫ
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

class GlossyReflective : public Reflective // ע�⣬�Դ�
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

	//����������
	void set_ior(double ior);
	//���������
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