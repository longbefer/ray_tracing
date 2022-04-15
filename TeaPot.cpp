#include "pch.h"
#include "TeaPot.h"
#include "Box.h"
#include "ShadeRec.h"
#include "SmoothBezier.h"
#include "SmoothBicubicBezierPatch.h"
#include <mutex>
#include <thread>
TeaPot::TeaPot(void)
{
	this->center = Point3D(0, 0, 0);
	this->multi = 20;
	this->Renumber = 3;
	ReadVertex();
	ReadPatch();
}

TeaPot::TeaPot(Point3D& center) {
	this->center = center;
	this->multi = 20;
	this->Renumber = 3;
	ReadVertex();
	ReadPatch();
}

void TeaPot::SetRecursiveNumber(int Renumber)
{
	this->Renumber = Renumber;
}

void TeaPot::SetTeaPotCenter(Point3D center)
{
	this->center = center;
}

void TeaPot::SetMultiple(double multi)
{
	this->multi = multi;
}

void TeaPot::ReadVertex()
{
	CStdioFile file;  //定义文件对象
	if (!file.Open(L"Vertices.txt", CFile::modeRead))
	{
		//MessageBox(L"can not open file!");
		MessageBox(NULL, TEXT("can not open file!"), TEXT("MB_YESNOCANCEL"), MB_YESNOCANCEL);
		return;
	}
	CString  strLine;//定义行字符串
	int index = 0;//用于区分数字
	CString data1, data2, data3;//存放坐标
	for (int i = 0; i < 306; i++)
	{
		file.ReadString(strLine);//按行读入
		for (int j = 0; j < strLine.GetLength(); j++)
		{
			if (' ' == strLine[j])
				index++;
			switch (index)
			{
			case 0:
				data1 += strLine[j];
				break;
			case 1:
				data2 += strLine[j];
				break;
			case 2:
				data3 += strLine[j];
				break;
			}
		}
		P[i].x = _wtof(data1.GetBuffer()) * multi + center.x;//浮点数表示的x值
		P[i].y = _wtof(data3.GetBuffer()) * multi + center.y;//浮点数表示的z值
		P[i].z = _wtof(data2.GetBuffer()) * multi + center.z;//浮点数表示的y值
		strLine = "", data1 = "", data2 = "", data3 = "", index = 0;
	}
	file.Close();
}

void TeaPot::ReadPatch()
{
	CStdioFile file;
	if (!file.Open(L"Patches.txt", CFile::modeRead))
	{
		//MessageBox(L"can not open file!");
		MessageBox(NULL, TEXT("can not open file!"), TEXT("MB_YESNOCANCEL"), MB_YESNOCANCEL);
		return;
	}
	CString strLine;
	int index = 0;
	CString str[16];//存放控制点索引号
	for (int nSurface = 0; nSurface < 32; nSurface++)
	{
		file.ReadString(strLine);
		for (int i = 0; i < strLine.GetLength(); i++)
		{
			if (' ' == strLine[i])
				index++;
			switch (index)
			{
			case 0:
				str[0] += strLine[i];
				break;
			case 1:
				str[1] += strLine[i];
				break;
			case 2:
				str[2] += strLine[i];
				break;
			case 3:
				str[3] += strLine[i];
				break;
			case 4:
				str[4] += strLine[i];
				break;
			case 5:
				str[5] += strLine[i];
				break;
			case 6:
				str[6] += strLine[i];
				break;
			case 7:
				str[7] += strLine[i];
				break;
			case 8:
				str[8] += strLine[i];
				break;
			case 9:
				str[9] += strLine[i];
				break;
			case 10:
				str[10] += strLine[i];
				break;
			case 11:
				str[11] += strLine[i];
				break;
			case 12:
				str[12] += strLine[i];
				break;
			case 13:
				str[13] += strLine[i];
				break;
			case 14:
				str[14] += strLine[i];
				break;
			case 15:
				str[15] += strLine[i];
				break;
			}
		}
		S[nSurface].pNumber = 16;//双三次Bezier曲面片有16个控制点
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				S[nSurface].pIndex[i][j] = _wtoi(str[i * 4 + j].GetBuffer());//整数表示的控制点索引号
		strLine = "";
		for (int nPoint = 0; nPoint < 16; nPoint++)
		{
			str[nPoint] = "";
		}
		index = 0;
	}
	file.Close();
}

bool TeaPot::hit(const Ray& ray, float& tmin, ShadeRec& sr)const
{
	//判断是否击中总的物体，最大点不超过4*multi+center
	//再判断是否击中壶边、壶体等

	Ray temp_ray = Ray(ray.o, ray.d);
	float temp_t = 1e20f;
	Point3D right_top = Point3D(center.x + 3.525 * multi, center.y + 3.15 * multi, center.z + 2 * multi);
	Point3D left_bottom = Point3D(center.x - 3 * multi, center.y - 0 * multi, center.z - 2 * multi);
	Box* box = new Box(right_top, left_bottom);
	if (!(box->shadow_hit(temp_ray, temp_t))) {
		delete box; box = NULL;
		return false;
	}
	delete box; box = NULL;

	float t = 1e20f;
	bool if_hit = false;
	Point3D hit_point;
	Vector3D normal;

	CT2 uv;//保存纹理

	World world_ptr;//临时的

	//临时导入照相机，请用后删除。时间：2020/02/16
	Camera* cam = new Pinhole;
	cam->set_eye(sr.w.camera_ptr->get_eye());
	world_ptr.camera_ptr = cam;

	//ShadeRec sr1(world_ptr), sr2(world_ptr), sr3(world_ptr), sr4(world_ptr), sr5(world_ptr), sr6(world_ptr);
	ShadeRec temp_sr[] = {
		ShadeRec(world_ptr), ShadeRec(world_ptr),
		ShadeRec(world_ptr), ShadeRec(world_ptr),
		ShadeRec(world_ptr), ShadeRec(world_ptr)
	};
	bool hit_body[6] = { 
		//HitRim(temp_ray,temp_sr[0]), HitBody(temp_ray,temp_sr[1]), HitHandle(temp_ray,temp_sr[2]),
		//HitSpout(temp_ray,temp_sr[3]), HitLid(temp_ray,temp_sr[4]), HitBottom(temp_ray,temp_sr[5]) 
	};
	//ShadeRec temp_sr[] = { sr1,sr2,sr3,sr4,sr5,sr6 };

	std::thread t1([&]() {
		hit_body[0] = HitRim(temp_ray, temp_sr[0]);
		hit_body[1] = HitBody(temp_ray, temp_sr[1]);
	});

	std::thread t2([&]() {
		hit_body[2] = HitHandle(temp_ray, temp_sr[2]);
		hit_body[3] = HitSpout(temp_ray, temp_sr[3]);
	});

	std::thread t3([&]() {
		hit_body[4] = HitLid(temp_ray, temp_sr[4]);
		hit_body[5] = HitBottom(temp_ray, temp_sr[5]);
	});
	
	t1.detach();
	t2.detach();
	t3.detach();

	//tmin = t;

	for (int i = 0; i < sizeof(hit_body) / sizeof(bool); i++)
		if (hit_body[i] && temp_sr[i].t < t) {//物体与物体之间的t，比如茶壶体与茶盖之间的t
			if_hit = true;
			t = temp_sr[i].t;
			normal = temp_sr[i].normal;
			uv = CT2(temp_sr[i].u, temp_sr[i].v);
			hit_point = temp_sr[i].hit_point;
		}
	if (if_hit) {
		//sr.t = t;
		tmin = t;
		sr.normal = normal;
		sr.hit_point = hit_point;
		sr.u = uv.u; sr.v = uv.v;
	}
	return if_hit;
}

bool TeaPot::shadow_hit(const Ray& ray, float& tmin)const
{
	Ray temp_ray = Ray(ray.o, ray.d);
	float temp_t = 1e20f;
	Point3D right_top = Point3D(center.x + 3.525 * multi, center.y + 3.15 * multi, center.z + 2 * multi);
	Point3D left_bottom = Point3D(center.x - 3 * multi, center.y - 0 * multi, center.z - 2 * multi);
	Box* box = new Box(right_top, left_bottom);
	if (!(box->shadow_hit(temp_ray, temp_t))) {
		delete box; box = NULL;
		return false;
	}
	delete box; box = NULL;

	float t = 1e20f;
	bool if_hit = false;

	World world_ptr;//临时的
	//ShadeRec sr1(world_ptr), sr2(world_ptr), sr3(world_ptr), sr4(world_ptr), sr5(world_ptr), sr6(world_ptr);
	ShadeRec temp_sr[] = {
	ShadeRec(world_ptr), ShadeRec(world_ptr),
	ShadeRec(world_ptr), ShadeRec(world_ptr),
	ShadeRec(world_ptr), ShadeRec(world_ptr)
	};
	//bool hit_body[] = { HitRim(temp_ray,sr1),HitBody(temp_ray,sr2),HitHandle(temp_ray,sr3),
	//	HitSpout(temp_ray,sr4),HitLid(temp_ray,sr5),HitBottom(temp_ray,sr6) };

	bool hit_body[6] = {
		//HitRim(temp_ray,temp_sr[0]), HitBody(temp_ray,temp_sr[1]), HitHandle(temp_ray,temp_sr[2]),
		//HitSpout(temp_ray,temp_sr[3]), HitLid(temp_ray,temp_sr[4]), HitBottom(temp_ray,temp_sr[5]) 
	};
	//ShadeRec temp_sr[] = { sr1,sr2,sr3,sr4,sr5,sr6 };
	std::thread t1([&]() {
		hit_body[0] = HitRim(temp_ray, temp_sr[0]);
		hit_body[1] = HitBody(temp_ray, temp_sr[1]);
	});

	std::thread t2([&]() {
		hit_body[2] = HitHandle(temp_ray, temp_sr[2]);
		hit_body[3] = HitSpout(temp_ray, temp_sr[3]);
	});

	std::thread t3([&]() {
		hit_body[4] = HitLid(temp_ray, temp_sr[4]);
		hit_body[5] = HitBottom(temp_ray, temp_sr[5]);
	});

	t1.join();
	t2.join();
	t3.join();

	tmin = t;

	for (int i = 0; i < sizeof(hit_body) / sizeof(bool); i++)
		if (hit_body[i] && temp_sr[i].t < t) {//物体与物体之间的t，比如茶壶体与茶盖之间的t
			if_hit = true;
			t = temp_sr[i].t;
			tmin = t;
		}
	return if_hit;

	//return false;
}

bool TeaPot::HitRim(const Ray& ray, ShadeRec& sr)const
{
	//0-63属于壶边的点
	Ray temp_ray = Ray(ray.o, ray.d);
	float temp_t = 1e20f;
	Point3D right_top = Point3D(center.x + 1.5 * multi, center.y + 2.53125 * multi, center.z + 1.5 * multi);
	Point3D left_bottom = Point3D(center.x - 1.5 * multi, center.y + 2.4 * multi, center.z - 1.5 * multi);
	Box* box = new Box(right_top, left_bottom);
	if (!(box->shadow_hit(temp_ray, temp_t))) {
		delete box; box = NULL;
		return false;
	}
	delete box; box = NULL;

	float tmin = 1e20f;
	Point3D hitPoint;
	Vector3D normal;

	CT2 uv;

	CP3 P3[4][4];//三维顶点
	for (int nSurface = 0; nSurface < 4; nSurface++)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				P3[i][j] = P[S[nSurface].pIndex[i][j] - 1];
		//Bezier surf(P3, Renumber);
		//CBicubicBezierPatch surf(P3);
		SmoothBezier surf(P3, Renumber);
		//SmoothBicubicBezierPatch surf(P3);
		if (surf.Hit(ray, sr) && (sr.t < tmin)) {//判断每个面片中的最小值t
			sr.hit_an_object = true;
			tmin = sr.t;
			hitPoint = sr.hit_point;
			normal = sr.normal;
			uv = CT2(sr.u, sr.v);
		}
	}
	if (sr.hit_an_object) {
		sr.t = tmin;
		sr.hit_point = hitPoint;
		sr.normal = normal;
		sr.material_ptr = p_Material;
		sr.u = uv.u; sr.v = uv.v;
		return true;
	}
	return false;
}

bool TeaPot::HitBody(const Ray& ray, ShadeRec& sr)const
{
	//64-191
	Ray temp_ray = Ray(ray.o, ray.d);
	float temp_t = 1e20f;
	Point3D right_top = Point3D(center.x + 2.0 * multi, center.y + 2.4 * multi, center.z + 2.0 * multi);
	Point3D left_bottom = Point3D(center.x - 2.0 * multi, center.y + 0.15 * multi, center.z - 2.0 * multi);
	Box* box = new Box(right_top, left_bottom);
	if (!(box->shadow_hit(temp_ray, temp_t))) {
		delete box; box = NULL;
		return false;
	}
	delete box; box = NULL;

	float tmin = 1e20f;
	Point3D hitPoint;
	Vector3D normal;

	CT2 uv;

	CP3 P3[4][4];//三维顶点
	for (int nSurface = 4; nSurface < 12; nSurface++)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				P3[i][j] = P[S[nSurface].pIndex[i][j] - 1];
		//Bezier surf(P3, Renumber);
		//CBicubicBezierPatch surf(P3);
		SmoothBezier surf(P3, Renumber);
		//SmoothBicubicBezierPatch surf(P3);
		if (surf.Hit(ray, sr) && (sr.t < tmin)) {
			sr.hit_an_object = true;
			tmin = sr.t;
			hitPoint = sr.hit_point;
			normal = sr.normal;
			uv = CT2(sr.u, sr.v);
		}
	}
	if (sr.hit_an_object) {
		sr.t = tmin;
		sr.hit_point = hitPoint;
		sr.normal = normal;
		sr.material_ptr = p_Material;
		sr.u = uv.u; sr.v = uv.v;
		return true;
	}
	return false;
}

bool TeaPot::HitHandle(const Ray& ray, ShadeRec& sr)const
{
	//32-47
	Ray temp_ray = Ray(ray.o, ray.d);
	float temp_t = 1e20f;
	Point3D right_top = Point3D(center.x - 1.5 * multi, center.y + 2.25 * multi, center.z + 0.3 * multi);
	Point3D left_bottom = Point3D(center.x - 3.0 * multi, center.y + 0.6 * multi, center.z - 0.3 * multi);
	Box* box = new Box(right_top, left_bottom);
	if (!(box->shadow_hit(temp_ray, temp_t))) {
		delete box; box = NULL;
		return false;
	}
	delete box; box = NULL;

	float tmin = 1e20f;
	Point3D hitPoint;
	Vector3D normal;

	CT2 uv;

	CP3 P3[4][4];//三维顶点
	for (int nSurface = 12; nSurface < 16; nSurface++)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				P3[i][j] = P[S[nSurface].pIndex[i][j] - 1];
		//Bezier surf(P3, Renumber);
		//CBicubicBezierPatch surf(P3);
		SmoothBezier surf(P3, Renumber);
		//SmoothBicubicBezierPatch surf(P3);
		if (surf.Hit(ray, sr) && (sr.t < tmin)) {
			sr.hit_an_object = true;
			tmin = sr.t;
			hitPoint = sr.hit_point;
			normal = sr.normal;
			uv = CT2(sr.u, sr.v);
		}
	}
	if (sr.hit_an_object) {
		sr.t = tmin;
		sr.hit_point = hitPoint;
		sr.normal = normal;
		sr.material_ptr = p_Material;
		sr.u = uv.u; sr.v = uv.v;
		return true;
	}
	return false;
}

bool TeaPot::HitSpout(const Ray& ray, ShadeRec& sr)const
{
	//48-53
	Ray temp_ray = Ray(ray.o, ray.d);
	float temp_t = 1e20f;
	Point3D right_top = Point3D(center.x + 3.525 * multi, center.y + 2.5125 * multi, center.z + 0.66 * multi);
	Point3D left_bottom = Point3D(center.x + 1.7 * multi, center.y + 0.6 * multi, center.z - 0.66 * multi);
	Box* box = new Box(right_top, left_bottom);
	if (!(box->shadow_hit(temp_ray, temp_t))) {
		delete box; box = NULL;
		return false;
	}
	delete box; box = NULL;

	float tmin = 1e20f;
	Point3D hitPoint;
	Vector3D normal;

	CT2 uv;

	CP3 P3[4][4];//三维顶点
	for (int nSurface = 16; nSurface < 20; nSurface++)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				P3[i][j] = P[S[nSurface].pIndex[i][j] - 1];
		//Bezier surf(P3, Renumber);
		//CBicubicBezierPatch surf(P3);
		SmoothBezier surf(P3, Renumber);
		//SmoothBicubicBezierPatch surf(P3);
		if (surf.Hit(ray, sr) && (sr.t < tmin)) {
			sr.hit_an_object = true;
			tmin = sr.t;
			hitPoint = sr.hit_point;
			normal = sr.normal;
			uv = CT2(sr.u, sr.v);
		}
	}
	if (sr.hit_an_object) {
		sr.t = tmin;
		sr.hit_point = hitPoint;
		sr.normal = normal;
		sr.material_ptr = p_Material;
		sr.u = uv.u; sr.v = uv.v;
		return true;
	}
	return false;
}

bool TeaPot::HitLid(const Ray& ray, ShadeRec& sr)const
{
	//54-69
	Ray temp_ray = Ray(ray.o, ray.d);
	float temp_t = 1e20f;
	Point3D right_top = Point3D(center.x + 1.3 * multi, center.y + 3.15 * multi, center.z + 1.3 * multi);
	Point3D left_bottom = Point3D(center.x - 1.3 * multi, center.y + 2.4 * multi, center.z - 1.3 * multi);
	Box* box = new Box(right_top, left_bottom);
	if (!(box->shadow_hit(temp_ray, temp_t))) {
		delete box; box = NULL;
		return false;
	}
	delete box; box = NULL;

	float tmin = 1e20f;
	Point3D hitPoint;
	Vector3D normal;

	CT2 uv;

	CP3 P3[4][4];//三维顶点
	for (int nSurface = 20; nSurface < 28; nSurface++)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				P3[i][j] = P[S[nSurface].pIndex[i][j] - 1];
		//Bezier surf(P3, Renumber);
		//CBicubicBezierPatch surf(P3);
		SmoothBezier surf(P3, Renumber);
		//SmoothBicubicBezierPatch surf(P3);
		if (surf.Hit(ray, sr) && (sr.t < tmin)) {
			sr.hit_an_object = true;
			tmin = sr.t;
			hitPoint = sr.hit_point;
			normal = sr.normal;
			uv = CT2(sr.u, sr.v);
		}
	}
	if (sr.hit_an_object) {
		sr.t = tmin;
		sr.hit_point = hitPoint;
		sr.normal = normal;
		sr.material_ptr = p_Material;
		sr.u = uv.u; sr.v = uv.v;
		return true;
	}
	return false;
}

bool TeaPot::HitBottom(const Ray& ray, ShadeRec& sr)const
{
	//70-86
	Ray temp_ray = Ray(ray.o, ray.d);
	float temp_t = 1e20f;
	Point3D right_top = Point3D(center.x + 1.5 * multi, center.y + 0.15 * multi, center.z + 1.5 * multi);
	Point3D left_bottom = Point3D(center.x - 1.5 * multi, center.y - 0 * multi, center.z - 1.5 * multi);
	Box* box = new Box(right_top, left_bottom);
	if (!(box->shadow_hit(temp_ray, temp_t))) {
		delete box; box = NULL;
		return false;
	}
	delete box; box = NULL;

	float tmin = 1e20f;
	Point3D hitPoint;
	Vector3D normal;

	CT2 uv;

	CP3 P3[4][4];//三维顶点
	for (int nSurface = 28; nSurface < 32; nSurface++)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				P3[i][j] = P[S[nSurface].pIndex[i][j] - 1];
		//Bezier surf(P3, Renumber);
		//CBicubicBezierPatch surf(P3);
		SmoothBezier surf(P3, Renumber);
		//SmoothBicubicBezierPatch surf(P3);
		if (surf.Hit(ray, sr) && (sr.t < tmin)) {
			sr.hit_an_object = true;
			tmin = sr.t;
			hitPoint = sr.hit_point;
			normal = sr.normal;
			uv = CT2(sr.u, sr.v);
		}
	}
	if (sr.hit_an_object) {
		sr.t = tmin;
		sr.hit_point = hitPoint;
		sr.normal = normal;
		sr.material_ptr = p_Material;
		sr.u = uv.u; sr.v = uv.v;
		return true;
	}
	return false;
}
