#include "Rasterizer.h"
#include "Rendertarget.h"
#include "Texture.h"
#include "FrameBuffer.h"
namespace RCP
{
	const float EPSLON = 0.1f;

	Rasterizer::Rasterizer():
	mPixelShader(NULL)
	{

	}

	Rasterizer::~Rasterizer()
	{
	}

	void Rasterizer::initialize(unsigned int width, unsigned int height, PixelFormat pf)
	{

	}

	void Rasterizer::pushPrimitive(const Primitive& pri)
	{
		switch (pri.type)
		{
		case Primitive::PT_POINT:
			mPrimitiveVector.push_back(pri);
			return;
			break;
		case Primitive::PT_LINE:
			mPrimitiveVector.push_back(pri);
			return;
			break;
		case Primitive::PT_TRIANGLE:
			break;
		default:
			assert(0);
		}

		//上下排序
		unsigned int yOrder[3] = {0,1,2};
		//開始排
		if (pri.vertex[0].pos.y > pri.vertex[1].pos.y)
		{
			yOrder[0] = 1;
			yOrder[1] = 0;
		}
		if (pri.vertex[yOrder[1]].pos.y > pri.vertex[2].pos.y)
		{
			yOrder[2] = yOrder[1];
			yOrder[1] = 2;
			if (pri.vertex[yOrder[0]].pos.y > pri.vertex[2].pos.y)
			{
				yOrder[1] = yOrder[0];
				yOrder[0] = 2;
			}
		}
		//終于排完了

		Primitive resultPri = pri;
		for (unsigned int i = 0; i < 3; ++i)
			resultPri.vertex[i] = pri.vertex[yOrder[i]]; 

		//平底
		if (fequal(resultPri.vertex[2].pos.y, resultPri.vertex[1].pos.y,EPSLON))
		{
			if (resultPri.vertex[1].pos.x > resultPri.vertex[2].pos.x)
			{
				resultPri.vertex[1] = pri.vertex[yOrder[2]];
				resultPri.vertex[2] = pri.vertex[yOrder[1]];
			}
			resultPri.triType = 1;
			mPrimitiveVector.push_back(resultPri);
		}
		//平頂
		else if (fequal(resultPri.vertex[0].pos.y, resultPri.vertex[1].pos.y,EPSLON))
		{
			if (resultPri.vertex[0].pos.x > resultPri.vertex[1].pos.x)
			{
				resultPri.vertex[0] = pri.vertex[yOrder[1]];
				resultPri.vertex[1] = pri.vertex[yOrder[0]];
			}
			resultPri.triType = -1;
			mPrimitiveVector.push_back(resultPri);
		}
		//普通
		else
		{
			//计算新顶点
			Vertex newVertex;
			float ratio =( resultPri.vertex[1].pos.y - resultPri.vertex[0].pos.y ) / ( resultPri.vertex[2].pos.y - resultPri.vertex[0].pos.y );

			lerp(newVertex.pos,ratio,resultPri.vertex[0].pos,resultPri.vertex[2].pos);
			newVertex.pos.y = resultPri.vertex[1].pos.y;
			lerp(newVertex.norm,ratio,resultPri.vertex[0].norm,resultPri.vertex[2].norm);
			lerp(newVertex.specular,ratio,resultPri.vertex[0].specular,resultPri.vertex[2].specular);
			for (unsigned int i = 0; i < 8 ; ++i)
			{
				if (mPixelShader || (!mPixelShader && i == 0))
					lerp(newVertex.color[i],ratio,resultPri.vertex[0].color[i],resultPri.vertex[2].color[i]);
				if (pri.sampler[i].texture != NULL)//有纹理的话
					lerp(newVertex.texCrood[i],ratio,resultPri.vertex[0].texCrood[i],resultPri.vertex[2].texCrood[i]);
			}
			//分解成两个平底三角形
			Primitive up,down;
			up = down = resultPri;
			up.vertex[2] = newVertex;
			up.triType = 1;
			down.vertex[0] = down.vertex[1];
			down.vertex[1] = newVertex;
			down.triType = -1;
			bool swap = resultPri.vertex[1].pos.x > newVertex.pos.x;
			//左右排序
			if (swap)
			{
				up.vertex[1] = newVertex;
				up.vertex[2] = resultPri.vertex[1];

				down.vertex[0] = newVertex;
				down.vertex[1] = resultPri.vertex[1];
			}

			mPrimitiveVector.push_back(up);
			mPrimitiveVector.push_back(down);
		}
	}

	void Rasterizer::flush(const FrameBuffer& fb,const RenderState& state)
	{


		mCurrentFrameBuffer = fb;
		mRenderState = state;

		PrimitiveVector::iterator i,endi = mPrimitiveVector.end();
		for (i = mPrimitiveVector.begin(); i != endi; ++i)
		{
			switch(i->type)
			{
			case Primitive::PT_POINT:
				drawPoint(*i);
				break;
			case Primitive::PT_LINE:
				drawLine(*i);
				break;
			case Primitive::PT_TRIANGLE:
				drawTriangle(*i);
				break;
			default:
				assert(0);
			}

			//copyToTarget(target);
			//clear();
		}

		mPrimitiveVector.clear();

	}

	void Rasterizer::drawPoint(const Primitive& pri)
	{
		int sum = 0;
		switch (pri.type)
		{
		case Primitive::PT_POINT:
			sum = 1;
			break;
		case Primitive::PT_LINE:
			sum = 2;
			break;
		case Primitive::PT_TRIANGLE:
			sum = 3;
			break;
		}

		Pixel point;
		float w;
		for (int i = 0; i < sum; ++i)
		{
			point.x = ceil(pri.vertex[i].pos.x);
			point.y = ceil(pri.vertex[i].pos.y);
			point.z = pri.vertex[i].pos.z;
			point.invw = pri.vertex[i].pos.w;
			w = 1.0f/point.invw;
			point.specular = pri.vertex[i].specular * w;
			
				Colour colorBlend;
				for (unsigned int i = 0; i < 8; ++i)
				{
					point.color[i] = pri.vertex[i].color[i] * w;
					if (pri.sampler[i].texture != NULL)
					{
						if (mPixelShader)
							mPixelShader->sampler[i] = pri.sampler[i];
						
						point.u = pri.vertex[i].texCrood[i].x * w;
						point.v = pri.vertex[i].texCrood[i].y * w;
						//先這裡就不混合了，到時候還要給混合公式
						//colorBlend = pri.sampler[i].sample(point.u ,point.v); 
					}
				}
				//這裡也是
				if (pri.sampler[0].texture != NULL)
				point.color[0] *= colorBlend;

				if (mPixelShader != NULL)
				{
					point.color[0] = mPixelShader->shade(point);
				}
				else//加高光运算
				{
					if (pri.sampler[0].texture != NULL)
						point.color[0] *= colorBlend;
					point.color[0] = (point.color[0] + point.specular).saturate();
				}

				drawImpl(point);
		}
	}

	void Rasterizer::drawLine(const Primitive& pri)
	{
		const Vertex& point1 = pri.vertex[0];
		const Vertex& point2 = pri.vertex[1];
		Vertex diff = point2 - point1;
		Vertex point3( point1);
		float height = point2.pos.y - point1.pos.y;
		diff *= 1.0f / height;
		Pixel point;
		float w;
		for (int y =0; y < height; ++y,point3 += diff)
		{
			w = 1.0f /point3.pos.w;

			point.x = ceil(point3.pos.x);
			point.y = ceil(point3.pos.y);
			point.invw = point3.pos.w;
			point.z = point3.pos.z;
			//point3 *= w;
			point.specular = point3.specular * w;


			//临时处理
			Colour colorBlend;
			for (unsigned int i = 0; i < 8; ++i)
			{
				point.color[i] = point3.color[i] * w;
				if (pri.sampler[i].texture != NULL)
				{
					if (mPixelShader)
						mPixelShader->sampler[i] = pri.sampler[i];

					point.u = point3.texCrood[i].x * w;
					point.v = point3.texCrood[i].y * w;
					//先這裡就不混合了，到時候還要給混合公式
					//colorBlend = pri.sampler[i].sample(point.u ,point.v); 
				}
			}
			//這裡也是
			if (pri.sampler[0].texture != NULL)
				point.color[0] *= colorBlend;

			if (mPixelShader != NULL)
			{
				point.color[0] = mPixelShader->shade(point);
			}
			else//加高光运算
			{
				if (pri.sampler[0].texture != NULL)
					point.color[0] *= colorBlend;
				point.color[0] = (point.color[0] + point.specular).saturate();
			}

			drawImpl(point);

		}
		

	}

	void Rasterizer::drawTriangle(const Primitive& pri)
	{
		assert(pri.triType != 0);
		unsigned int offset = ( pri.triType + 1 )/2;
		unsigned int xmin,xmax,ymin,ymax;
		xmin = ceil(pri.vertex[offset].pos.x);
		xmax = ceil(pri.vertex[offset + 1].pos.x);
		ymin = ceil(pri.vertex[0].pos.y);
		ymax = ceil(pri.vertex[2].pos.y);

		assert(xmin <= xmax);
		assert(ymin <= ymax);

		Vertex diff1(pri.vertex[1] - pri.vertex[0]);
		Vertex diff2(pri.vertex[2] - pri.vertex[0]);
		float area = diff2.pos.x * diff1.pos.y - diff1.pos.x * diff2.pos.y;
		if (equal(area,0.0f))
			return;
		float invarea = 1.0f / area;

		Vertex ddx((diff2 * diff1.pos.y - diff1 * diff2.pos.y )*invarea);
		Vertex ddy((diff1 * diff2.pos.x - diff2 * diff1.pos.x  )*invarea);


		float dx1 = pri.vertex[2-offset].pos.x - pri.vertex[0].pos.x;
		dx1 = dx1 / (pri.vertex[2-offset].pos.y - pri.vertex[0].pos.y);

		float dx2 = pri.vertex[2].pos.x - pri.vertex[1-offset].pos.x;
		dx2 = dx2 / (pri.vertex[2].pos.y - pri.vertex[1-offset].pos.y);


		Vertex point1, point2,point3,d3;
		Pixel point;
		float w;
		float x0,x1;
		x0 = pri.vertex[0].pos.x + (ymin - pri.vertex[0].pos.y) * dx1;
		x1 = pri.vertex[1-offset].pos.x + (ymin - pri.vertex[1-offset].pos.y) * dx2;
		xmin = ceil(x0);
		xmax = ceil(x1);

		for (unsigned int y = ymin; y < ymax; ++y )
		{


			point3 = pri.vertex[0];
			point3 +=  ddy * (y - pri.vertex[0].pos.y);
			point3 += ddx * (xmin - pri.vertex[0].pos.x) ;
			
			for (unsigned int x = xmin; x < xmax; ++x)
			{
				
				

				w = 1.0f /point3.pos.w;

				point.x = x;
				point.y = y;
				point.invw = point3.pos.w;
				point.z = point3.pos.z;
				//point3 *= w;
				point.specular = point3.specular * w;


				//临时处理
				Colour colorBlend;
				for (unsigned int i = 0; i < 8; ++i)
				{
					if (point3.color.isUsed(i))
						point.color[i] = point3.color[i] * w;
					if (pri.sampler[i].texture != NULL)
					{
						if (mPixelShader)
							mPixelShader->sampler[i] = pri.sampler[i];
						
						point.u = point3.texCrood[i].x * w;
						point.v = point3.texCrood[i].y * w;
						//先這裡就不混合了，到時候還要給混合公式
						colorBlend = pri.sampler[i].sample(point.u ,point.v,ddx.texCrood[i],ddy.texCrood[i]); 
					}
				}
				//這裡也是
				if (pri.sampler[0].texture != NULL)
				point.color[0] *= colorBlend;

				if (mPixelShader != NULL)
				{
					point.color[0] = mPixelShader->shade(point);
				}
				else//加高光运算
				{
					if (pri.sampler[0].texture != NULL)
						point.color[0] *= colorBlend;
					point.color[0] = (point.color[0] + point.specular).saturate();
				}

				drawImpl(point);
				point3 += ddx  ;
			}

			x0 += dx1;
			x1 += dx2;
			xmin = ceil(x0);
			xmax = ceil(x1);
			
		}

	}

	Colour Rasterizer::getFactor(BlendMode bm,const Colour& srcColour, const Colour& destColour)
	{
		switch (bm)
		{
		case BM_ZERO:
			{
				return Colour(0,0,0,0);
				break;
			}
		case BM_ONE:
			{
				return Colour(1,1,1,1);
				break;
			}
		case BM_SRCCOLOR:
			{
				return srcColour;
				break;
			}
		case BM_INVSRCCOLOR:
			{
				return Colour(1,1,1,1) - srcColour;
				break;
			}
		case BM_SRCALPHA:
			{
				return Colour(srcColour.a,srcColour.a,srcColour.a,srcColour.a);
				break;
			}
		case BM_INVSRCALPHA:
			{
				float i = 1 - srcColour.a;
				return Colour(i,i,i,i);
				break;
			}
		case BM_DESTALPHA:
			{
				return Colour(destColour.a,destColour.a,destColour.a,destColour.a);
				break;
			}
		case BM_INVDESTALPHA:
			{
				float i = 1 - destColour.a;
				return Colour(i,i,i,i);
				break;
			}
		case BM_DESTCOLOR:
			{
				return destColour;
				break;
			}
		case BM_INVDESTCOLOR :
			{
				return Colour(1,1,1,1) - destColour;
				break;
			}
		case BM_SRCALPHASAT :
			{
				float f = std::min<float>(srcColour.a,1 - destColour.a);
				return Colour(f,f,f,1);
				break;
			}
		default:
			assert(0);
			return 0;
		}
	}

	Colour Rasterizer::alphaBlend(const Colour& srcColour, const Colour& destColour)
	{
		if (!mRenderState.alphaBlendEnable)
			return srcColour;

		Colour srcFactor = getFactor(mRenderState.srcBlend, srcColour, destColour);
		Colour destFactor = getFactor(mRenderState.destBlend,srcColour, destColour);
		return srcFactor * srcColour + destFactor * destColour;
	}


	void Rasterizer::drawImpl(const Pixel& p)
	{
		if (!pixelTest(p))
			return;

		Colour destColour;
		unsigned int color;
		mCurrentFrameBuffer.getValue(color,BT_COLOUR,p.x,p.y);
		destColour.getFromARGB(color);

		color = alphaBlend( p.color[0],destColour).get32BitARGB();


		//color = p.color[0].get32BitARGB();
		mCurrentFrameBuffer.setValue(BT_COLOUR,p.x,p.y,color);
	}

	bool Rasterizer::depthTest(const Pixel& p)
	{
		if (!mRenderState.zTestEnable)
			return true;
		float z ;
		mCurrentFrameBuffer.getValue(z,BT_DEPTH,p.x,p.y);
		bool result = p.z <= z;

		if (result && mRenderState.zWriteEnable)
		{
			mCurrentFrameBuffer.setValue(BT_DEPTH,p.x,p.y,p.z);
		}
		return result;

	}

	bool Rasterizer::scissorTest(const Pixel& p)
	{
		if (p.x < mScissorRect.x || p.x > mScissorRect.z ||
			p.y < mScissorRect.y || p.y > mScissorRect.w)
			return false;
		return true;
	}

	bool Rasterizer::alphaTest(const Pixel& p)
	{
		return compareOperation(p.color[0].a, mRenderState.alphaTestRef,mRenderState.alphaTestFunc);
	}

	void Rasterizer::stencilOperation(unsigned int& value,unsigned  int ref, StencilOperation op)
	{
		switch (op)
		{
		case SO_KEEP:
			break;
		case SO_ZERO:
			value = 0;
			break;
		case SO_REPLACE:
			value = ref;
			break;
		case SO_INCRSAT :
			value = (value < mRenderState.stencilMask)?value +1:mRenderState.stencilMask;
			break;
		case SO_DECRSAT :
			value = (value > 0)?value -1:0;
			break;
		case SOP_INVERT :
			value = ~value;
			break;
		case SO_INCR :
			value = (value < mRenderState.stencilMask)?value +1:0;
			break;
		case SO_DECR :
			value = (value > 0)?value -1:mRenderState.stencilMask;
			break;
		}

		value &= mRenderState.stencilWriteMask;
	}

	bool Rasterizer::stencilTest(const Pixel& p,bool zTest)
	{
		unsigned int cur ;
		mCurrentFrameBuffer.getValue(cur,BT_STENCIL,p.x,p.y);

		bool result = compareOperation (cur & mRenderState.stencilMask ,
			mRenderState.stencilRef & mRenderState.stencilMask,mRenderState.stencilTestFunc);
		if (result)
		{
			if (zTest)
				stencilOperation(cur,mRenderState.stencilRef, mRenderState.stencilPass);
			else
				stencilOperation(cur,mRenderState.stencilRef, mRenderState.stencilZFail);

			mCurrentFrameBuffer.setValue(BT_STENCIL,p.x,p.y,cur);
			return true;
		}
		else
		{
			stencilOperation(cur,mRenderState.stencilRef, mRenderState.stencilFail);
			mCurrentFrameBuffer.setValue(BT_STENCIL,p.x,p.y,cur);
			return false;
		}

	}

	bool Rasterizer::pixelTest(const Pixel& p)
	{

		//if (!scissorTest(p))
		//	return false;
		if (!alphaTest(p))
			return false;
		if (mRenderState.zTestEnable && !depthTest(p))
		{
			stencilTest(p,false);
			return false;
		}

		if(!stencilTest(p,true))
			return false;

		return true;
	}



	void Rasterizer::setPixelShader(PixelShader* ps)
	{
		mPixelShader = ps;
	}

	template<class T>
	bool Rasterizer::compareOperation(const T& value1, const T& value2, CompareFunc func)
	{
		switch (func)
		{
		case CF_NEVER:
			{
				return false;
				break;
			}
		case CF_LESS:
			{
				return value1 < value2;
				break;
			}
		case CF_LESSEQUAL:
			{
				return value1 <= value2;
				break;
			}
		case CF_GREATER:
			{
				return value1 > value2;
				break;
			}
		case CF_NOTEQUAL:
			{
				return value1 != value2;
				break;
			}
		case CF_EQUAL:
			{
				return value1 == value2 ;
				break;
			}
		case CF_GREATEREQUAL:
			{
				return value1 >= value2;
				break;
			}
		case CF_ALWAYS:
			{
				return true;
				break;
			}
		default :
			assert(0);
			return false;

		}
	}

}