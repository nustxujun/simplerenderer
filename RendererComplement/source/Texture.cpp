#include "Texture.h"
#include "RenderTarget.h"

namespace RCP
{
	Texture::~Texture()
	{
		if (mRenderTargets.empty())
			return;
		RenderTargets::iterator i,endi = mRenderTargets.end();
		for (i = mRenderTargets.begin(); i != endi; ++i )
		{
			delete (*i);
		}
	}

		
	Texture::Texture(unsigned int width, unsigned int height, unsigned int levels, PixelFormat pf, TextureManager* tm):
		mWidth(width),mHeight(height),mLevels(levels),mPixelFormat(pf),Resource((ResourceManager*)tm)	
	{
		
	}

	void Texture::initImpl() 
	{
		unsigned int w = mWidth;
		unsigned int h = mHeight;
		
		if (mLevels == 0)
			mLevels = -1;

		RenderTarget* rt;
		unsigned int lev = 0;
		while ( !(w ==0 && h == 0) && lev < mLevels)
		{
			w = w==0?1:w;
			h = h==0?1:h;

			rt = new RenderTarget(w,h,PixelUtil::getNumElemBytes(mPixelFormat));
			mRenderTargets.push_back(rt);
			w /= 2;
			h /= 2;

			++lev; 
		}
		mLevels = lev;
	}

	RenderTarget* Texture::getRenderTarget(unsigned int level)const
	{
		assert( level < mLevels);
		assert(isInitialized());

		return mRenderTargets[level];

	}

	PixelFormat Texture::getPixelFormat()const 
	{
		return mPixelFormat;
	}

	unsigned int Texture::getWidth()const
	{
		return mWidth;
	}

	unsigned int Texture::getHeight()const
	{
		return mHeight;
	}

	unsigned int Texture::getLevelCount()const
	{
		return mLevels;
	}

}