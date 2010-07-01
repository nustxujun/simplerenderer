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

	//void Texture::assignUV(float& u,float& v)const 
	//{
	//	TextureAddresingMode mode[2];
	//	mode[0] = mTextureState.addresingModeU;
	//	mode[1] = mTextureState.addresingModeV;

	//	float value[2];
	//	value[0] = u;	
	//	value[1] = v;
	//	int vn;
	//	float d;
	//	for (int i = 0; i < 2; ++i)
	//	{
	//		if (value[i] < 1.0f && value[i] >= 0.0f)
	//			continue;
	//		vn = (int)value[i];
	//		d = value[i] - vn;
	//		if (d < 0)
	//			d = 1.0f + d;
	//		vn %= 2;
	//		switch (mode[i])
	//		{
	//		case TAM_WRAP:
	//			{
	//				
	//				value[i] = d;
	//				break;
	//			}
	//		case TAM_MIRROR:
	//			{
	//				if (vn == 0)
	//					value[i] = 1 - d;
	//				else
	//					value[i] =d;
	//				break;
	//			}
	//		case TAM_CLAMP:
	//			{
	//				if (value[i] > 1.0f)
	//					value[i] = 1;
	//				if (value[i] < 0.f)
	//					value[i] = 0;
	//				break;
	//			}
	//		case TAM_BORDER:
	//			{
	//				if (value[i] > 1.0f || value[i] < 0.f)
	//					value[i] = 0;
	//				
	//				break;
	//			}
	//		case TAM_MIRRORONCE:
	//			{
	//				if (value[i] > 1.0f || value[i] < 0.f)
	//					value[i] = 1 - d;
	//				
	//				break;
	//			}
	//		}
	//	}


	//	u = value[0];
	//	v = value[1];
	//}


}