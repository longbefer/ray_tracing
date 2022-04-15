#include "pch.h"
#include "Sampler.h"
#include <algorithm>
Sampler::Sampler(int num_samples)
{
	this->num_sets = 1;
	this->count = 0;
	this->jump = 0;
	this->num_samples = num_samples;
}
Sampler::~Sampler(void)
{
	//if (!samples.empty())
	//	samples.erase(samples.begin(), samples.end());
	//if (!samples.empty())
	//	shuffled_indices.erase(shuffled_indices.begin(), shuffled_indices.end());
}
void Sampler::generate_samples(void)
{
	for (int j = 0; j < num_sets; j++)
		for (int i = 0; i < num_samples; i++) {
			set_rand_seed(i);
			samples.push_back(Point3D(rand_float(), rand_float()));//抖动采样
		}
}
void Sampler::setup_shuffled_indices(void)
{
	shuffled_indices.reserve(num_samples * num_sets);
	std::vector<int> indices;
	for (int j = 0; j < num_samples; j++)
		indices.push_back(j);
	for (int p = 0; p < num_sets; p++) {
		std::random_shuffle(indices.begin(), indices.end());
		for (int j = 0; j < num_samples; j++)
			shuffled_indices.push_back(indices[j]);
	}

}

void Sampler::shuffle_samples(void)
{//不清楚这个的作用，我改成将生成的采样点随机排布了
	for (int i = 0; i < samples.size(); i++) {
		set_rand_seed(i);//生成随机种子，不确定是否需要
		int random_num1 = rand_int() % (num_samples * num_sets);
		int random_num2 = rand_int() % (num_samples * num_sets);
		Point3D temp = samples[random_num1];
		samples[random_num1] = samples[random_num2];
		samples[random_num2] = temp;
	}
}

Point3D Sampler::sample_unit_square(void)
{
	if (count % num_samples == 0)
		jump = (rand_int() % num_sets) * num_samples;
	return(samples[jump + count++ % num_samples]);
}

int Sampler::get_num_samples()
{
	return this->num_samples;
}

Regular::Regular(int num_samples)
{
	this->num_samples = num_samples;
}

NRooks::NRooks(int num_sampler)
{
	this->num_samples = num_sampler;
}

void NRooks::generate_samples(void)
{
	for(int p=0;p<num_sets;p++)
		for (int j = 0; j < num_samples; j++) {
			set_rand_seed(j);
			Point3D pv;
			pv.x = (j + rand_float()) / num_samples;
			pv.y = (j + rand_float()) / num_samples;
			samples.push_back(pv);
		}
	shuffle_x_coordinates();
	shuffle_y_coordinates();
}

void NRooks::shuffle_x_coordinates()
{
	for (int p = 0; p < num_sets; p++)
		for (int i = 0; i < num_samples - 1; i++) {
			set_rand_seed(i);
			int target = rand_int() % num_samples + p * num_samples;
			float temp = (float)samples[i + p * num_samples + 1].x;
			samples[i + p * num_samples + 1].x = samples[target].x;
			samples[target].x = temp;
		}
}

void NRooks::shuffle_y_coordinates()
{
	for (int p = 0; p < num_sets; p++)
		for (int i = 0; i < num_samples - 1; i++) {
			set_rand_seed(i);
			int target = rand_int() % num_samples + p * num_samples;
			float temp = (float)samples[i + p * num_samples + 1].y;
			samples[i + p * num_samples + 1].y = samples[target].y;
			samples[target].y = temp;
		}
}

MultiJittered::MultiJittered(int num_samples)
{
	this->num_samples = num_samples;
}

void MultiJittered::generate_samples(void)
{//未找到，以下先调用Jittered的
	int n = (int)sqrt(num_samples);
	for (int p = 0; p < num_sets; p++)
		for (int j = 0; j < n; j++)
			for (int k = 0; k < n; k++) {
				set_rand_seed(k);//在每个像素中，分成n*n个小格，给每个小格采一次样
				Point3D sp((k + rand_float()) / n, (j + rand_float()) / n);
				samples.push_back(sp);
			}
}
