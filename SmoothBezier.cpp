#include "pch.h"
#include "SmoothBezier.h"
#include "ShadeRec.h"
#include "SmoothTriangle.h"
SmoothBezier::SmoothBezier(void)
{
	ReNumber = 1;
}

SmoothBezier::SmoothBezier(CP3 V[4][4], int ReNumber)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			this->V3[i * 4 + j] = V[i][j];
	this->ReNumber = ReNumber;
}

SmoothBezier::~SmoothBezier()
{
}

BOOL SmoothBezier::Hit(const Ray& ray, ShadeRec& sr)const
{
	CT2 Rect[4];
	Rect[0] = CT2(0, 0); Rect[1] = CT2(1, 0);//��ʼ��uv
	Rect[2] = CT2(1, 1); Rect[3] = CT2(0, 1);
	return Recursion(ray, sr, ReNumber, Rect);//�ݹ麯��
}

BOOL SmoothBezier::Recursion(const Ray& ray, ShadeRec& sr, int ReNumber, CT2* pRect)const
{
	if (ReNumber == 0)
	{
		PointVector P[4];
		Tessellation(pRect, P);
		return DrawFacet(ray, sr, P);//����Сƽ��;
	}
	else
	{
		double HalfU = (pRect[0].u + pRect[1].u) / 2.0;
		double HalfV = (pRect[0].v + pRect[3].v) / 2.0;
		CT2 SubRect[4][4];//�Ĳ���
		//�����ӳ�����
		SubRect[0][0] = pRect[0];
		SubRect[0][1].u = HalfU;     SubRect[0][1].v = pRect[0].v;
		SubRect[0][2].u = HalfU;     SubRect[0][2].v = HalfV;
		SubRect[0][3].u = pRect[0].u; SubRect[0][3].v = HalfV;
		//�����ӳ�����
		SubRect[1][0] = SubRect[0][1];
		SubRect[1][1] = pRect[1];
		SubRect[1][2].u = pRect[1].u; SubRect[1][2].v = HalfV;
		SubRect[1][3] = SubRect[0][2];
		//�����ӳ�����
		SubRect[2][0] = SubRect[1][3];
		SubRect[2][1] = SubRect[1][2];
		SubRect[2][2] = pRect[2];
		SubRect[2][3].u = HalfU; SubRect[2][3].v = pRect[2].v;
		//�����ӳ�����
		SubRect[3][0] = SubRect[0][3];
		SubRect[3][1] = SubRect[2][0];
		SubRect[3][2] = SubRect[2][3];
		SubRect[3][3] = pRect[3];

		float tmin = 1e20f;
		Vector3D normal;
		Point3D hitPoint;
		bool if_hit = false;

		CT2 uv;

		ShadeRec sr1(sr.w), sr2(sr.w), sr3(sr.w), sr4(sr.w);

		sr1.hit_an_object = Recursion(ray, sr1, ReNumber - 1, SubRect[0]);
		sr2.hit_an_object = Recursion(ray, sr2, ReNumber - 1, SubRect[1]);
		sr3.hit_an_object = Recursion(ray, sr3, ReNumber - 1, SubRect[2]);
		sr4.hit_an_object = Recursion(ray, sr4, ReNumber - 1, SubRect[3]);

		ShadeRec unit_sr[] = { sr1,sr2,sr3,sr4 };

		for (int i = 0; i < sizeof(unit_sr) / sizeof(ShadeRec); i++)
			if (unit_sr[i].hit_an_object && unit_sr[i].t < tmin) {
				if_hit = true;
				tmin = unit_sr[i].t;
				normal = unit_sr[i].normal;
				hitPoint = unit_sr[i].hit_point;
				uv = CT2(unit_sr[i].u, unit_sr[i].v);
			}
		sr.t = tmin;
		sr.normal = normal;
		sr.hit_point = hitPoint;
		sr.u = uv.u; sr.v = uv.v;
		return if_hit;
	}
}

void SmoothBezier::Tessellation(CT2* pT, PointVector* P)const
{
	double M[4][4];//ϵ������Mbe
	M[0][0] = -1; M[0][1] = 3; M[0][2] = -3; M[0][3] = 1;
	M[1][0] = 3;  M[1][1] = -6; M[1][2] = 3; M[1][3] = 0;
	M[2][0] = -3; M[2][1] = 3; M[2][2] = 0; M[2][3] = 0;
	M[3][0] = 1;  M[3][1] = 0; M[3][2] = 0; M[3][3] = 0;
	CP3 VT[4][4];//���߼����ÿ��Ƶ�����
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			VT[i][j] = V3[i * 4 + j];
	LeftMultiplyMatrix(M, VT);//ϵ�����������ά�����
	TransposeMatrix(M);//����ת�þ���
	RightMultiplyMatrix(VT, M);//ϵ�������ҳ���ά�����
	double u0, u1, u2, u3, v0, v1, v2, v3;//u��v��������
	for (int i = 0; i < 4; i++)
	{
		u3 = pow(pT[i].u, 3.0), u2 = pow(pT[i].u, 2.0), u1 = pow(pT[i].u, 1.0), u0 = 1;
		v3 = pow(pT[i].v, 3.0), v2 = pow(pT[i].v, 2.0), v1 = pow(pT[i].v, 1.0), v0 = 1;
		CP3 P3 = (u3 * VT[0][0] + u2 * VT[1][0] + u1 * VT[2][0] + u0 * VT[3][0]) * v3
			+ (u3 * VT[0][1] + u2 * VT[1][1] + u1 * VT[2][1] + u0 * VT[3][1]) * v2
			+ (u3 * VT[0][2] + u2 * VT[1][2] + u1 * VT[2][2] + u0 * VT[3][2]) * v1
			+ (u3 * VT[0][3] + u2 * VT[1][3] + u1 * VT[2][3] + u0 * VT[3][3]) * v0;
		/////////////////////////////////////�����ķ�ʸ��
		Vector3D Pu = 3 * u2 * (VT[0][0] * v3 + VT[0][1] * v2 + VT[0][2] * v1 + VT[0][3]) +
			2 * u1 * (VT[1][0] * v3 + VT[1][1] * v2 + VT[1][2] * v1 + VT[1][3]) +
			(VT[2][0] * v3 + VT[2][1] * v2 + VT[2][2] * v1 + VT[2][3]);//����u�����������
		Vector3D Pv = 3 * v2 * (VT[0][0] * u3 + VT[1][0] * u2 + VT[2][0] * u1 + VT[3][0]) +
			2 * v1 * (VT[0][1] * u3 + VT[1][1] * u2 + VT[2][1] * u1 + VT[3][1]) +
			(VT[0][2] * u3 + VT[1][2] * u2 + VT[2][2] * u1 + VT[3][2]);//����v�����������
		Vector3D N = Cross(Pu, Pv).Normalize();//P3��ķ�ʸ��
		P[i].point = P3;
		P[i].vector = N;
		P[i].t = CT2(pT[i].u, pT[i].v);
	}
}

BOOL SmoothBezier::DrawFacet(const Ray& ray, ShadeRec& sr, PointVector* P)const
{
	//pDC->MoveTo(ROUND(P[0].x), ROUND(P[0].y));
	//pDC->LineTo(ROUND(P[1].x), ROUND(P[1].y));
	//pDC->LineTo(ROUND(P[2].x), ROUND(P[2].y));
	//pDC->LineTo(ROUND(P[3].x), ROUND(P[3].y));
	//pDC->LineTo(ROUND(P[0].x), ROUND(P[0].y));

	float t;
	SmoothTriangle* pFill = new SmoothTriangle(P[0], P[2], P[1]);
	if (pFill->hit(ray, t, sr)) {
		sr.hit_an_object = true;
		sr.t = t;
		//sr.u, sr.v;��triangle���Ѹ�ֵ
		delete pFill; pFill = NULL;
		return TRUE;
	}
	delete pFill; pFill = NULL;

	SmoothTriangle* pFill1 = new SmoothTriangle(P[0], P[3], P[2]);
	if (pFill1->hit(ray, t, sr)) {
		sr.hit_an_object = true;
		sr.t = t;
		//sr.u, sr.v;
		delete pFill1; pFill1 = NULL;
		return TRUE;
	}
	delete pFill1; pFill1 = NULL;
	return FALSE;
}

void SmoothBezier::LeftMultiplyMatrix(double M[4][4], CP3 V[4][4])const//��˾���M*V
{
	CP3 VT[4][4];//��ʱ����
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			VT[i][j] = M[i][0] * V[0][j] + M[i][1] * V[1][j] + M[i][2] * V[2][j] + M[i][3] * V[3][j];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			V[i][j] = VT[i][j];
}

void SmoothBezier::RightMultiplyMatrix(CP3 V[4][4], double M[4][4])const//�ҳ˾���V*M
{
	CP3 VT[4][4];//��ʱ����
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			VT[i][j] = V[i][0] * M[0][j] + V[i][1] * M[1][j] + V[i][2] * M[2][j] + V[i][3] * M[3][j];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			V[i][j] = VT[i][j];
}

void SmoothBezier::TransposeMatrix(double M[4][4])const//ת�þ���
{
	double VT[4][4];//��ʱ����
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			VT[j][i] = M[i][j];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			M[i][j] = VT[i][j];
}