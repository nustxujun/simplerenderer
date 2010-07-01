#ifndef _Texture_H_
#define _Texture_H_

#include"Prerequisites.h"
#include "Resource.h"

namespace RCP
{
	class Texture:
		public Resource
	{
		friend class TextureManager;
	public:
		virtual ~Texture();
		RenderTarget* getRenderTarget(unsigned int level)const;

		PixelFormat getPixelFormat()const ;
		unsigned int getWidth()const;
		unsigned int getHeight()const;
		unsigned int getLevelCount()const;


	protected:
		
		Texture(unsigned int width, unsigned int height, unsigned int levels, PixelFormat pf, TextureManager* tm);
		void initImpl() ;
	private:
		unsigned int mWidth;
		unsigned int mHeight;
		//���Ϊ0��will generate all texture sublevels down to 1 by 1 pixels 
		unsigned int mLevels;
		PixelFormat mPixelFormat;

		typedef std::vector<RenderTarget*> RenderTargets;
		RenderTargets mRenderTargets;


	};
}
#endif//_Texture_H_
