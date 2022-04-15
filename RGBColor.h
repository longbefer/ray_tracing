#pragma once

class RGBColor
{
public:
	RGBColor(void);
	virtual ~RGBColor(void);
	RGBColor(double, double, double, double = 0.0f);
	friend RGBColor operator+(const RGBColor&, const RGBColor&);//运算符重载
	friend RGBColor operator-(const RGBColor&, const RGBColor&);
	friend RGBColor operator*(const RGBColor&, const RGBColor&);
	friend RGBColor operator*(const RGBColor&, double);
	friend RGBColor operator*(double, const RGBColor&);
	friend RGBColor operator/(const RGBColor&, double);
	friend RGBColor operator+=(RGBColor&, RGBColor&);
	friend RGBColor operator-=(RGBColor&, RGBColor&);
	friend RGBColor operator*=(RGBColor&, RGBColor&);
	friend RGBColor operator/=(RGBColor&, double);
	friend RGBColor operator*=(RGBColor&, double);
	RGBColor pow_color(float inv_gamma);
	void   Normalize();	//归一化到[0,1]区间
public:
	double R, G, B;
	double alpha;
};

#define black RGBColor(0,0,0)
#define white RGBColor(1,1,1)
#define red RGBColor(1,0,0)
#define green RGBColor(0,1,0)
#define blue RGBColor(0,0,1)

typedef RGBColor CRGBA;