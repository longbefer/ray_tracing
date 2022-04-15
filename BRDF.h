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
		sample_f(const ShadeRec& sr, Vector3D& wi, const Vector3D& wo)const { return white; };//���ڶ�β�����������ɫ���Ȳ���Ȼ
	virtual RGBColor
		rho(const ShadeRec& sr, const Vector3D& wo)const = 0;
protected:
	//Sampler* sampler_ptr;

};

// ��ȫ������
class Lambertian : public BRDF {
public:
	virtual RGBColor
		f(const ShadeRec& sr, const Vector3D& wr, const Vector3D& wo)const {//������
		return (kd * cd * invPI);
	};
	virtual RGBColor
		sample_f(const ShadeRec& sr, Vector3D& wi, const Vector3D& wo)const {
		return kd * cd * invPI;
	};
	virtual RGBColor
		rho(const ShadeRec& sr, const Vector3D& wo)const {//������
		return(kd * cd);
	};
public:
	void set_kd(float);
	void set_cd(RGBColor);
private:
	float kd;//����ϵ���������뷴��ı���ϵ��
	RGBColor cd;//������ɫ
};

// ��ȫ���淴��
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

// ������
class GlossySpecular : public BRDF {
public:
	//woΪ�㵽�����ߣ��ӵ㣩��ʸ����wiΪ���չ��ߵ����ʸ����sr���ڴ洢��ķ�ʸ��
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
	float ks;//���淴��ϵ������Χ0-1֮�䣬Ϊ1ʱΪȫ���䣬Ϊ0ʱ������
	RGBColor cs;//����������ɫ
	double exp;//�߹�ָ����Խ��߹�Խ����
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
