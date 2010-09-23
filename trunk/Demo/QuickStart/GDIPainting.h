#ifndef _GDIPainting_H_
#define _GDIPainting_H_

/*
	通过GDI+的渲染方式
*/

#include "RendererHeader.h"
#include "PaintingMethod.h"
using namespace RCP;
using namespace Gdiplus;
class GDIPainting:public PaintingMethod
{
public :


	GDIPainting():
		mBitmap(0)
	{}
	~GDIPainting()
	{
		SAFE_DELETE(mBitmap);
	}
	void init(HDC hdc,unsigned int w, unsigned int h,RCP::PixelFormat pf)
	{
		mhdc = hdc;
		mBitmap = new Bitmap(w,h,PixelFormat32bppRGB);
	}
	virtual void paint(const RenderTarget* renderTarget) 
	{
		const RenderTarget* rt = renderTarget;
		size_t w = rt->getWidth();
		size_t h = rt->getHeight();
		//rt->seek(0);
		
		Graphics graphics( mhdc );
		
		Color newColor;

		Rect rect(0,0,w,h);
		BitmapData bmd;
		mBitmap->LockBits(&rect,Gdiplus::ImageLockModeWrite,mBitmap->GetPixelFormat(),&bmd);
		memcpy(bmd.Scan0,rt->getData(),rt->getSizeInBytes());
		mBitmap->UnlockBits(&bmd);
		graphics.DrawImage(mBitmap,0,0);

	}
protected:
	HDC mhdc;
	Bitmap* mBitmap;
};

#endif//_GDIPainting_H_