#include "Light.h"

namespace RCP
{
	Light::Light():
		type(LT_POINT),
		range(0),
		falloff(0),
		attenuation0(0),
		attenuation1(0),
		attenuation2(0),
		theta(0),
		phi(0),
		mEnable(true)

	{}

	Light::~Light()
	{}

	bool Light::isEnable() const
	{
		return mEnable;
	}

	void Light::setEnable(bool enable)
	{
		mEnable = enable;
	}
}