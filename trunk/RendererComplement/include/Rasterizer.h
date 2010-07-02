#ifndef _Rasterizer_H_
#define _Rasterizer_H_

#include "Prerequisites.h"
#include "Colour.h"
#include "PipelinePlus.h"
namespace RCP
{
	class Rasterizer
	{

	public :
		Rasterizer();
		virtual ~Rasterizer();
		void initialize(unsigned int width, unsigned int height, PixelFormat pf);

		void pushPrimitive(const Primitive& pri);
		void flush(RenderTarget* target,const RenderState& state);


		template<class T>
		void interpolate(T& output,float input0, float input1, float inputx, const T& value0, const T& value1);

		template<class T>
		void interpolate(T& output,float ratio, const T& value0, const T& value1);

		void setPixelShader(PixelShader* ps);


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
		bool stencilTest(const Pixel& p,bool zTest);
		void stencilOperation(unsigned int& value,unsigned  int ref, StencilOperation op);
		bool alphaTest(const Pixel& p);
		bool depthTest(const Pixel& p);

		void clear(const Colour& color);
		void clear(float z);
		void clearStencil(unsigned int stencil);
		
		template<class T>
		bool compareOperation(const T& value1, const T& value2, CompareFunc func);
	protected:

		RenderTarget* mColorBuffer;
		RenderTarget* mZBuffer;
		RenderTarget* mStencilBuffer;
		PixelShader* mPixelShader;
		Vector4 mScissorRect;
		

		typedef std::vector<Primitive> PrimitiveVector;
		PrimitiveVector mPrimitiveVector;

		RenderState mRenderState;


	};
}
#endif//_Rasterizer_H_
