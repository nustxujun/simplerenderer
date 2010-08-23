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
		RenderTarget* getRenderTarget(unsigned int level,unsigned int face = 0)const;

		PixelFormat getPixelFormat()const ;
		unsigned int getWidth()const;
		unsigned int getHeight()const;
		unsigned int getLevelCount()const;
		unsigned int getDepth()const ;
		unsigned int getFaceCount()const;
		TextureType getTextureType()const;
	protected:
		
		Texture(unsigned int width, unsigned int height, unsigned int levels,TextureType type, PixelFormat pf, TextureManager* tm);
		void initImpl() ;

	private:
		unsigned int mWidth;
		unsigned int mHeight;

		//Èç¹ûÎª0£¬will generate all texture sublevels down to 1 by 1 pixels 
		unsigned int mLevels;
		PixelFormat mPixelFormat;
		TextureType mTextureType;

		typedef std::vector<RenderTarget*> RenderTargets;
		RenderTargets mRenderTargets;


	};
}
#endif//_Texture_H_
