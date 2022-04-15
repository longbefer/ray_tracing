#include "pch.h"
#include "Texture.h"

Texture::Texture(void)
{
	image = NULL;
}

Texture::~Texture(void)
{
}

void Texture::GetImage(UINT nIDResource)
{
	CBitmap NewBitmap;
	NewBitmap.LoadBitmapW(nIDResource);
	NewBitmap.GetBitmap(&bmp);
	int nbytesize = bmp.bmWidthBytes * bmp.bmHeight;
	image = new BYTE[nbytesize];
	NewBitmap.GetBitmapBits(nbytesize, (LPVOID)image);
}

RGBColor Texture::GetTexture(double u, double v)//uv要在0-1之间啊
{
	/*防止越界*/
	int pic_u = u * bmp.bmWidth, pic_v = v * bmp.bmHeight;
	if (pic_u < 0) pic_u = 0; if (pic_v < 0) pic_v = 0;
	if (pic_u > bmp.bmWidth - 1) pic_u = bmp.bmWidth - 1;
	if (pic_v > bmp.bmHeight - 1) pic_v = bmp.bmHeight - 1;
	/*查找对应纹理空间的颜色值*/
	pic_v = bmp.bmHeight - 1 - pic_v;//交换v，图片uv和创建uv方向不一致
	int position = pic_v * bmp.bmWidthBytes + 4 * pic_u;//颜色分量位置
	COLORREF color = RGB(image[position + 2], image[position + 1], image[position]);//获取颜色值
	return  RGBColor(GetRValue(color) / 255.0, GetGValue(color) / 255.0, GetBValue(color) / 255.0);
}

void Texture::DeleteObject(void)
{
	if (image) {
		delete[] image;
		image = NULL;
	}
}
