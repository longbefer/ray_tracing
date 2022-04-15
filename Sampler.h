#pragma once
#include <vector>
#include "Point3D.h"

inline int rand_int() {
	return (rand());
}

inline float rand_float() {
	return ((float)rand() / RAND_MAX);
}

inline void set_rand_seed(const int seed) {
	srand(seed);
}

class Sampler //采样是为了减少锯齿等，采样耗费时间，如果太长，可以注释掉相关Sampler函数及其实现
{
public:
	Sampler() {
		num_samples = 25; num_sets = 1; count = 0; jump = 0;
	}
	Sampler(int num_samples);
	~Sampler(void);
	void generate_samples(void);// 生成采样的点
	void setup_shuffled_indices(void);//设置一个随机的index
	void shuffle_samples(void);//将samples随机排列
	Point3D sample_unit_square(void);//返回单位正方形内一个采样点
	int get_num_samples();
protected:
	int num_samples;//n*n
	int num_sets;//暂时不清楚作用
	std::vector<Point3D> samples;
	std::vector<int> shuffled_indices;
	unsigned long count;
	int jump;
};

class Regular :public Sampler {
public:
	Regular(int num_samples);
public:
	void generate_samples(void) {
		for (int j = 0; j < num_sets; j++)
			for (int i = 0; i < num_samples; i++)
				samples.push_back(Point3D(0.5, 0.5));//均匀采样
	}
};

class Jittered : public Sampler
{
public:
	Jittered(int num_samples) {
		this->num_samples = num_samples;
	}
private:
	void generate_samples(void) {
		int n = (int)sqrt(num_samples);
		for(int p = 0; p < num_sets; p++)
			for(int j = 0; j < n; j++)
				for (int k = 0; k < n; k++) {
					set_rand_seed(k);//在每个像素中，分成n*n个小格，给每个小格采一次样
					Point3D sp((k + rand_float()) / n, (j + rand_float()) / n);
					samples.push_back(sp);
				}
	};
};

class NRooks : public Sampler {
public:
	NRooks(int num_sampler);
public:
	void generate_samples(void);
	void shuffle_x_coordinates();
	void shuffle_y_coordinates();
};

class MultiJittered :public Sampler {
public:
	MultiJittered(int num_sampler);
public:
	void generate_samples(void);
};