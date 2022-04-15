#pragma once
#include "Vector3D.h"
#include "Sampler.h"
class ShadeRec;
class BRDF
{
public:
	virtual RGBColor
		f(const ShadeRec& sr, const Vector3D& wr, const Vector3D& wo)const = 0;
	virtual RGBColor
		sample_f(const ShadeRec& sr, Vector3D& wi, const Vector3D& wo)const { return white; };//用于多次采样，消除颜色过度不自然
	virtual RGBColor
		rho(const ShadeRec& sr, const Vector3D& wo)const = 0;
protected:
	//Sampler* sampler_ptr;

};

// 完全漫反射
class Lambertian : public BRDF {
public:
	virtual RGBColor
		f(const ShadeRec& sr, const Vector3D& wr, const Vector3D& wo)const {//漫反射
		return (kd * cd * invPI);
	};
	virtual RGBColor
		sample_f(const ShadeRec& sr, Vector3D& wi, const Vector3D& wo)const {
		return kd * cd * invPI;
	};
	virtual RGBColor
		rho(const ShadeRec& sr, const Vector3D& wo)const {//环境光
		return(kd * cd);
	};
public:
	void set_kd(float);
	void set_cd(RGBColor);
private:
	float kd;//比例系数，入射与反射的比例系数
	RGBColor cd;//反射颜色
};

// 完全镜面反射
class PerfectSpecular : public BRDF {
public:
	virtual RGBColor
		f(const ShadeRec& sr, const Vector3D& wr, const Vector3D& wo)const {
		return black;
	};
	virtual RGBColor
		sample_f(const ShadeRec& sr,const Vector3D& wi, Vector3D& wo)const;
	virtual RGBColor
		rho(const ShadeRec& sr, const Vector3D& wo)const {
		return black;
	};
public:
	void set_kr(float kr);
	void set_cr(RGBColor cr);
private:
	float kr;
	RGBColor cr;
};

// 光泽反射
class GlossySpecular : public BRDF {
public:
	//wo为点到主光线（视点）的矢量，wi为光照光线到点的矢量，sr用于存储点的法矢量
	virtual RGBColor
		f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi)const;
	virtual RGBColor
		sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi)const;
	virtual RGBColor
		rho(const ShadeRec& sr, const Vector3D& wo)const {
		return black;
	};
public:
	void set_ks(float ks);
	void set_cs(RGBColor cs);
	void set_exp(double exp);
private:
	float ks;//镜面反射系数，范围0-1之间，为1时为全反射，为0时不反射
	RGBColor cs;//人射辐射度颜色
	double exp;//高光指数，越大高光越集中
};

class NormalTransmitter : public BRDF {
public:
	bool if_tir(ShadeRec& sr);
	void set_kt(double kt);
	void set_ior(double ior);
public:
	virtual RGBColor
		f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi)const {
		return black;
	};
	virtual RGBColor
		sample_f(const ShadeRec& sr, Vector3D&, Vector3D& )const;
	virtual RGBColor
		rho(const ShadeRec& sr, const Vector3D& wo)const {
		return black;
	};
private:
	double kt;
	double ior;
};

inline void NormalTransmitter::set_kt(double kt)
{
	this->kt = kt;
}
inline void NormalTransmitter::set_ior(double ior)
{
	this->ior = ior;
}
