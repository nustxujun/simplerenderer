#include "Material.h"

namespace RCP
{
	const Material Material::DEFAULT(Colour(1.f,1.f,1.f,0.f),Colour(0.f),Colour(0.f),Colour(0.f),0.0f);

	Material::Material():
		mUsingDiffuseVertexColor(true),
		mUsingSpecularVertexColor(false)
	{}

	Material::Material(const Colour& d,const Colour& a,const Colour& s, const Colour& e,float p):
		diffuse(d),ambient(a),specular(s),emissive(e),power(p),mUsingDiffuseVertexColor(true),mUsingSpecularVertexColor(false)
	{
	
	}

	Material::~Material()
	{}

	void Material::setDiffuseVertexColorEnable(bool enable)
	{
		mUsingDiffuseVertexColor = enable;
	}

	void Material::setSpecularVertexColorEnable(bool enable)
	{
		mUsingSpecularVertexColor = enable;
	}

	bool Material::isDiffuseVertexColorEnable()const
	{
		return mUsingDiffuseVertexColor;
	}

	bool Material::isSpecularVertexColorEnable()const
	{
		return mUsingSpecularVertexColor;
	}
}
