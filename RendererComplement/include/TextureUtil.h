#ifndef _TextureUtil_H_
#define _TextureUtil_H_

#include"Prerequisites.h"


namespace RCP
{
	class TextureUtil
	{
	public :
		static Texture* loadTextureFromFile(Renderer* renderer, const std::string& fileName,unsigned int mipLevels,PixelFormat pf);
		static Texture* loadTextureFromFileInMemory();
	};
}
#endif//_TextureUtil_H_
