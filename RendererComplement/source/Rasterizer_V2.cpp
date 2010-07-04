#include "Rasterizer_V2.h"
#include "Rendertarget.h"
#include "Texture.h"
#include "FrameBuffer.h"
namespace RCP
{
	const float EPSLON = 0.1f;

	Rasterizer_V2::Rasterizer_V2()
	{
	
	}

	Rasterizer_V2::~Rasterizer_V2()
	{
	}

	void Rasterizer_V2::initialize(unsigned int width, unsigned int height, PixelFormat pf)
	{
		mPixelShader = NULL;
	}

	void Rasterizer_V2::pushPrimitive(const Primitive& pri)
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

		//��������
		unsigned int yOrder[3] = {0,1,2};
		//�_ʼ��
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
		//�K��������

		Primitive resultPri = pri;
		for (unsigned int i = 0; i < 3; ++i)
			resultPri.vertex[i] = pri.vertex[yOrder[i]]; 

		mPrimitiveVector.push_back(resultPri);

		////ƽ��
		//if (fequal(resultPri.vertex[2].pos.y, resultPri.vertex[1].pos.y,EPSLON))
		//{
		//	if (resultPri.vertex[1].pos.x > resultPri.vertex[2].pos.x)
		//	{
		//		resultPri.vertex[1] = pri.vertex[yOrder[2]];
		//		resultPri.vertex[2] = pri.vertex[yOrder[1]];
		//	}
		//	resultPri.triType = 1;
		//	mPrimitiveVector.push_back(resultPri);
		//}
		////ƽ�
		//else if (fequal(resultPri.vertex[0].pos.y, resultPri.vertex[1].pos.y,EPSLON))
		//{
		//	if (resultPri.vertex[0].pos.x > resultPri.vertex[1].pos.x)
		//	{
		//		resultPri.vertex[0] = pri.vertex[yOrder[1]];
		//		resultPri.vertex[1] = pri.vertex[yOrder[0]];
		//	}
		//	resultPri.triType = -1;
		//	mPrimitiveVector.push_back(resultPri);
		//}
		////��ͨ
		//else
		//{
		//	//�����¶���
		//	Vertex newVertex;
		//	float ratio =( resultPri.vertex[1].pos.y - resultPri.vertex[0].pos.y ) / ( resultPri.vertex[2].pos.y - resultPri.vertex[0].pos.y );
		//	
		//	interpolate(newVertex.pos,ratio,resultPri.vertex[0].pos,resultPri.vertex[2].pos);
		//	newVertex.pos.y = resultPri.vertex[1].pos.y;
		//	interpolate(newVertex.norm,ratio,resultPri.vertex[0].norm,resultPri.vertex[2].norm);
		//	interpolate(newVertex.specular,ratio,resultPri.vertex[0].specular,resultPri.vertex[2].specular);
		//	for (unsigned int i = 0; i < 8 ; ++i)
		//	{
		//		if (mPixelShader || (!mPixelShader && i == 0))
		//			interpolate(newVertex.color[i],ratio,resultPri.vertex[0].color[i],resultPri.vertex[2].color[i]);
		//		if (pri.sampler[i].texture != NULL)//������Ļ�
		//			interpolate(newVertex.texCrood[i],ratio,resultPri.vertex[0].texCrood[i],resultPri.vertex[2].texCrood[i]);
		//	}
		//	//�ֽ������ƽ��������
		//	Primitive up,down;
		//	up = down = resultPri;
		//	up.vertex[2] = newVertex;
		//	up.triType = 1;
		//	down.vertex[0] = down.vertex[1];
		//	down.vertex[1] = newVertex;
		//	down.triType = -1;
		//	bool swap = resultPri.vertex[1].pos.x > newVertex.pos.x;
		//	//��������
		//	if (swap)
		//	{
		//		up.vertex[1] = newVertex;
		//		up.vertex[2] = resultPri.vertex[1];

		//		down.vertex[0] = newVertex;
		//		down.vertex[1] = resultPri.vertex[1];
		//	}

		//	mPrimitiveVector.push_back(up);
		//	mPrimitiveVector.push_back(down);
		//}
	}

	void Rasterizer_V2::flush(const FrameBuffer& fb,const RenderState& state)
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

	void Rasterizer_V2::drawPoint(const Primitive& pri)
	{

	}

	void Rasterizer_V2::drawLine(const Primitive& pri)
	{

	}

	void Rasterizer_V2::drawTriangle(const Primitive& pri)
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

		Vertex point1, point2;
		Pixel point3;
		float ratio1,ratio2,ratio3;
		float w;
		for (unsigned int y = ymin; y < ymax; ++y )
		{
			point3.y = y;

			ratio1 = (y - pri.vertex[0].pos.y) / (pri.vertex[2 - offset].pos.y -  pri.vertex[0].pos.y);
			ratio2 = (y - pri.vertex[1 - offset].pos.y) / (pri.vertex[2].pos.y -  pri.vertex[1 - offset].pos.y);

			interpolate(point1.pos,ratio1,pri.vertex[0].pos,pri.vertex[2 - offset].pos);
			interpolate(point2.pos,ratio2,pri.vertex[1 - offset].pos,pri.vertex[2].pos);
			
			interpolate(point1.norm,ratio1,pri.vertex[0].norm,pri.vertex[2 - offset].norm);
			interpolate(point2.norm,ratio2,pri.vertex[1 - offset].norm,pri.vertex[2].norm);
			interpolate(point1.specular,ratio1,pri.vertex[0].specular,pri.vertex[2 - offset].specular);
			interpolate(point2.specular,ratio2,pri.vertex[1 - offset].specular,pri.vertex[2].specular);

			for (unsigned int i = 0; i < 8; ++i)
			{
				if (pri.sampler[i].texture != NULL)
				{
					interpolate(point1.texCrood[i],ratio1,pri.vertex[0].texCrood[i],pri.vertex[2 - offset].texCrood[i]);
					interpolate(point2.texCrood[i],ratio2,pri.vertex[1 - offset].texCrood[i],pri.vertex[2].texCrood[i]);
				
				}
				interpolate(point1.color[i],ratio1,pri.vertex[0].color[i],pri.vertex[2 - offset].color[i]);
				interpolate(point2.color[i],ratio2,pri.vertex[1 - offset].color[i],pri.vertex[2].color[i]);
			}


			xmin = ceil(point1.pos.x);
			xmax = ceil(point2.pos.x);
			for (unsigned int x = xmin; x < xmax; ++x)
			{
				point3.x = x;

				ratio3 = (x - point1.pos.x) / (point2.pos.x -  point1.pos.x);
				//ע�������� 1/w
				interpolate(point3.invw, ratio3,point1.pos.w,point2.pos.w);	
				w = 1.0f / point3.invw;



				interpolate(point3.specular, ratio3,point1.specular,point2.specular);
				point3.specular *= w;
				
				//��Ҫz 
				interpolate(point3.z, ratio3,point1.pos.z,point2.pos.z);	
				
				
				//��ʱ����
				Colour colorBlend;
				for (unsigned int i = 0; i < 8; ++i)
				{
					//ֻ�е���mPixelShader��ʱ���ִ�к����
					if (mPixelShader || (!mPixelShader && i == 0))
					{
						//��ɫ
						interpolate(point3.color[i], ratio3,point1.color[i],point2.color[i]);
						point3.color[i] *= w;
					}
					if (pri.sampler[i].texture != NULL)
					{
						if (mPixelShader)
							mPixelShader->sampler[i] = pri.sampler[i];
						//�����u vʵ���� U/w V/w;
						interpolate(point3.u,ratio3,point1.texCrood[i].x,point2.texCrood[i].x);
						interpolate(point3.v,ratio3,point1.texCrood[i].y,point2.texCrood[i].y);
						point3.u *= w;
						point3.v *= w;
						//���@�e�Ͳ�����ˣ����r��߀Ҫ�o��Ϲ�ʽ
						colorBlend = pri.sampler[i].sample(point3.u ,point3.v); 
					}
				}
				//�@�eҲ��
				if (pri.sampler[0].texture != NULL)
				point3.color[0] *= colorBlend;

				if (mPixelShader != NULL)
				{
					point3.color[0] = mPixelShader->shade(point3);
				}
				else//�Ӹ߹�����
				{
					if (pri.sampler[0].texture != NULL)
						point3.color[0] *= colorBlend;
					point3.color[0] = (point3.color[0] + point3.specular).clamp();
				}

				drawImpl(point3);
			}
		}
		
	}

	void Rasterizer_V2::drawImpl(const Pixel& p)
	{
		if (!pixelTest(p))
			return;

		unsigned int color;
		color = p.color[0].get32BitARGB();
		mCurrentFrameBuffer.setValue(BT_COLOUR,p.x,p.y,color);
	}

	bool Rasterizer_V2::depthTest(const Pixel& p)
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

	bool Rasterizer_V2::scissorTest(const Pixel& p)
	{
		if (p.x < mScissorRect.x || p.x > mScissorRect.z ||
			p.y < mScissorRect.y || p.y > mScissorRect.w)
			return false;
		return true;
	}

	bool Rasterizer_V2::alphaTest(const Pixel& p)
	{
		return compareOperation(p.color[0].a, mRenderState.alphaTestRef,mRenderState.alphaTestFunc);
	}

	void Rasterizer_V2::stencilOperation(unsigned int& value,unsigned  int ref, StencilOperation op)
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

	bool Rasterizer_V2::stencilTest(const Pixel& p,bool zTest)
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

	bool Rasterizer_V2::pixelTest(const Pixel& p)
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



	void Rasterizer_V2::setPixelShader(PixelShader* ps)
	{
		mPixelShader = ps;
	}

	template<class T>
	bool Rasterizer_V2::compareOperation(const T& value1, const T& value2, CompareFunc func)
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