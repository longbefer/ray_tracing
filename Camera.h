#pragma once
#include "World.h"
#include <mutex>
class Camera
{
public:

	Camera(void);
	Point3D get_eye();
	void set_eye(Point3D& eye);
	void set_lookat(Point3D& lookat);
	void computer_uvw(void);

	virtual void render_scene(World& w) = 0;

protected:
	Point3D eye;
	Point3D lookat;
	Vector3D up;
	Vector3D u, v, w;
	float exposure_time;
};

class Pinhole : public Camera {
public:
	Pinhole(void);
	Vector3D ray_direction(const Point3D& p)const;
	virtual void render_scene(World& w);
	void set_view_distance(float vpd);
protected:
	float d;
	float zoom;
};

// 多线程设置
class ThreadCamera : public Pinhole {
public: 
	virtual void render_scene(World& w);
private: 
	void render_line(World&, std::mutex&, const int&, const int&);
	//void render_line(const int i);
	BYTE* save_color;
};