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
		if (mLevels != 0)
		{
			RenderTarget* rt;
			for (unsigned int i = 0; i < mLevels; ++i)
			{
				rt = new RenderTarget(mWidth,mHeight,PixelUtil::getNumElemBytes(mPixelFormat));
				mRenderTargets.push_back(rt);
			}
		}
		else
		{
			unsigned int w = mWidth;
			unsigned int h = mHeight;
			RenderTarget* rt;
			unsigned levels = 0;
			for (; w == 1 && h == 1;++levels)
			{
				rt = new RenderTarget(w,h,PixelUtil::getNumElemBytes(mPixelFormat));
				mRenderTargets.push_back(rt);
				w /= 2;
				h /= 2;
				w = w==0?1:w;
				h = h==0?1:h;
			}
			mLevels = levels;
		}
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