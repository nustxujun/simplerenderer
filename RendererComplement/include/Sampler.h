#ifndef _Sampler_H_
#define _Sampler_H_


#include"Prerequisites.h"
#include "Colour.h"

namespace RCP
{
	class Sampler
	{
	public:
		Sampler();
		~Sampler();

		Colour sample(const Vector2& coord)const;
		Colour sample(float u, float v)const ;

		void assignUV(float& u,float& v)const ;

		void setTextureState(const TextureState& state);
	public:
		const Texture* texture;
	private:
		
		TextureState mTextureState;
	};
}
#endif//_Sampler_H_
