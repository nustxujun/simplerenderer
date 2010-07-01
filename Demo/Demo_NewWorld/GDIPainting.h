#ifndef _GDIPainting_H_
#define _GDIPainting_H_

/*
	通过GDI+的渲染方式
*/

#include "RendererHeader.h"
#include "PaintingMethod.h"
using namespace RCP;
class GDIPainting:public PaintingMethod
{
public :
	void prepare(HDC hdc)
	{
		mhdc = hdc;
	}
	virtual void paint(const BackBuffer* backbuffer) 
	{
		RenderTarget* rt = backbuffer->getRenderTarget();
		size_t w = rt->getWidth();
		size_t h = rt->getHeight();
		rt->seek(0);
		using namespace Gdiplus;
		Graphics graphics( mhdc );
		
		Color newColor;
		Bitmap bmp(w,h,PixelFormat32bppARGB);
		int c;
		for (int i = 0; i < w; ++i)
		{
			for (int j = 0; j < h; ++j )
			{
				rt->read(&c,rt->getColourDepth());
				newColor.SetValue((c|0xff000000));
				bmp.SetPixel(i,j,newColor);
			}
		}
		graphics.DrawImage(&bmp,0,0);

	}
protected:
	HDC mhdc;
};

#endif//_GDIPainting_H_