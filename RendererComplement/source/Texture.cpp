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

		
	Texture::Texture(unsigned int width, unsigned int height, unsigned int levels,TextureType type , PixelFormat pf, TextureManager* tm):
		mWidth(width),mHeight(height),mLevels(levels),mTextureType(type),mPixelFormat(pf),Resource((ResourceManager*)tm)	
	{
		
	}

	void Texture::initImpl() 
	{

		
		if (mLevels == 0)
			mLevels = -1;

		RenderTarget* rt;
		for (unsigned int face = 0; face < getFaceCount(); ++face)
		{
			unsigned int w = mWidth;
			unsigned int h = getTextureType() == TT_CUBE_MAP ? mWidth : mHeight;
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
	}

	RenderTarget* Texture::getRenderTarget(unsigned int level,unsigned int face)const
	{
		if (level >= mLevels)
			THROW_EXCEPTION("level 超出最大层数");
		if (face >= getFaceCount())
			THROW_EXCEPTION("face 超出最大数");
		assert(isInitialized());

		return mRenderTargets[face * mLevels + level];

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

	unsigned int Texture::getFaceCount()const
	{
		return getTextureType() == TT_CUBE_MAP ? 6 : 1;
	}

	TextureType Texture::getTextureType()const
	{
		return mTextureType;
	}

}