#include "pch.h"
#include "Camera.h"
#include <thread>
#include <mutex>

Camera::Camera(void)
{
	eye = Point3D();
	lookat = Point3D();
	up = Vector3D(0, 1, 0);
	u = v = w = Vector3D();
	exposure_time = 1.0f;
}

Point3D Camera::get_eye()
{
	return eye;
}

void Camera::set_eye(Point3D& eye)
{
	this->eye = eye;
}

void Camera::set_lookat(Point3D& lookat)
{
	this->lookat = lookat;
}

void Camera::computer_uvw(void)
{
	w = eye - lookat;
	w = w.Normalize();
	u = Cross(up, w);			//叉乘
	u = u.Normalize();
	v = Cross(w, u);

	if (eye.x == lookat.x && eye.z == lookat.z && lookat.y < eye.y)//相机垂直向下观察
	{
		u = Vector3D(0, 0, 1);
		v = Vector3D(1, 0, 0);
		w = Vector3D(0, 1, 0);
	}
	if (eye.x == lookat.x && eye.z == lookat.z && lookat.y > eye.y)//相机垂直向上观察
	{
		u = Vector3D(1, 0, 0);
		v = Vector3D(0, 0, 1);
		w = Vector3D(0, -1, 0);
	}
}

Pinhole::Pinhole(void)
{
	d = 0.0f;
	zoom = 1.0f;
}

Vector3D Pinhole::ray_direction(const Point3D& p) const
{
	Vector3D dir = p.x * u + p.y * v - d * w;
	dir = dir.Normalize();
	return dir;
}

void Pinhole::render_scene(World& w)
{
	RGBColor L;
	ViewPlane vp(w.vp);
	Ray ray;
	int depth = 0;
	Point3D sp;
	Point3D pp;

	vp.s /= zoom;//变焦，将zoom变大，将物体放大
	ray.o = eye;

	//设置采样点
	vp.sampler_ptr->generate_samples();
	vp.sampler_ptr->setup_shuffled_indices();
	vp.sampler_ptr->shuffle_samples();

	for (int r = 0; r < vp.vres; r++)
		for (int c = 0; c < vp.hres; c++) {
			L = w.background_color;

			for (int j = 0; j < vp.num_samples; j++) {
				//设置采样
				sp = vp.sampler_ptr->sample_unit_square();
				pp.x = vp.s * (c - 0.5 * vp.hres + sp.x);//可将sp.x和sp.y改为rand_float()
				pp.y = vp.s * (r - 0.5 * vp.vres + sp.y);

				ray.d = ray_direction(pp);
				L += w.tracer_ptr->trace_ray(ray, depth);
			}
			L /= vp.num_samples;
			L *= exposure_time;
			w.display_pixel(r, c, L);
		}
}

void Pinhole::set_view_distance(float vpd)
{
	this->d = vpd;
}

void ThreadCamera::render_scene(World& w)
{
	ViewPlane vp(w.vp);
	std::mutex mux;  // 加锁
	//RGBColor** cls = ; // 若将其作为全局变量将会拖慢速度，可能不会吧。。。（因为CPU会比对是否更改，但是可能cache中不能存放这么多吧。。。）
	save_color = new BYTE[vp.vres * vp.hres * 4ULL]{}; 

	// 设置可以并发执行的个数
	//auto c = std::thread(render_line, 9);
	//int maxCurrency = t.hardware_concurrency();
	std::thread t1([&]() {
		render_line(w, mux, 0, vp.vres / 4);
	});

	std::thread t2([&]() {
		render_line(w, mux, vp.vres / 4, vp.vres / 2);
	});

	std::thread t3([&]() {
		render_line(w, mux, vp.vres / 2, 3 * vp.vres / 4);
	});

	std::thread t4([&]() {
		render_line(w, mux, 3 * vp.vres / 4, vp.vres);
	});

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	// 将其全部传输到
	//for (int i = 0; i < vp.vres; ++i)
	//	for (int j = 0; j < vp.hres; ++j)
	//		w.display_pixel(i, j, save_color[i][j]);
	//while (!t1.joinable() || !t2.joinable()
	//	|| !t3.joinable() || !t4.joinable()) {}
	CDC MemDC;
	MemDC.CreateCompatibleDC(w.pDC);
	CBitmap NewBitmap;
	NewBitmap.CreateBitmap(vp.hres, vp.vres, 1, 32, save_color);
	MemDC.SelectObject(&NewBitmap);
	w.pDC->BitBlt(0, 0, vp.hres, vp.vres, &MemDC, 0, 0, SRCCOPY);
	NewBitmap.DeleteObject();

	delete[] save_color;
}

void ThreadCamera::render_line(World& w, std::mutex& mut, const int& start, const int& end)
{
	RGBColor L;
	ViewPlane vp(w.vp);
	/* 由于多线程，这个最好不要使用World的指针，否则会错误 */
	//Sampler* sample_ptr = new Sampler(vp.num_samples);
	std::unique_ptr<Sampler> sampler_ptr(new Sampler(vp.num_samples));
	vp.sampler_ptr = sampler_ptr.get();
	Ray ray;
	int depth = 1;
	Point3D sp;
	Point3D pp;

	//vp.s /= zoom;//变焦，将zoom变大，将物体放大
	ray.o = eye;

	//设置采样点
	vp.sampler_ptr->generate_samples();
	vp.sampler_ptr->setup_shuffled_indices();
	vp.sampler_ptr->shuffle_samples();

	for (int r = start; r < end; r++)
		for (int c = 0; c < vp.hres; c++) {
			L = w.background_color;

			for (int j = 0; j < vp.num_samples; j++) {
				//设置采样
				sp = vp.sampler_ptr->sample_unit_square();
				pp.x = vp.s * (c - 0.5 * vp.hres + sp.x);//可将sp.x和sp.y改为rand_float()
				pp.y = vp.s * (r - 0.5 * vp.vres + sp.y);

				ray.d = ray_direction(pp);
				std::lock_guard<std::mutex>  lock(mut);
				//mut.lock();
				L += w.tracer_ptr->trace_ray(ray, depth);
				//mut.unlock();
			}
			L /= vp.num_samples;
			L *= exposure_time;
			//mut.lock();
			//w.display_pixel(r, c, L);
			//mut.unlock();
			DWORD dwIndex = (vp.hres - r - 1) * vp.hres * 4 + 4 * c;
			BYTE* pClr = (save_color + dwIndex);
			BYTE RED = (BYTE)(L.R * 255);
			BYTE GREEN = (BYTE)(L.G * 255);
			BYTE BLUE = (BYTE)(L.B * 255);
			BYTE ALPHA = (BYTE)(L.alpha * 255);
			*pClr++ = BLUE;
			*pClr++ = GREEN;
			*pClr++ = RED;
			*pClr++ = ALPHA;
		}
}
