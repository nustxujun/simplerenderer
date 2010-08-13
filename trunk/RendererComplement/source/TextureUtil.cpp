#include "TextureUtil.h"
#include "Renderer.h"
#include "Texture.h"
#include "RenderTarget.h"
#include <FreeImage.h>

namespace RCP
{
	Texture* TextureUtil::loadTextureFromFile(Renderer* renderer, const std::string& fileName,unsigned int mipLevels,PixelFormat pf)
	{
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		FIBITMAP *dib(0);
		BYTE* bits(0);
		unsigned int width(0), height(0);
		Texture* tex = 0;

		fif = FreeImage_GetFileType(fileName.c_str(), 0);
		//if still unknown, try to guess the file format from the file extension
		if(fif == FIF_UNKNOWN) 
			fif = FreeImage_GetFIFFromFilename(fileName.c_str());
		//if still unkown, return failure
		if(fif == FIF_UNKNOWN)
			assert(0);

		//check that the plugin has reading capabilities and load the file
		if(FreeImage_FIFSupportsReading(fif))
			dib = FreeImage_Load(fif, fileName.c_str());
		//if the image failed to load, return failure
		if(dib == NULL)
			assert(0);

		//retrieve the image data
		bits = FreeImage_GetBits(dib);
		//get the image width and height
		width = FreeImage_GetWidth(dib);
		height = FreeImage_GetHeight(dib);
		//if this somehow one of these failed (they shouldn't), return failure
		if((bits == 0) || (width == 0) || (height == 0))
			assert(0);

		//
		FREE_IMAGE_TYPE imageType = FreeImage_GetImageType(dib);

		switch(imageType)
		{
		case FIT_UNKNOWN:
		case FIT_COMPLEX:
		case FIT_UINT32:
		case FIT_INT32:
		case FIT_DOUBLE:

			//unsupported
			assert(0);
				
			break;
		case FIT_RGBAF:
			if (PixelUtil::getNumElemBytes(pf) != 4)
				assert(0);
			break;
		case FIT_BITMAP:
			{
				FIBITMAP *newdib = FreeImage_ConvertTo32Bits(dib);
				FreeImage_Unload(dib);
				dib = newdib;
				bits = FreeImage_GetBits(dib);
			
			}
			break;
		default :
			assert(0);
		}

		tex = renderer->createTexture(width, height,mipLevels,pf);
		//get real level count;
		mipLevels = tex->getLevelCount();

		unsigned int w,h;
		RenderTarget* rt;
		size_t pos;
		w =  width;
		h = height;
		unsigned int elemBytes = PixelUtil::getNumElemBytes(pf);
		unsigned int wSize = w * elemBytes;
		for (unsigned int i = 0; i < mipLevels; ++i)
		{
			rt = tex->getRenderTarget(i);
			for (int h = height - 1; h >= 0; --h)
			{
				pos = w * h * elemBytes;
				rt->write( bits + pos, wSize);
			}
		}
		


		//Free FreeImage's copy of the data
		FreeImage_Unload(dib);

		return tex;

	}

}