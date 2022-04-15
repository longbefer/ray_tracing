#include "pch.h"
#include "RGBColor.h"
#define BOUND(x,a,b) (((x)<(a))?(a):(((x)>(b))?(b):(x)))//½Ø¶Ïº¯Êý

RGBColor::RGBColor(void)
{
	this->R = 0.0f;
	this->G = 0.0f;
	this->B = 0.0f;
	this->alpha = 0.0f;
}

RGBColor::~RGBColor(void)
{
}

RGBColor::RGBColor(double R, double G, double B, double alpha)
{
	this->R = R;
	this->G = G;
	this->B = B;
	this->alpha = alpha;
}

void RGBColor::Normalize()
{
	R = BOUND(R, 0.0f, 1.0f);
	G = BOUND(G, 0.0f, 1.0f);
	B= BOUND(B, 0.0f, 1.0f);
}

RGBColor operator+(const RGBColor& c1, const RGBColor& c2)
{
	RGBColor c;
	c.R = c1.R + c2.R;
	c.G = c1.G + c2.G;
	c.B = c1.B + c2.B;
	return c;
}

RGBColor operator-(const RGBColor& c1, const RGBColor& c2)
{
	RGBColor c;
	c.R = c1.R - c2.R;
	c.G = c1.G - c2.G;
	c.B = c1.B - c2.B;
	return c;
}

RGBColor operator*(const RGBColor& c1, const RGBColor& c2)
{
	RGBColor c;
	c.R = c1.R * c2.R;
	c.G = c1.G * c2.G;
	c.B = c1.B * c2.B;
	return c;
}

RGBColor operator*(const RGBColor& c1, double k)
{
	RGBColor c;
	c.R = k * c1.R;
	c.G = k * c1.G;
	c.B = k * c1.B;
	return c;
}

RGBColor operator*(double k, const RGBColor& c1)
{
	RGBColor c;
	c.R = k * c1.R;
	c.G = k * c1.G;
	c.B = k * c1.B;
	return c;
}

RGBColor operator/(const RGBColor& c1, double k)
{
	RGBColor c;
	c.R = c1.R / k;
	c.G = c1.G / k;
	c.B = c1.B / k;
	return c;
}

RGBColor operator+=(RGBColor& c1, RGBColor& c2)
{
	c1.R = c1.R + c2.R;
	c1.G = c1.G + c2.G;
	c1.B = c1.B + c2.B;
	return c1;
}

RGBColor operator-=(RGBColor& c1, RGBColor& c2)
{
	c1.R = c1.R - c2.R;
	c1.G = c1.G - c2.G;
	c1.B = c1.B - c2.B;
	return c1;
}

RGBColor operator*=(RGBColor& c1, RGBColor& c2)
{
	c1.R = c1.R * c2.R;
	c1.G = c1.G * c2.G;
	c1.B = c1.B * c2.B;
	return c1;
}

RGBColor operator/=(RGBColor& c1, double k)
{
	c1.R = c1.R / k;
	c1.G = c1.G / k;
	c1.B = c1.B / k;
	return c1;
}

RGBColor operator*=(RGBColor& c1, double k)
{
	c1.R = c1.R * k;
	c1.G = c1.G * k;
	c1.B = c1.B * k;
	return c1;
}

RGBColor RGBColor::pow_color(float inv_gamma) {
	RGBColor c1;
	c1.R = pow(this->R, inv_gamma);
	c1.G = pow(this->G, inv_gamma);
	c1.B = pow(this->B, inv_gamma);
	return c1;
}