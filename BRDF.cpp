#include "pch.h"
#include "BRDF.h"
#include "ShadeRec.h"
void Lambertian::set_kd(float kd)
{
	this->kd = kd;
}

void Lambertian::set_cd(RGBColor cd)
{
	this->cd = cd;
}

RGBColor PerfectSpecular::sample_f(const ShadeRec& sr,const Vector3D& wo, Vector3D& wi) const
{
	float ndotwo = (float)Dot(sr.normal, wo);
	wi = -1 * wo + 2.0 * sr.normal * ndotwo;
	//这里的wi为反射光，即主光线与矢量的交点的反射光线
	return kr * cr / Dot(sr.normal, wi);
}

void PerfectSpecular::set_kr(float kr)
{
	this->kr = kr;
}

void PerfectSpecular::set_cr(RGBColor cr)
{
	this->cr = cr;
}

RGBColor GlossySpecular::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const
{
	{//镜面反射光
		RGBColor L;
		float ndotwi = (float)Dot(sr.normal, wi);
		Vector3D r(-1 * wi + 2.0 * sr.normal * ndotwi);//反射方向r，由公式r= -l+ 2(n*l)n 得到 
		// 推导：ndotwi为cos(theta)，而wi由于归一化故为1，wi * ndotwi 为在 sr.normal上的投影长度，
		// 将长度*方向减去入射光线即为反射光线
		float rdotwo = (float)Dot(r, wo);//wo为点到主光线的矢量
		if (rdotwo > 0.0)
			L = cs * ks * pow(rdotwo, exp);//根据phong模型得到
		return L;
	}
}

RGBColor GlossySpecular::sample_f(const ShadeRec& sr,const Vector3D& wo, Vector3D& wi) const
{
	double ndotwo = Dot(sr.normal, wo);

	Vector3D r = -1 * wo + 2.0 * sr.normal * ndotwo;

	Vector3D w = r;
	Vector3D u = Cross(Vector3D(0.00424, 1.0, 0.00764), w);
	//Vector3D wf = w.x < -0.1 || w.x > 0.1 ? Vector3D(0, 1, 0) : Vector3D(1, 0, 0);
	//Vector3D u = Cross(wf, w);
	u = u.Normalize();
	Vector3D v = Cross(u, w);

	CP3 point = sr.hit_point;

	wi = point.x * u + point.y * v + point.z * w;

	if (Dot(sr.normal, wi) < 0.0)
		wi = -point.x * u - point.y * v - point.z * w;

	double phong_lobe = pow(Dot(r, wi), exp);

	return (ks * cs * phong_lobe);
}

void GlossySpecular::set_ks(float ks)
{
	this->ks = ks;
}

void GlossySpecular::set_cs(RGBColor cs)
{
	this->cs = cs;
}

void GlossySpecular::set_exp(double exp)
{
	this->exp = exp;
}


//CRGBA NormalTransmitter::f(ShadeRec& sr, Vector3D& wo, Vector3D& wt)
//{
//	return CRGBA(0.0, 0.0, 0.0);
//}
//CRGBA NormalTransmitter::sample_f(ShadeRec& sr, Vector3D& wo, Vector3D& wt)
//{
//	Vector3D n(sr.normal);
//	float cos_thetai = Dot(n, wo);
//	float eta = ior;
//
//	if (cos_thetai < 0.0) {			// transmitted ray is outside     
//		cos_thetai = -cos_thetai;
//		n = -1 * n;  					// reverse direction of normal
//		eta = 1.0 / eta; 			// invert ior 
//	}
//
//	float temp = 1.0 - (1.0 - cos_thetai * cos_thetai) / (eta * eta);
//	float cos_theta2 = sqrt(temp);
//	wt = -1 * wo / eta - (cos_theta2 - cos_thetai / eta) * n;
//
//	return (kt / (eta * eta) * CRGBA(1.0, 1.0, 1.0) / fabs(Dot(sr.normal, wt)));
//}
//CRGBA NormalTransmitter::rho(ShadeRec& sr, Vector3D& wo)
//{
//	return CRGBA(0.0, 0.0, 0.0);
//}
inline RGBColor NormalTransmitter::sample_f(const ShadeRec& sr, Vector3D& wo, Vector3D& wt) const
{
	Vector3D n(sr.normal);
	float cos_thetai = static_cast<float>(Dot(n, wo));
	float eta = static_cast<float>(ior);

	if (cos_thetai < 0.0) {			// transmitted ray is outside     
		cos_thetai = -cos_thetai;
		n = -1 * n;  					// reverse direction of normal
		eta = 1.0f / eta; 			// invert ior 
	}

	float temp = 1.0f - (1.0f - cos_thetai * cos_thetai) / (eta * eta);
	float cos_theta2 = sqrt(temp);
	wt = -1 * wo / eta - (cos_theta2 - cos_thetai / eta) * n;

	return (kt / (eta * eta) * CRGBA(1.0, 1.0, 1.0) / fabs(Dot(sr.normal, wt)));
}

bool NormalTransmitter::if_tir(ShadeRec& sr)
{
	Vector3D wo(-1 * sr.ray.d);
	double cos_thetai = Dot(sr.normal, wo);
	double eta = ior;

	if (cos_thetai < 0.0)
		eta = 1.0 / eta;

	return (1.0 - (1.0 - cos_thetai * cos_thetai) / (eta * eta) < 0.0);
}