#include "pch.h"
#include "Sphere.h"
#include "ShadeRec.h"
Sphere::Sphere(void)
{
	center = Point3D();
	color = RGBColor();
	radius = 1.0f;
}
//Sphere::Sphere(Point3D& center, double radius, Material* material)
//{
//	this->center = center;
//	this->radius = radius;
//	this->color = RGBColor();
//	this->p_Material = material;
//}
Sphere::~Sphere(void)
{
}
bool Sphere::hit(const Ray& ray, float& tmin, ShadeRec& sr) const
{
	// Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
	Vector3D ray_to_center = Vector3D(ray.o, center);//����ָ������
	float b = (float)Dot(ray_to_center, ray.d);//ax^2+bx+c=0
	float det = (float)(b * b - Dot(ray_to_center, ray_to_center) + radius * radius);
	const double eps = 1e-3;
	if (det < 0)
		return false;//����ǰû������
	else {
		float dets = sqrt(det);
		if (b - dets > eps){
			tmin = b - dets;
			Vector3D distance = tmin * ray.d;
			sr.hit_point = ray.o + Point3D(distance.x, distance.y, distance.z);
			sr.normal = Vector3D(center, sr.hit_point).Normalize();//ʸ��
			sr.color = this->color;//��Ĭ��Ϊ��һ��ɫ
			return true;
		} else if (b + dets>eps) {
			tmin = b + dets;
			Vector3D distance = tmin * ray.d;
			sr.hit_point = ray.o + Point3D(distance.x, distance.y, distance.z);
			sr.normal = Vector3D(center, sr.hit_point).Normalize();//ʸ��
			sr.color = this->color;//��Ĭ��Ϊ��һ��ɫ
			return true;
		} else return false;
	}
}

bool Sphere::shadow_hit(const Ray& ray, float& tmin) const
{
	//���������жϴ�����ͬ
	//�жϸõ㣨��Ϊ����ĳһ�㣩�����������t�Ƿ� �� �õ㵽���չ��ߵľ���t�Ƿ�һ�£����һ�£�������������û�н��㣬�򲻴�����Ӱ��
	Vector3D ray_to_center = Vector3D(ray.o, center);//����ָ������
	float b = (float)Dot(ray_to_center, ray.d);//ax^2+bx+c=0
	float det = (float)(b * b - Dot(ray_to_center, ray_to_center) + radius * radius);
	const double eps = 1e-3;//1e-3�ȽϺã�ֻ�н��ٵ����
	if (det < 0)
		return false;//���嵽���չ���ǰû������
	else {
		float dets = sqrt(det);
		if (b - dets > eps) {
			tmin = b - dets;
			return true;
		}
		else if (b + dets > eps) {
			tmin = b + dets;
			return true;
		}
		else return false;
	}
}

//Point3D Sphere::sample(void)
//{
//	Vector3D normal_temp = Vector3D(rand_int() % 10, rand_int() % 10, rand_int() % 10).Normalize();
//	return center + Point3D(normal_temp.x, normal_temp.y, normal_temp.z) * radius;
//}
//
//Vector3D Sphere::get_normal(const Point3D& p)
//{
//	return Vector3D(center, p);
//}
