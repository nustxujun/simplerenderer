#ifndef _PipelinePlus_H_
#define _PipelinePlus_H_

#include"Prerequisites.h"
#include "Colour.h"
#include "Sampler.h"

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
		Colour color[8];
		Colour specular;
		Vector2 texCrood[8];
		Vertex()
		{}

		Vertex operator - (const Vertex& vert)const
		{
			Vertex result;
			result.pos = pos - vert.pos;
			result.norm = norm - vert.norm;
			result.specular = specular - vert.specular;
			for (int i = 0; i < 8; ++i)
			{
				result.color[i] = color[i] - vert.color[i];
				result.texCrood[i] = texCrood[i] - vert.texCrood[i];
			}
			return result;
		}

		void operator *= (float f)
		{
			pos *= f;
			norm *= f;
			specular *= f;
			for (int i = 0; i < 8; ++i)
			{
				color[i] *= f;
				texCrood[i] *= f;
			}
		}

		Vertex operator * (float f)const
		{
			Vertex result;
			result.pos = pos * f;
			result.norm = norm * f;
			result.specular = specular * f;
			for (int i = 0; i < 8; ++i)
			{
				result.color[i] = color[i] * f;
				result.texCrood[i] = texCrood[i] * f;
			}
			return result;
		}

		const Vertex& operator += (const Vertex& vert)
		{
			pos += vert.pos;
			norm += vert.norm;
			specular += vert.specular;
			for (int i = 0; i < 8; ++i)
			{
				
				color[i] += vert.color[i];
				
				texCrood[i] += vert.texCrood[i];
			}
			return *this;
		}




	};

	struct Pixel
	{
		unsigned int x,y;
		float z,invw;
		Colour color[8];
		Colour specular;
		float u,v;
	};

	struct Primitive
	{
		enum PrimitiveType
		{
			PT_POINT,
			PT_LINE,
			PT_TRIANGLE,
			PT_ERROR,
		};
		unsigned int type;

		Vertex vertex[3];
		Sampler sampler[8];
		const Viewport* vp;
		//是正三角1，还是倒三角-1，还是没分0
		int triType;

		Primitive():
			type(PT_ERROR),
			triType(0),
			vp(0)
		{
			
		}

		const Primitive& operator = (const Primitive& prim)
		{
			type = prim.type;
			triType = prim.triType;
			vp = prim.vp;
			if (prim.type == PT_ERROR)
				return *this;
			for (unsigned short i = 0; i < 3; ++i)
			{
				vertex[i] = prim.vertex[i];
			}

			for (short i = 0; i < 8;++i)
				sampler[i] = prim.sampler[i];
			return *this;
		}

	};

	class VertexShader
	{
	public:
		virtual void execute(Vertex& ver) = 0;

		Colour convert(const Vector4& vec)
		{
			Colour c;
			c.r = vec.x;
			c.g = vec.y;
			c.b = vec.z;
			c.a = vec.w;
			return c;
		}

		Colour convert(const Vector3& vec)
		{
			Colour c;
			c.r = vec.x;
			c.g = vec.y;
			c.b = vec.z;
			c.a = 0.0f;
			return c;
		}
	};

	class PixelShader
	{
	public:
		virtual Colour shade(const Pixel& pixel) = 0;
		
		Vector4 convert(const Colour& color)
		{
			Vector4 v;
			v.x = color.r;
			v.y = color.g;
			v.z = color.b;
			v.w = color.a;
			return v;
		}
		
		Sampler sampler[8];
	};

}
#endif//_PipelinePlus_H_
