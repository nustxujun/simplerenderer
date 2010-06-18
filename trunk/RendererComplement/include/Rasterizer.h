#ifndef _Rasterizer_H_
#define _Rasterizer_H_

#include "Prerequisites.h"
#include "Vector4.h"

namespace RCP
{

	struct Vertex
	{
		Vector4 pos;
		Vector3 norm;
		Vector4 color;
		Vector2 texCrood[8];
		Vertex()
		{}

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
		//是正三角1，还是倒三角-1，还是没分0
		int triType;

		Primitive():
			type(ERROR),
			triType(0)
		{
			memset(tex,0,sizeof(Texture*) * 8);
		}

		const Primitive& operator = (const Primitive& prim)
		{
			type = prim.type;
			triType = prim.triType;
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

	class Rasterizer
	{
	public :
		Rasterizer();
		virtual ~Rasterizer();
		void initialize(unsigned int width, unsigned int height, PixelFormat pf );

		void pushPrimitive(const Primitive& pri);
		void flush(RenderTarget* target);

		void clear();

	protected:
		inline bool fequal(float a, float b,float epslon)
		{
			float c = a - b;
			if (c > -epslon && c < epslon)
				return true;
			else
				return false;
		}

		inline unsigned int ceil(float a)
		{
			int i = (int)a;
			if (a > i +0.01)
				return i + 1;
			else
				return i;
		}

		void drawPoint(const Primitive& pri);
		void drawLine(const Primitive& pri);
		void drawTriangle(const Primitive& pri);

		void drawImpl(const Vertex& v);
		bool colorTest(const Vertex& v);

		template<class T>
		void interpolate(T& output,float input0, float input1, float inputx, const T& value0, const T& value1);

		template<class T>
		void interpolate(T& output,float ratio, const T& value0, const T& value1);

		Vector4 addressTex(const Texture* tex,float u,float v);
		

		void copyToTarget(RenderTarget* target);

	protected:

		RenderTarget* mTempBuffer;

		typedef std::vector<Primitive> PrimitiveVector;
		PrimitiveVector mPrimitiveVector;


	};
}
#endif//_Rasterizer_H_
