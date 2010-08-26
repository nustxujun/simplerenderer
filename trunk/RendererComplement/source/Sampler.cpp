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

	float Sampler::collod(const Vector2& ddx, const Vector2& ddy, float lodbias)
	{
		float result = std::max<float>(ddx.dotProduct(ddx),ddy.dotProduct(ddy));
		result = log2(std::max<float>(result,0.000001f) + lodbias);
		return result;
	}

	Colour Sampler::sampleImpl(RenderTarget* rt,float u, float v,const Vector2& ddx, const Vector2& ddy, float lodbias )const
	{
		assignUV(u,v);
		//先不管mipmap
		unsigned int x,y;
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
			THROW_EXCEPTION("Unexpected pixel format.");
		}

		return color;	
	}

	Colour Sampler::sample(float u, float v,const Vector2& ddx,const Vector2& ddy, float lodbias)const 
	{
		if (texture == NULL)
			return Colour(1.0f);

		assignUV(u,v);
		//先不管mipmap
		unsigned int x,y;
		RenderTarget* rt = texture->getRenderTarget(0);
		return sampleImpl(rt,u,v,ddx,ddy,lodbias);
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
		mTextureState = state;
	}

	Colour Sampler::sampleCube(float u, float v, float w, const Vector2& ddx, const Vector2& ddy, float lodbias )const
	{
		int face = 0;
		float s, t, m;

		float x = u;
		float y = v;
		float z = w;

		float ax = abs(x);
		float ay = abs(y);
		float az = abs(z);

		if(ax > ay && ax > az)
		{
			// x max
			m = ax;
			if(x > 0){
				//+x
				s = 0.5f * (z / m + 1.0f);
				t = 0.5f * (y / m + 1.0f);
				face = 0;
			} else {
				//-x

				s = 0.5f * (-z / m + 1.0f);
				t = 0.5f * (y / m + 1.0f);
				face = 1;
			}
		} else {

			if(ay > ax && ay > az){
				m = ay;
				if(y > 0){
					//+y
					s =0.5f * (x / m + 1.0f);
					t = 0.5f * (z / m + 1.0f);
					face = 2;
				} else {
					s = 0.5f * (x / m + 1.0f);
					t = 0.5f * (-z / m + 1.0f);
					face = 3;
				}
			} else {
				m = az;
				if(z > 0){
					//+z
					s = 0.5f * (-x / m + 1.0f);
					t = 0.5f * (y / m + 1.0f);
					face = 4;
				} else {
					s = 0.5f * (x / m + 1.0f);
					t = 0.5f * (y / m + 1.0f);
					face = 5;
				}
			}
		}

		RenderTarget* rt = texture->getRenderTarget(0,face);
		return sampleImpl(rt,s,t,ddx,ddy,lodbias);
	}

}