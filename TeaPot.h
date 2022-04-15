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
	void SetRecursiveNumber(int Renumber); //���õݹ����
	void SetTeaPotCenter(Point3D center);//���ò������
	void SetMultiple(double multi);//���÷Ŵ���
	void ReadVertex();//������ƶ���ζ���
	void ReadPatch();//����˫��������Ƭ

	bool hit(const Ray& ray, float& tmin, ShadeRec& sr)const;
	bool shadow_hit(const Ray& ray, float& tmin)const;//�Ƿ�����Ӱ֮��
private:
	bool HitRim(const Ray& ray, ShadeRec& sr)const;
	bool HitBody(const Ray& ray, ShadeRec& sr)const;
	bool HitHandle(const Ray& ray, ShadeRec& sr)const;
	bool HitSpout(const Ray& ray, ShadeRec& sr)const;
	bool HitLid(const Ray& ray, ShadeRec& sr)const;
	bool HitBottom(const Ray& ray, ShadeRec& sr)const;
private:
	int Renumber;//�ݹ����
	CP3 P[306];//����ܿ��Ƶ�
	
	CPatch S[32];//�������Ƭ
	//Bezier surf;

	double multi;//�Ŵ���
	CP3 center;//һ��Ĭ������
};

