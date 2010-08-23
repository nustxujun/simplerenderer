#ifndef _TextureManager_H_
#define _TextureManager_H_

#include "ResourceManager.h"
namespace RCP
{
	class TextureManager:
		public ResourceManager
	{
	public:
		Texture* createTexture(unsigned int width, unsigned int height, unsigned int numMipmap,TextureType type, PixelFormat pf);
	};
}
#endif//_TextureManager_H_
