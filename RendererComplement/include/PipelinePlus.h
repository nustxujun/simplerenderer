#ifndef _PipelinePlus_H_
#define _PipelinePlus_H_

#include"Prerequisites.h"
#include "Colour.h"

namespace RCP
{
	class DataCollector
	{
	public:
		template<class T>
		void getData(T& t,const void* data)
		{
			getDataImpl(t,data);
		}
	private:
		void getDataImpl(Vector4& vec4, const void* data);
		void getDataImpl(Vector3& vec3, const void* data);
		void getDataImpl(Vector2& vec2, const void* data);
		void getDataImpl(float& f, const void* data);
		void getDataImpl(int& d, const void* data);
	};

	struct Vertex
	{
		Vector4 pos;
		Vector3 norm;
		//透视除法后，这里将保存 color/w specular/w texCrood/w;
		Colour color;
		Colour specular;
		Vector2 texCrood[8];
		Vertex()
		{}

	};

	struct Pixel
	{
		unsigned int x,y;
		float z,invw;
		Colour color;
		Colour specular;
		float u,v;
	};

	struct Primitive
	{
		enum
		{
			POINT,
			LINE,
			TRIANGLE,
			ERROR,
		};
		unsigned int type;

		Vertex vertex[3];
		Texture* tex[8];
		const Viewport* vp;
		//是正三角1，还是倒三角-1，还是没分0
		int triType;

		Primitive():
			type(ERROR),
			triType(0),
			vp(0)
		{
			memset(tex,0,sizeof(Texture*) * 8);
		}

		const Primitive& operator = (const Primitive& prim)
		{
			type = prim.type;
			triType = prim.triType;
			vp = prim.vp;
			if (prim.type == ERROR)
				return *this;
			for (unsigned short i = 0; i < 3; ++i)
			{
				vertex[i] = prim.vertex[i];
			}

			memcpy(tex,prim.tex,sizeof(Texture*) * 8);
			return *this;
		}

	};
}
#endif//_PipelinePlus_H_
