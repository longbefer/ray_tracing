#include "pch.h"
#include "BicubicBezierPatch.h"
#include "Triangle.h"
#include "ShadeRec.h"
CBicubicBezierPatch::CBicubicBezierPatch(CP3 P[4][4])
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			this->P[i][j] = P[i][j];
}


CBicubicBezierPatch::~CBicubicBezierPatch(void)
{
}

void CBicubicBezierPatch::ReadCurvedVertex(Point3D** V, double tStep)const
{
	double M[4][4];//系数矩阵Mbe
	M[0][0] = -1; M[0][1] = 3; M[0][2] = -3; M[0][3] = 1;
	M[1][0] = 3; M[1][1] = -6; M[1][2] = 3; M[1][3] = 0;
	M[2][0] = -3; M[2][1] = 3; M[2][2] = 0; M[2][3] = 0;
	M[3][0] = 1; M[3][1] = 0; M[3][2] = 0; M[3][3] = 0;
	CP3 P3[4][4];//曲线计算用控制点数组
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			P3[i][j] = P[i][j];
	LeftMultiplyMatrix(M, P3);//系数矩阵左乘三维点矩阵
	TransposeMatrix(M);//计算转置矩阵
	RightMultiplyMatrix(P3, M);//系数矩阵右乘三维点矩阵
	double u0, u1, u2, u3, v0, v1, v2, v3;//u，v参数的幂
	int m = 0, n = 0;
	for (double u = 0; u <= 1; u += tStep, m++, n = 0)
		for (double v = 0; v <= 1; v += tStep)
		{
			u3 = u * u * u, u2 = u * u, u1 = u, u0 = 1;
			v3 = v * v * v, v2 = v * v, v1 = v, v0 = 1;
			CP3 pt = (u3 * P3[0][0] + u2 * P3[1][0] + u1 * P3[2][0] + u0 * P3[3][0]) * v3
				+ (u3 * P3[0][1] + u2 * P3[1][1] + u1 * P3[2][1] + u0 * P3[3][1]) * v2
				+ (u3 * P3[0][2] + u2 * P3[1][2] + u1 * P3[2][2] + u0 * P3[3][2]) * v1
				+ (u3 * P3[0][3] + u2 * P3[1][3] + u1 * P3[2][3] + u0 * P3[3][3]) * v0;
			V[m][n++] = pt;
		}
}


bool CBicubicBezierPatch::Hit(const Ray& ray, ShadeRec& sr)const
{
	double tStep = 0.1;//步长
	int nIndex = ROUND(1 / tStep) + 1;
	Point3D** V = new Point3D * [nIndex];
	for (int i = 0; i < nIndex; i++)
		V[i] = new Point3D[nIndex];
	ReadCurvedVertex(V, tStep);

	float tmin = 1e20f;
	Point3D hitPoint;
	Vector3D normal;
	bool if_hit = false;

	for (int i = 0; i < nIndex - 1; i++)
	{
		for (int j = 0; j < nIndex - 1; j++)
		{
			ShadeRec temp_sr(sr.w);

			CP3 P[4] = { V[i][j],V[i + 1][j],V[i + 1][j + 1],V[i][j + 1] };
			Triangle* pFill = new Triangle(P[0], P[2], P[1]);
			Triangle* pFill1 = new Triangle(P[0], P[3], P[2]);
			if (pFill->hit(ray, temp_sr.t, temp_sr) || pFill1->hit(ray, temp_sr.t, temp_sr))//这个只针对茶壶来说
				if (temp_sr.t < tmin) {
					if_hit = true;
					tmin = temp_sr.t;
					normal = temp_sr.normal;
					hitPoint = temp_sr.hit_point;
				}
			delete pFill; pFill = NULL;
			delete pFill1; pFill1 = NULL;
		}
	}
	if (V) {
		for (int i = 0; i < nIndex; i++) {
			delete[] V[i]; V[i] = NULL;
		}
		delete[] V; V = NULL;
	}

	sr.t = tmin;
	sr.normal = normal;
	sr.hit_point = hitPoint;
	return if_hit;
}

void CBicubicBezierPatch::LeftMultiplyMatrix(double M[][4], CP3 P[][4])const//左乘矩阵M*P
{
	CP3 PTemp[4][4];//临时矩阵
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			PTemp[i][j] = M[i][0] * P[0][j] + M[i][1] * P[1][j] + M[i][2] * P[2][j] + M[i][3] * P[3][j];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			P[i][j] = PTemp[i][j];
}

void CBicubicBezierPatch::RightMultiplyMatrix(CP3 P[][4], double M[][4])const//右乘矩阵P*M
{
	CP3 PTemp[4][4];//临时矩阵
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			PTemp[i][j] = P[i][0] * M[0][j] + P[i][1] * M[1][j] + P[i][2] * M[2][j] + P[i][3] * M[3][j];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			P[i][j] = PTemp[i][j];
}

void CBicubicBezierPatch::TransposeMatrix(double M[][4])const//转置矩阵
{
	double PTemp[4][4];//临时矩阵
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			PTemp[j][i] = M[i][j];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			M[i][j] = PTemp[i][j];
}

//bool CBicubicBezierPatch::Hit(const Ray& ray, ShadeRec& sr)const
//{
//	double tStep = 0.1;//步长
//	int nIndex = ROUND(1 / tStep) + 1;
//	Point3D** V = new Point3D * [nIndex];
//	for (int i = 0; i < nIndex; i++)
//		V[i] = new Point3D[nIndex];
//	ReadCurvedVertex(V, tStep);
//
//	for (int i = 0; i < nIndex - 1; i++)
//	{
//		for (int j = 0; j < nIndex - 1; j++)
//		{
//			CP3 P[4] = { V[i][j],V[i + 1][j],V[i + 1][j + 1],V[i][j + 1] };
//			float t = 1e20f;
//			Triangle* pFill = new Triangle(P[0], P[2], P[1]);
//			if (pFill->hit(ray, t, sr)) {
//				sr.t = t;
//				delete pFill; pFill = NULL;
//				if (V) {
//					for (int i = 0; i < nIndex; i++) {
//						delete[] V[i]; V[i] = NULL;
//					}
//					delete[] V; V = NULL;
//				}
//				return TRUE;
//			}
//			delete pFill; pFill = NULL;
//
//			Triangle* pFill1 = new Triangle(P[0], P[3], P[2]);
//			if (pFill1->hit(ray, t, sr)) {
//				sr.t = t;
//				delete pFill1; pFill1 = NULL;
//				if (V) {
//					for (int i = 0; i < nIndex; i++) {
//						delete[] V[i]; V[i] = NULL;
//					}
//					delete[] V; V = NULL;
//				}
//				return TRUE;
//			}
//			delete pFill1; pFill1 = NULL;
//		}
//	}
//	if (V) {
//		for (int i = 0; i < nIndex; i++) {
//			delete[] V[i]; V[i] = NULL;
//		}
//		delete[] V; V = NULL;
//	}
//	return FALSE;
//}

//bool CBicubicBezierPatch::Hit(const Ray& ray, ShadeRec& sr)const
//{
//	double tStep = 0.1;//步长
//	int nIndex = ROUND(1 / tStep) + 1;
//	Point3D** V = new Point3D * [nIndex];
//	for (int i = 0; i < nIndex; i++)
//		V[i] = new Point3D[nIndex];
//	ReadCurvedVertex(V, tStep);
//
//	//为了在一个曲面片上求出最小的t，我们需要一个存储数据的数组，多大呢?
//	//对于茶壶来说应该是2个足够了（万恶的壶边），但对复杂的情况，可能需要较多的数据点（重复弯曲的曲线）（感觉其实最多击中三个点）
//	//以上仅针对三次bezier
//
//	ShadeRec sr1(sr.w), sr2(sr.w), sr3(sr.w), sr4(sr.w);
//	ShadeRec unit_sr[] = { sr1,sr2,sr3,sr4 };
//	int t_count = 0;//用来记录最多击中几个三角形
//
//	float tmin = 1e20f;
//	Point3D hitPoint;
//	Vector3D normal;
//	bool if_hit = false;
//
//	for (int i = 0; i < nIndex - 1; i++)
//	{
//		for (int j = 0; j < nIndex - 1; j++)
//		{
//			CP3 P[4] = { V[i][j],V[i + 1][j],V[i + 1][j + 1],V[i][j + 1] };
//			float t = 1e20f;
//			Triangle* pFill = new Triangle(P[0], P[2], P[1]);
//			if (pFill->hit(ray, t, unit_sr[t_count])) {
//				unit_sr[t_count].hit_an_object = true;
//				unit_sr[t_count].t = t;
//				t_count++;
//			}
//			delete pFill; pFill = NULL;
//
//			Triangle* pFill1 = new Triangle(P[0], P[3], P[2]);//要是同时击中两个三角形就见鬼了。。。
//			if (pFill1->hit(ray, t, unit_sr[t_count])) {//以防外一，以防万一
//				unit_sr[t_count].hit_an_object = true;
//				unit_sr[t_count].t = t;
//				t_count++;
//			}
//			delete pFill1; pFill1 = NULL;
//		}
//	}
//	if (V) {
//		for (int i = 0; i < nIndex; i++) {
//			delete[] V[i]; V[i] = NULL;
//		}
//		delete[] V; V = NULL;
//	}
//
//	//此时unit_sr中记录了最多击中的次数
//	for(int i=0;i<sizeof(unit_sr)/sizeof(ShadeRec);i++)
//		if (unit_sr[i].hit_an_object && unit_sr[i].t < tmin) {
//			if_hit = true;
//			tmin = unit_sr[i].t;
//			normal = unit_sr[i].normal;
//			hitPoint = unit_sr[i].hit_point;
//		}
//	sr.t = tmin;
//	sr.normal = normal;
//	sr.hit_point = hitPoint;
//	return if_hit;
//}