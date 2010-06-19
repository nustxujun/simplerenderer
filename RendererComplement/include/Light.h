#ifndef _Light_H_
#define _Light_H_

#include "Prerequisites.h"
#include "Colour.h"
#include "Vector4.h"
namespace RCP
{
	enum LightType
	{
		LT_POINT = 1,
		LT_SPOT = 2,
		LT_DIRECTIONAL = 3,
	};

	class Light
	{
	public:
		Light();
		virtual ~Light();

		void setEnable(bool enable);
		bool isEnable()const ;
	public:
		LightType type;
		Colour diffuse;
		Colour specular;
		Colour ambient;
		Vector4 position;
		Vector4 direction;
		float range;
		float falloff;
		float attenuation0;
		float attenuation1;
		float attenuation2;
		float theta;
		float phi;

	protected:
		bool mEnable;
		

	};
}
#endif//_Light_H_
