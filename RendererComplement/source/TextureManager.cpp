#include "TextureManager.h"
#include "Texture.h"

namespace RCP
{
	Texture* TextureManager::createTexture(unsigned int width, unsigned int height, unsigned int numMipmap,TextureType type, PixelFormat pf)
	{
		Texture* tex = new Texture(width,height, numMipmap + 1 ,type,pf, this);
		tex->initialize();
		tex->addRef();
		add((Resource*)tex);
		return tex;
	}
}