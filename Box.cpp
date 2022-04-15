#include "pch.h"
#include "Box.h"
#include "ShadeRec.h"
Box::Box(void)
{
}

Box::Box(Point3D& p1, Point3D& p2)//右上前点，左下后点
{
	if (p1.x > p2.x && p1.y > p2.y && p1.z > p2.z) {
		this->_top_up = p1;
		this->_bottom_dowm = p2;
	} else {
		this->_top_up = p2;
		this->_bottom_dowm = p1;
	}
}

Box::~Box(void)
{
}

bool Box::hit(const Ray& ray, float& tmin, ShadeRec& sr) const
{
	float kEpsilon = 1e-4f;
	float ox = (float)ray.o.x; float oy = (float)ray.o.y; float oz = (float)ray.o.z;
	float dx = (float)ray.d.x; float dy = (float)ray.d.y; float dz = (float)ray.d.z;

	float tx_min, ty_min, tz_min;
	float tx_max, ty_max, tz_max;

	float x0 = (float)_bottom_dowm.x;
	float x1 = (float)_top_up.x;

	float a = 1.0f / dx;
	if (a >= 0) {//当斜率大于0时
		tx_min = (x0 - ox) * a;
		tx_max = (x1 - ox) * a;
	} else {//当斜率小于0时
		tx_min = (x1 - ox) * a;
		tx_max = (x0 - ox) * a;
	}

	float y0 = (float)_bottom_dowm.y;
	float y1 = (float)_top_up.y;

	float b = 1.0f / dy;
	if (b >= 0) {
		ty_min = (y0 - oy) * b;
		ty_max = (y1 - oy) * b;
	}
	else {
		ty_min = (y1 - oy) * b;
		ty_max = (y0 - oy) * b;
	}

	float z0 = (float)_bottom_dowm.z;
	float z1 = (float)_top_up.z;

	float c = 1.0f / dz;
	if (c >= 0) {
		tz_min = (z0 - oz) * c;
		tz_max = (z1 - oz) * c;
	}
	else {
		tz_min = (z1 - oz) * c;
		tz_max = (z0 - oz) * c;
	}


	float t0, t1;
	int face_in, face_out;
	if (tx_min > ty_min) {//找到最大的t
		t0 = tx_min;
		face_in = (a >= 0.0) ? 0 : 3;
	} else {
		t0 = ty_min;
		face_in = (b >= 0.0) ? 1 : 4;
	}
	if (tz_min > t0) {
		t0 = tz_min;
		face_in = (c >= 0.0) ? 2 : 5;//和z轴方向一致/相反
	}

	if (tx_max < ty_max) {//找到最小的t
		t1 = tx_max;
		face_out = (a >= 0.0) ? 3 : 0;
	} else {
		t1 = ty_max;
		face_out = (b >= 0.0) ? 4 : 1;
	}
	if (tz_max < t1) {
		t1 = tz_max;
		face_out = (c >= 0.0) ? 5 : 2;
	}

	if (t0<t1 && t1>kEpsilon) {
		if (t0 > kEpsilon) {
			tmin = t0;
			sr.normal = get_normal(face_in);
		} else {
			tmin = t1;
			sr.normal = get_normal(face_out);
		}
		Vector3D distance = tmin * ray.d;
		sr.hit_point = ray.o + Point3D(distance.x, distance.y, distance.z);
		sr.color = color;
		return true;
	} else return false;
}

bool Box::shadow_hit(const Ray& ray, float& tmin) const
{
	float kEpsilon = 1e-4f;
	float ox = (float)ray.o.x; float oy = (float)ray.o.y; float oz = (float)ray.o.z;
	float dx = (float)ray.d.x; float dy = (float)ray.d.y; float dz = (float)ray.d.z;

	float tx_min, ty_min, tz_min;
	float tx_max, ty_max, tz_max;

	float x0 = (float)_bottom_dowm.x;
	float x1 = (float)_top_up.x;

	//有ox + t * dx = 击中点1， ox + t * dx = 击中点2
	//有oy + t * dy = 击中点1， oy + t * dy = 击中点2
	//比较t即是比较距离光线最近的点

	float a = 1.0f / dx;
	if (a >= 0) {//当斜率大于0时
		tx_min = (x0 - ox) * a;
		tx_max = (x1 - ox) * a;
	}
	else {//当斜率小于0时
		tx_min = (x1 - ox) * a;
		tx_max = (x0 - ox) * a;
	}

	float y0 = (float)_bottom_dowm.y;
	float y1 = (float)_top_up.y;

	float b = 1.0f / dy;
	if (b >= 0) {
		ty_min = (y0 - oy) * b;
		ty_max = (y1 - oy) * b;
	}
	else {
		ty_min = (y1 - oy) * b;
		ty_max = (y0 - oy) * b;
	}

	float z0 = (float)_bottom_dowm.z;
	float z1 = (float)_top_up.z;

	float c = 1.0f / dz;
	if (c >= 0) {
		tz_min = (z0 - oz) * c;
		tz_max = (z1 - oz) * c;
	}
	else {
		tz_min = (z1 - oz) * c;
		tz_max = (z0 - oz) * c;
	}

	//在t中，由图，在最小中找个最大的即为最近击中的点（由图易得）
	float t0, t1;
	if (tx_min > ty_min)//找到最大的t，即距离最近的t
		t0 = tx_min;
	else
		t0 = ty_min;

	if (tz_min > t0)
		t0 = tz_min;
	//在最大中找个最小的即为最远击中的点
	if (tx_max < ty_max)//找到最小的t
		t1 = tx_max;
	else t1 = ty_max;

	if (tz_max < t1)
		t1 = tz_max;

	return (t0 < t1 && t1 > kEpsilon);//t0<0表示在盒内，t1>0表示在盒外，且不朝向盒子，t0>0表示在盒外且朝向盒子
}

Vector3D Box::get_normal(const int face_hit) const
{
	switch (face_hit) {
	case 0:return Vector3D(-1, 0, 0);
	case 1:return Vector3D(0, -1, 0);
	case 2:return Vector3D(0, 0, -1);
	case 3:return Vector3D(1, 0, 0);
	case 4:return Vector3D(0, 1, 0);
	case 5:return Vector3D(0, 0, 1);
	default:return Vector3D(0, 0, -1);
	}
}
