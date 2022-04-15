#include "pch.h"
#include "Material.h"
#include "ShadeRec.h"
Matte::Matte(void)
	:Material(),
	ambient_brdf(new Lambertian),
	diffuse_brdf(new Lambertian)
{}

Matte::~Matte(void)
{
	if (ambient_brdf) {
		delete ambient_brdf;
		ambient_brdf = NULL;
	}
	if (diffuse_brdf) {
		delete diffuse_brdf;
		diffuse_brdf = NULL;
	}
}

void Matte::set_ka(const float ka)
{
	ambient_brdf->set_kd(ka);
}

void Matte::set_kd(const float kd)
{
	diffuse_brdf->set_kd(kd);
}

//void Matte::set_cd(const RGBColor& c)
//{
//	ambient_brdf->set_cd(c);
//	diffuse_brdf->set_cd(c);
//}

void Matte::set_ambient(RGBColor& c)
{
	this->ambient_brdf->set_cd(c);
}

void Matte::set_diffuse(RGBColor& c)
{
	this->diffuse_brdf->set_cd(c);
}

inline RGBColor Matte::shade(ShadeRec& sr)//�������ߣ�������ɫ
{
	Vector3D wo = -1 * sr.ray.d;
	RGBColor L = ambient_brdf->rho(sr, wo) * sr.w.ambient_ptr->L(sr);
	int numLights = (int)sr.w.lights.size();

	for (int j = 0; j < numLights; j++) {
		Vector3D wi = sr.w.lights[j]->get_direction(sr);
		float ndotwi = (float)Dot(sr.normal, wi);//�жϸõ��Ƿ�Ӧ�ñ������Ӱ�����ж������滹�Ǳ���
		if (ndotwi > 0.0){
			bool in_shadow = false;
			if (sr.w.lights[j]->casts_shadows()) {//fix bool 
				Ray shadowRay(sr.hit_point, wi);//�ɵ�ָ����չ��ߣ����⣩
				in_shadow = sr.w.lights[j]->in_shadow(shadowRay, sr);
			}

			if (!in_shadow)
				L += diffuse_brdf->f(sr, wo, wi) * sr.w.lights[j]->L(sr) * ndotwi;
		}

	}
	return L;
}

//inline RGBColor Matte::area_light_shade(ShadeRec& sr)
//{
//	return shade(sr);
//}

Phong::Phong(void)
	:ambient_brdf(new Lambertian),
	diffuse_brdf(new Lambertian),
	specular_brdf(new GlossySpecular)
{
}

Phong::~Phong(void)
{
	if (ambient_brdf) {
		delete ambient_brdf;
		ambient_brdf = NULL;
	}
	if (diffuse_brdf) {
		delete diffuse_brdf;
		diffuse_brdf = NULL;
	}
	if (specular_brdf) {
		delete specular_brdf;
		specular_brdf = NULL;
	}
}

void Phong::set_ka(const float k)
{
	this->ambient_brdf->set_kd(k);
}

void Phong::set_kd(const float k)
{
	this->diffuse_brdf->set_kd(k);
}

void Phong::set_ks(const float k)
{
	specular_brdf->set_ks(k);
}

void Phong::set_ambient(RGBColor& c)
{
	this->ambient_brdf->set_cd(c);
}

void Phong::set_diffuse(RGBColor& c)
{
	this->diffuse_brdf->set_cd(c);
}

void Phong::set_specular(RGBColor& c)
{
	specular_brdf->set_cs(c);
}

void Phong::set_specular_e(const double e)
{
	specular_brdf->set_exp(e);
}

inline RGBColor Phong::shade(ShadeRec& sr)
{
	Vector3D wo = -1 * sr.ray.d;

	RGBColor L;

	//��������
	//if (sr.img)//����������
	//	L += sr.img->GetTexture(sr.u, sr.v);

	//ͶӰ����
	//if (sr.img) {
	//	CP3 projective_point = CP3(0, 0, 200);
	//	int scale = 50;//scale factor
	//	int image_width = (int)sr.img->bmp.bmWidth / scale;
	//	int image_height = (int)sr.img->bmp.bmHeight / scale;
	//	CP3 image_org = CP3(-image_width / 2, -image_height / 2, 100);
	//	Vector3D image_normal = Vector3D(CP3(0, 0, -1)).Normalize();//image normal, change
	//	Vector3D d = Vector3D(projective_point - sr.hit_point ).Normalize();
	//	//Vector3D d = -1 * image_normal;//can hit image?
	//	//hit image
	//	float t = (float)(Dot(Vector3D(image_org - sr.hit_point), image_normal) / Dot(d, image_normal));
	//	if (t > 1e-4) {
	//		CP3 hit_point = sr.hit_point + CP3(t * d.x, t * d.y, t * d.z);
	//		Vector3D dd = Vector3D(hit_point - image_org);
	//		float ddota = (float)Dot(dd, CP3(image_width, 0, 0));
	//		float ddotb = (float)Dot(dd, CP3(0, image_height, 0));
	//		if ((ddota >= 0.0 && ddota <= image_width * image_width) && (ddotb >= 0.0 && ddotb <= image_height * image_height)) {
	//			//get image color
	//			double inv_width = 1.0 / sr.img->bmp.bmWidth;
	//			double inv_height = 1.0 / sr.img->bmp.bmHeight;
	//			double u = (double)((hit_point.x - image_org.x) * scale * inv_width);
	//			double v = (double)((hit_point.y - image_org.y) * scale * inv_height);
	//			L += sr.img->GetTexture(u, v);
	//		}
	//	}
	//}

	L += ambient_brdf->rho(sr, wo) * sr.w.ambient_ptr->L(sr);//����world���Ĭ�ϻ�����
	int num_lights = (int)sr.w.lights.size();//�����õ�����

	for (int j = 0; j < num_lights; j++) {
		Vector3D wi = sr.w.lights[j]->get_direction(sr);//���չ�������ʸ������ָ�����
		float ndotwi = (float)Dot(sr.normal, wi);

		if (ndotwi > 0.0) {
			bool in_shadow = false;
			if (sr.w.lights[j]->casts_shadows()) {//fix bool 
				Ray shadowRay(sr.hit_point, wi);//�ɵ�ָ����չ��ߣ����⣩
				in_shadow = sr.w.lights[j]->in_shadow(shadowRay, sr);
			}

			if (!in_shadow)
				L += (diffuse_brdf->f(sr, wo, wi) + specular_brdf->f(sr, wo, wi)) *
				sr.w.lights[j]->L(sr) * ndotwi;
		}
	}
	return L;
}

//RGBColor Phong::area_light_shade(ShadeRec& s)
//{
//	return shade(s);
//}
//
//inline RGBColor Emissive::area_light_shade(ShadeRec& sr)
//{
//	if (Dot(-1 * sr.normal, sr.ray.d) > 0.0)
//		return ls * ce;
//	else return sr.w.background_color;
//}

Material::~Material()
{
}

Reflective::Reflective(void):Phong()
{
	reflective_brdf = new PerfectSpecular;
	//reflective_brdf = new GlossySpecular;
}

Reflective::~Reflective()
{
	if (reflective_brdf) {
		delete reflective_brdf;
		reflective_brdf = NULL;
	}
}

void Reflective::set_kr(const float k)
{
	reflective_brdf->set_kr(k);
}

void Reflective::set_reflective(RGBColor& c)
{
	reflective_brdf->set_cr(c);
}

RGBColor Reflective::shade(ShadeRec& sr)
{
	RGBColor L(Phong::shade(sr));//ͨ��phongʱ�Ѿ��ж��Ƿ������Ӱ

	Vector3D wo = -1 * sr.ray.d;
	Vector3D wi;
	RGBColor fr = reflective_brdf->sample_f(sr, wo, wi);
	Ray reflected_ray(sr.hit_point, wi);//������������Ϊ���o�����������Ϊd
	//ȥ�ж��Ƿ��������ཻ�����ཻ��ȡ����ɫ
	L += fr * sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1) * (Dot(sr.normal, wi));
	
	return L;
}


inline CRGBA Transparent::shade(ShadeRec& sr)
{
	CRGBA L = Phong::shade(sr);

	Vector3D wo = -1 * sr.ray.d;
	Vector3D wt;			//�����������
	Vector3D wi;			//���䷴�����
	//������������
	CRGBA fr = p_reflect->sample_f(sr, wo, wi);

	Ray reflect_ray;
	reflect_ray.o = sr.hit_point;
	reflect_ray.d = wi;

	if (p_btdf->if_tir(sr))
		L += sr.w.tracer_ptr->trace_ray(reflect_ray, sr.depth);
	else
	{
		CRGBA ft = p_btdf->sample_f(sr, wo, wt);
		Ray t_ray;
		t_ray.o = sr.hit_point;
		t_ray.d = wt;
		L += fr * sr.w.tracer_ptr->trace_ray(reflect_ray, sr.depth) * fabs(Dot(sr.normal, wi));
		L += ft * sr.w.tracer_ptr->trace_ray(t_ray, sr.depth) * fabs(Dot(sr.normal, wt));
	}

	return L;
}