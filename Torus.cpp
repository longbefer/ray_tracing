#include "pch.h"
#include "Torus.h"
#include "Box.h"
#include "ShadeRec.h"
Torus::Torus(void)
{
	this->point = Point3D();
	this->inner_radius = 10;
	this->outer_radius = 20;
}

Torus::Torus(Point3D& point, double inner, double outer)
{
	this->point = point;
	this->inner_radius = inner;
	this->outer_radius = outer;
}

Torus::~Torus(void)
{
}

Vector3D Torus::compute_normal(Point3D& p)const
{
	Vector3D normal;
	double param_squared = outer_radius * outer_radius + inner_radius * inner_radius;

	double x = p.x;
	double y = p.y;
	double z = p.z;
	double sum_squared = x * x + y * y + z * z;

	normal.x = 4.0 * x * (sum_squared - param_squared);
	normal.y = 4.0 * y * (sum_squared - param_squared + 2.0 * outer_radius * outer_radius);
	normal.z = 4.0 * z * (sum_squared - param_squared);
	normal = normal.Normalize();

	return (normal);
}

bool Torus::hit(const Ray& ray, float& tmin, ShadeRec& sr) const
{
	if (!shadow_hit(ray, tmin))
		return false;
	Point3D newp = ray.o - point;
	Ray rays = Ray(newp, ray.d);
	Vector3D vray = tmin * rays.d;
	sr.hit_point = rays.o + Point3D(vray.x, vray.y, vray.z);
	sr.normal = compute_normal(sr.hit_point);
	sr.color = color;
	return (true);
}

bool Torus::shadow_hit(const Ray& ray, float& tmin) const
{
	//通过包围盒判定是否击中，减少计算量
	Ray temp_ray = Ray(ray.o, ray.d);
	float temp_t = 1e20f;
	Point3D right_top = Point3D(point.x + outer_radius + inner_radius, point.y + inner_radius, point.z + inner_radius + outer_radius);
	Point3D left_bottom = Point3D(point.x - outer_radius - inner_radius, point.y - inner_radius, point.z - inner_radius - outer_radius);
	Box* box = new Box(right_top, left_bottom);
	if (!(box->shadow_hit(temp_ray, temp_t))) {//应该为hit（）但shadow_hit（）和其一样，且不需要ShadeRec类，相比之下更合理
		delete box; box = NULL;
		return false;
	}
	delete box; box = NULL;

	Point3D newp = ray.o - point;
	Ray rays = Ray(newp, ray.d);

	double x1 = rays.o.x; double y1 = rays.o.y; double z1 = rays.o.z;
	double d1 = rays.d.x; double d2 = rays.d.y; double d3 = rays.d.z;

	double coeffs[5];	// coefficient array for the quartic equation
	double roots[4];	// solution array for the quartic equation

	double  kEpsilon = 0.000001;
	double	kHugeValue = 1.0E20;

	// define the coefficients of the quartic equation

	double sum_d_sqrd = d1 * d1 + d2 * d2 + d3 * d3;
	double e = x1 * x1 + y1 * y1 + z1 * z1 - inner_radius * inner_radius - outer_radius * outer_radius;
	double f = x1 * d1 + y1 * d2 + z1 * d3;
	double four_a_sqrd = 4.0 * outer_radius * outer_radius;

	coeffs[0] = e * e - four_a_sqrd * (inner_radius * inner_radius - y1 * y1); 	// constant term
	coeffs[1] = 4.0 * f * e + 2.0 * four_a_sqrd * y1 * d2;
	coeffs[2] = 2.0 * sum_d_sqrd * e + 4.0 * f * f + four_a_sqrd * d2 * d2;
	coeffs[3] = 4.0 * sum_d_sqrd * f;
	coeffs[4] = sum_d_sqrd * sum_d_sqrd;

	// find roots of the quartic equation

	int num_real_roots = SolveQuartic(coeffs, roots);

	bool	intersected = false;
	double 	t = kHugeValue;

	if (num_real_roots == 0)  // ray misses the torus
		return(false);

	// find the smallest root greater than kEpsilon, if any
	// the roots array is not sorted

	for (int j = 0; j < num_real_roots; j++)
		if (roots[j] > kEpsilon) {
			intersected = true;
			if (roots[j] < t)
				t = roots[j];
		}

	if (!intersected)
		return (false);


	tmin = (float)t;
	return true;
}
