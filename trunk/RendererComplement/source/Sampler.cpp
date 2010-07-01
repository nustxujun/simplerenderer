#include "Sampler.h"
#include "Texture.h"
#include "RenderTarget.h"

namespace RCP
{
	Sampler::Sampler():
		texture(0)
	{

	}

	Sampler::~Sampler()
	{

	}


	Colour Sampler::sample(const Vector2& coord)const
	{
		return sample(coord.x,coord.y);

	}

	Colour Sampler::sample(float u, float v)const 
	{
		if (texture == NULL)
			return Colour(1.0f);
		assert( u < 1.0f && v < 1.0f);

		//ÏÈ²»¹Ümipmap
		unsigned int x,y;
		RenderTarget* rt = texture->getRenderTarget(0);
		x = unsigned int ((rt->getWidth()-1) * u);
		y = unsigned int ((rt->getHeight()-1) * v);
		size_t pos = (x + y * rt->getWidth() )* rt->getColourDepth();
		int c;
		rt->seek(pos);
		rt->read(&c,sizeof(int));
		Colour color;
		switch (texture->getPixelFormat())
		{
		case PF_A8R8G8B8 :
		case PF_X8R8G8B8 :
			color.getFromARGB(c);
			break;
		case PF_A8B8G8R8 :
		case PF_X8B8G8R8 :
			color.getFromABGR(c);
			break;
		case PF_B8G8R8A8 :
			color.getFromBGRA(c);
			break;
		case PF_R8G8B8A8 :
			color.getFromRGBA(c);
			break;
		default:
			assert(0);
		}

		return color;
	}


	void Sampler::assignUV(float& u,float& v)const 
	{
		TextureAddresingMode mode[2];
		mode[0] = mTextureState.addresingModeU;
		mode[1] = mTextureState.addresingModeV;

		float value[2];
		value[0] = u;	
		value[1] = v;
		int vn;
		float d;
		for (int i = 0; i < 2; ++i)
		{
			if (value[i] < 1.0f && value[i] >= 0.0f)
				continue;
			vn = (int)value[i];
			d = value[i] - vn;
			if (d < 0)
				d = 1.0f + d;
			vn %= 2;
			switch (mode[i])
			{
			case TAM_WRAP:
				{
					
					value[i] = d;
					break;
				}
			case TAM_MIRROR:
				{
					if (vn == 0)
						value[i] = 1 - d;
					else
						value[i] =d;
					break;
				}
			case TAM_CLAMP:
				{
					if (value[i] > 1.0f)
						value[i] = 1;
					if (value[i] < 0.f)
						value[i] = 0;
					break;
				}
			case TAM_BORDER:
				{
					if (value[i] > 1.0f || value[i] < 0.f)
						value[i] = 0;
					
					break;
				}
			case TAM_MIRRORONCE:
				{
					if (value[i] > 1.0f || value[i] < 0.f)
						value[i] = 1 - d;
					
					break;
				}
			}
		}


		u = value[0];
		v = value[1];
	}


	void Sampler::setTextureState(const TextureState& state)
	{

	}

}