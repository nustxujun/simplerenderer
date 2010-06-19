#ifndef _Material_H_
#define _Material_H_

#include"Prerequisites.h"
#include "Colour.h"

namespace RCP
{
	class Material
	{
	public:
		const static Material DEFAULT;
	public:
		Material();
		Material(const Colour& d,const Colour& a,const Colour& s, const Colour& e,float p);
		virtual ~Material();

		void setDiffuseVertexColorEnable(bool enable);
		void setSpecularVertexColorEnable(bool enable);

		bool isDiffuseVertexColorEnable()const ;
		bool isSpecularVertexColorEnable()const;

	public:
		Colour diffuse;
		Colour ambient;
		Colour specular;
		Colour emissive;
		float power;
	protected:
		bool mUsingDiffuseVertexColor;
		bool mUsingSpecularVertexColor;

	};
}
#endif//_Material_H_
