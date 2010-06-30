#ifndef _Rasterizer_H_
#define _Rasterizer_H_

#include "Prerequisites.h"
#include "Colour.h"
#include "PipelinePlus.h"
namespace RCP
{
	class Rasterizer
	{
	public:
		class PixelShader
		{
		public:
			virtual Colour shade(const Pixel& pixel) = 0;
		};
	public :
		Rasterizer();
		virtual ~Rasterizer();
		void initialize(unsigned int width, unsigned int height, PixelFormat pf );

		void pushPrimitive(const Primitive& pri);
		void flush(RenderTarget* target);


		template<class T>
		void interpolate(T& output,float input0, float input1, float inputx, const T& value0, const T& value1);

		template<class T>
		void interpolate(T& output,float ratio, const T& value0, const T& value1);


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
			if (a > i +0.0001)
				return i + 1;
			else
				return i;
		}

		void drawPoint(const Primitive& pri);
		void drawLine(const Primitive& pri);
		void drawTriangle(const Primitive& pri);
		void drawImpl(const Pixel& p);

		size_t getBufferPos(unsigned int x, unsigned int y, unsigned int width, unsigned int colorDepth);
		bool pixelTest(const Pixel& p);
		bool scissorTest(const Pixel& p);
		bool alphaTest(const Pixel& p);
		bool depthTest(const Pixel& p);


		Colour addressTex(const Texture* tex,float u,float v);

		void clear();

	protected:

		RenderTarget* mColorBuffer;
		RenderTarget* mZBuffer;
		PixelShader* mPixelShader;
		Vector4 mScissorRect;
		float mAlphaRef;

		typedef std::vector<Primitive> PrimitiveVector;
		PrimitiveVector mPrimitiveVector;


	};
}
#endif//_Rasterizer_H_
