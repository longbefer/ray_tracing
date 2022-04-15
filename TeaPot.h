#pragma once
#include "GeometricObject.h"
#include "CPatch.h"
#include "Bezier.h"
#include "BicubicBezierPatch.h"
class TeaPot : public GeometricObject
{
public:
	TeaPot(void);
	TeaPot(Point3D& center);
	void SetRecursiveNumber(int Renumber); //设置递归深度
	void SetTeaPotCenter(Point3D center);//设置茶壶中心
	void SetMultiple(double multi);//设置放大倍数
	void ReadVertex();//读入控制多边形顶点
	void ReadPatch();//读入双三次曲面片

	bool hit(const Ray& ray, float& tmin, ShadeRec& sr)const;
	bool shadow_hit(const Ray& ray, float& tmin)const;//是否处于阴影之中
private:
	bool HitRim(const Ray& ray, ShadeRec& sr)const;
	bool HitBody(const Ray& ray, ShadeRec& sr)const;
	bool HitHandle(const Ray& ray, ShadeRec& sr)const;
	bool HitSpout(const Ray& ray, ShadeRec& sr)const;
	bool HitLid(const Ray& ray, ShadeRec& sr)const;
	bool HitBottom(const Ray& ray, ShadeRec& sr)const;
private:
	int Renumber;//递归深度
	CP3 P[306];//茶壶总控制点
	
	CPatch S[32];//茶壶总面片
	//Bezier surf;

	double multi;//放大倍数
	CP3 center;//一个默认中心
};

