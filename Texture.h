#pragma once
#include "RGBColor.h"
class Texture
{
public:
	Texture(void);
	~Texture(void);
	void GetImage(UINT nIDResource);
	RGBColor GetTexture(double u, double v);//u,vÔÚ0-1Ö®¼ä
	void DeleteObject(void);
public:
	BYTE* image;
	BITMAP bmp;
};

