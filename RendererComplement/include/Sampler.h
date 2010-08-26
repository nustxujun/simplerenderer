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

		Colour sample(float u, float v,const Vector2& ddx, const Vector2& ddy, float lodbias = 0)const ;
		Colour sampleCube(float u, float v, float w, const Vector2& ddx, const Vector2& ddy, float lodbias = 0)const;

		void assignUV(float& u,float& v)const ;

		void setTextureState(const TextureState& state);
	private:
		float collod(const Vector2& ddx, const Vector2& ddy, float lodbias);
		int getColourData(float u, float v, float lod);

		Colour sampleImpl(RenderTarget* rt,float u, float v,const Vector2& ddx, const Vector2& ddy, float lodbias )const;
	public:
		const Texture* texture;
	private:
		
		TextureState mTextureState;
	};
}
#endif//_Sampler_H_
