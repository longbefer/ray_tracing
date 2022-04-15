#include "pch.h"
#include "SmoothBicubicBezierPatch.h"
#include "SmoothTriangle.h"
#include "ShadeRec.h"

SmoothBicubicBezierPatch::SmoothBicubicBezierPatch(CP3 P[4][4])
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			this->P[i][j] = P[i][j];
}


SmoothBicubicBezierPatch::~SmoothBicubicBezierPatch(void)
{
}

void SmoothBicubicBezierPatch::ReadCurvedVertex(PointVector** V, double tStep)const
{
	double M[4][4];//ϵ������Mbe
	M[0][0] = -1; M[0][1] = 3; M[0][2] = -3; M[0][3] = 1;
	M[1][0] = 3; M[1][1] = -6; M[1][2] = 3; M[1][3] = 0;
	M[2][0] = -3; M[2][1] = 3; M[2][2] = 0; M[2][3] = 0;
	M[3][0] = 1; M[3][1] = 0; M[3][2] = 0; M[3][3] = 0;
	CP3 P3[4][4];//���߼����ÿ��Ƶ�����
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			P3[i][j] = P[i][j];
	LeftMultiplyMatrix(M, P3);//ϵ�����������ά�����
	TransposeMatrix(M);//����ת�þ���
	RightMultiplyMatrix(P3, M);//ϵ�������ҳ���ά�����
	double u0, u1, u2, u3, v0, v1, v2, v3;//u��v��������
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
			/////////////////////////////////////�����ķ�ʸ��
			Vector3D Pu = 3 * u2 * (P3[0][0] * v3 + P3[0][1] * v2 + P3[0][2] * v1 + P3[0][3]) +
				2 * u1 * (P3[1][0] * v3 + P3[1][1] * v2 + P3[1][2] * v1 + P3[1][3]) +
				(P3[2][0] * v3 + P3[2][1] * v2 + P3[2][2] * v1 + P3[2][3]);//����u�����������
			Vector3D Pv = 3 * v2 * (P3[0][0] * u3 + P3[1][0] * u2 + P3[2][0] * u1 + P3[3][0]) +
				2 * v1 * (P3[0][1] * u3 + P3[1][1] * u2 + P3[2][1] * u1 + P3[3][1]) +
				(P3[0][2] * u3 + P3[1][2] * u2 + P3[2][2] * u1 + P3[3][2]);//����v�����������
			Vector3D N = Cross(Pu, Pv).Normalize();//P3��ķ�ʸ��
			V[m][n].point = pt;
			V[m][n].vector = N;
			V[m][n++].t = CT2(u, v);
		}
}


bool SmoothBicubicBezierPatch::Hit(const Ray& ray, ShadeRec& sr)const
{
	double tStep = 0.1;//����
	int nIndex = ROUND(1 / tStep) + 1;
	PointVector** V = new PointVector * [nIndex];
	for (int i = 0; i < nIndex; i++)
		V[i] = new PointVector[nIndex];
	ReadCurvedVertex(V, tStep);

	float tmin = 1e20f;
	Point3D hitPoint;
	Vector3D normal;
	CT2 uv;
	bool if_hit = false;

	for (int i = 0; i < nIndex - 1; i++)
	{
		for (int j = 0; j < nIndex - 1; j++)
		{
			ShadeRec temp_sr(sr.w);

			PointVector P[4] = { V[i][j],V[i + 1][j],V[i + 1][j + 1],V[i][j + 1] };

			//��ʱ���٣���ʹ�ú�ɾ����ʱ��2020/01/16
//#ifdef _DEGBUG
			if (sr.w.camera_ptr) //��������������ڼ���
			{
				Vector3D ViewVector(P[0].point, sr.w.camera_ptr->get_eye());//�����ʸ��
				ViewVector = ViewVector.Normalize();//�淶����ʸ��
				double dotproduct = Dot(ViewVector, P[0].vector);
				if (dotproduct < 0) continue;
			}
//#endif


			SmoothTriangle* pFill = new SmoothTriangle(P[0], P[2], P[1]);
			SmoothTriangle* pFill1 = new SmoothTriangle(P[0], P[3], P[2]);
			if (pFill->hit(ray, temp_sr.t, temp_sr) || pFill1->hit(ray, temp_sr.t, temp_sr))//���ֻ��Բ����˵
				if (temp_sr.t < tmin) {
					if_hit = true;
					tmin = temp_sr.t;
					normal = temp_sr.normal;
					uv = CT2(temp_sr.u, temp_sr.v);
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
	sr.u = uv.u; sr.v = uv.v;
	return if_hit;
}

void SmoothBicubicBezierPatch::LeftMultiplyMatrix(double M[][4], CP3 P[][4])const//��˾���M*P
{
	CP3 PTemp[4][4];//��ʱ����
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			PTemp[i][j] = M[i][0] * P[0][j] + M[i][1] * P[1][j] + M[i][2] * P[2][j] + M[i][3] * P[3][j];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			P[i][j] = PTemp[i][j];
}

void SmoothBicubicBezierPatch::RightMultiplyMatrix(CP3 P[][4], double M[][4])const//�ҳ˾���P*M
{
	CP3 PTemp[4][4];//��ʱ����
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			PTemp[i][j] = P[i][0] * M[0][j] + P[i][1] * M[1][j] + P[i][2] * M[2][j] + P[i][3] * M[3][j];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			P[i][j] = PTemp[i][j];
}

void SmoothBicubicBezierPatch::TransposeMatrix(double M[][4])const//ת�þ���
{
	double PTemp[4][4];//��ʱ����
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			PTemp[j][i] = M[i][j];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			M[i][j] = PTemp[i][j];
}