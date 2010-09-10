#ifndef _PipelinePlus_H_
#define _PipelinePlus_H_

#include"Prerequisites.h"
#include "Colour.h"
#include "Sampler.h"
#include "Array.h"

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
		Vector3 tan;
		Vector3 bino;
		//透视除法后，这里将保存 color/w specular/w texCrood/w;
		Array<Colour,8> color;
		Colour specular;
		Array<Vector2,8> texCrood;
		Vertex()
		{}

		inline Vertex operator - (const Vertex& vert)const
		{
			Vertex result;
			result.pos = pos - vert.pos;
			result.norm = norm - vert.norm;
			result.specular = specular - vert.specular;
			result.tan = tan - vert.tan;
			result.bino = tan - vert.bino;
			for (int i = 0; i < 8; ++i)
			{
				if (color.isUsed(i) || vert.color.isUsed(i))
					result.color[i] = color[i] - vert.color[i];
				if (texCrood.isUsed(i) || vert.texCrood.isUsed(i))
					result.texCrood[i] = texCrood[i] - vert.texCrood[i];
			}
			return result;
		}

		inline void operator *= (float f)
		{
			pos *= f;
			norm *= f;
			specular *= f;
			tan *= f;
			bino*= f;
			for (int i = 0; i < 8; ++i)
			{
				if (color.isUsed(i))
					color[i] *= f;
				if (texCrood.isUsed(i))
					texCrood[i] *= f;
			}
		}

		inline Vertex operator * (float f)const
		{
			Vertex result;
			result.pos = pos * f;
			result.norm = norm * f;
			result.specular = specular * f;
			result.tan = tan * f;
			result.bino = bino * f;
			for (int i = 0; i < 8; ++i)
			{
				if (color.isUsed(i))
					result.color[i] = color[i] * f;
				if (texCrood.isUsed(i))
					result.texCrood[i] = texCrood[i] * f;
			}
			return result;
		}

		inline const Vertex& operator += (const Vertex& vert)
		{
			pos += vert.pos;
			norm += vert.norm;
			specular += vert.specular;
			tan += vert.tan;
			bino += vert.bino;
			for (int i = 0; i < 8; ++i)
			{
				
				if (color.isUsed(i) || vert.color.isUsed(i))
					color[i] += vert.color[i];
				if (texCrood.isUsed(i) || vert.texCrood.isUsed(i))
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

	class Shader
	{
	public:
		Matrix4X4 matrix4X4[16];
		Vector4 vector4[16];

	};

	class VertexShader:public Shader
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

	class PixelShader:public Shader
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
		Vertex ddx;
		Vertex ddy;

		Colour tex2D(unsigned int index ,float u, float v)
		{
			assert(index < 8);
			return sampler[index].sample(u,v,ddx.texCrood[index],ddy.texCrood[index]);
		}

		Colour texCube(unsigned int index ,float u, float v, float w)
		{
			assert(index < 8);
			return sampler[index].sampleCube(u,v,w,ddx.texCrood[index],ddy.texCrood[index]);
		}
	};

}
#endif//_PipelinePlus_H_
