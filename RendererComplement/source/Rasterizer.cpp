#include "Rasterizer.h"
#include "Rendertarget.h"
#include "Texture.h"
namespace RCP
{
	const float EPSLON = 0.1f;

	Rasterizer::Rasterizer():
		mColorBuffer(NULL)
	{
	
	}

	Rasterizer::~Rasterizer()
	{
		SAFE_DELETE(mColorBuffer);
		SAFE_DELETE(mZBuffer);
	}

	void Rasterizer::initialize(unsigned int width, unsigned int height, PixelFormat pf)
	{
		//unsigned int colourDepth = PixelUtil::getNumElemBytes(pf);
		//mColorBuffer = new RenderTarget(width, height, colourDepth);
		//z buffer应float32 恩
		mZBuffer = new RenderTarget(width,height,4);
		mScissorRect= Vector4(0,0,(float)width,(float)height);
		mAlphaRef = 0;

		mPixelShader = NULL;
	}

	void Rasterizer::pushPrimitive(const Primitive& pri)
	{
		switch (pri.type)
		{
		case Primitive::POINT:
			mPrimitiveVector.push_back(pri);
			return;
			break;
		case Primitive::LINE:
			mPrimitiveVector.push_back(pri);
			return;
			break;
		case Primitive::TRIANGLE:
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
			
			interpolate(newVertex.pos,ratio,resultPri.vertex[0].pos,resultPri.vertex[2].pos);
			newVertex.pos.y = resultPri.vertex[1].pos.y;
			interpolate(newVertex.norm,ratio,resultPri.vertex[0].norm,resultPri.vertex[2].norm);
			interpolate(newVertex.color,ratio,resultPri.vertex[0].color,resultPri.vertex[2].color);
			interpolate(newVertex.specular,ratio,resultPri.vertex[0].specular,resultPri.vertex[2].specular);
			for (unsigned int i = 0; i < 8 ; ++i)
				if (pri.tex[i] != NULL)//有纹理的话
					interpolate(newVertex.texCrood[i],ratio,resultPri.vertex[0].texCrood[i],resultPri.vertex[2].texCrood[i]);

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

	void Rasterizer::clear()
	{
		if (mZBuffer)
		{
			float value = 1.f;
			unsigned int num = mZBuffer->getWidth() * mZBuffer->getHeight();
			mZBuffer->seek(0);
			for (unsigned int i = 0; i < num; ++i)
			{
				mZBuffer->write(&value,sizeof(float));
			}
		}
	}

	void Rasterizer::flush(RenderTarget* target)
	{


		clear();
		mColorBuffer = target;

		PrimitiveVector::iterator i,endi = mPrimitiveVector.end();
		for (i = mPrimitiveVector.begin(); i != endi; ++i)
		{
			switch(i->type)
			{
			case Primitive::POINT:
				drawPoint(*i);
				break;
			case Primitive::LINE:
				drawLine(*i);
				break;
			case Primitive::TRIANGLE:
				drawTriangle(*i);
				break;
			default:
				assert(0);
			}

			//copyToTarget(target);
			//clear();
		}
		mColorBuffer = NULL;
		mPrimitiveVector.clear();

	}

	void Rasterizer::drawPoint(const Primitive& pri)
	{

	}

	void Rasterizer::drawLine(const Primitive& pri)
	{

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

		Vertex point1, point2;
		Pixel point3;
		float ratio1,ratio2,ratio3;
		for (unsigned int y = ymin; y < ymax; ++y )
		{
			point3.y = y;

			ratio1 = (y - pri.vertex[0].pos.y) / (pri.vertex[2 - offset].pos.y -  pri.vertex[0].pos.y);
			ratio2 = (y - pri.vertex[1 - offset].pos.y) / (pri.vertex[2].pos.y -  pri.vertex[1 - offset].pos.y);

			interpolate(point1.pos,ratio1,pri.vertex[0].pos,pri.vertex[2 - offset].pos);
			interpolate(point2.pos,ratio2,pri.vertex[1 - offset].pos,pri.vertex[2].pos);
			interpolate(point1.color,ratio1,pri.vertex[0].color,pri.vertex[2 - offset].color);
			interpolate(point2.color,ratio2,pri.vertex[1 - offset].color,pri.vertex[2].color);
			interpolate(point1.norm,ratio1,pri.vertex[0].norm,pri.vertex[2 - offset].norm);
			interpolate(point2.norm,ratio2,pri.vertex[1 - offset].norm,pri.vertex[2].norm);
			interpolate(point1.specular,ratio1,pri.vertex[0].specular,pri.vertex[2 - offset].specular);
			interpolate(point2.specular,ratio2,pri.vertex[1 - offset].specular,pri.vertex[2].specular);

			for (unsigned int i = 0; i < 8; ++i)
				if (pri.tex[i] != NULL)
				{
					interpolate(point1.texCrood[i],ratio1,pri.vertex[0].texCrood[i],pri.vertex[2 - offset].texCrood[i]);
					interpolate(point2.texCrood[i],ratio2,pri.vertex[1 - offset].texCrood[i],pri.vertex[2].texCrood[i]);
				}


			xmin = ceil(point1.pos.x);
			xmax = ceil(point2.pos.x);
			for (unsigned int x = xmin; x < xmax; ++x)
			{
				point3.x = x;

				ratio3 = (x - point1.pos.x) / (point2.pos.x -  point1.pos.x);
				//注意这里是 1/w
				interpolate(point3.invw, ratio3,point1.pos.w,point2.pos.w);	

				//颜色
				interpolate(point3.color, ratio3,point1.color,point2.color);
				point3.color /= point3.invw;

				interpolate(point3.specular, ratio3,point1.specular,point2.specular);
				point3.specular /= point3.invw;
				
				//需要z 
				interpolate(point3.z, ratio3,point1.pos.z,point2.pos.z);	
				
				
				//临时处理
				Colour colorBlend;
				for (unsigned int i = 0; i < 8; ++i)
				{
					if (pri.tex[i] != NULL)
					{
						//这里的u v实际是 U/w V/w;
						interpolate(point3.u,ratio3,point1.texCrood[i].x,point2.texCrood[i].x);
						interpolate(point3.v,ratio3,point1.texCrood[i].y,point2.texCrood[i].y);
						//先這裡就不混合了，到時候還要給混合公式
						colorBlend = addressTex(pri.tex[i],point3.u / point3.invw, point3.v/point3.invw);
					}
				}
				//這裡也是
				if (pri.tex[0] != NULL)
				point3.color *= colorBlend;
				drawImpl(point3);
			}
		}
		
	}

	size_t Rasterizer::getBufferPos(unsigned int x, unsigned int y, unsigned int width, unsigned int colorDepth)
	{
		return (x + y * width) * colorDepth;
	}

	void Rasterizer::drawImpl(const Pixel& p)
	{
		if (!pixelTest(p))
			return;

		unsigned int color;
		if (mPixelShader != NULL)
			color = mPixelShader->shade(p).get32BitARGB();
		else
			color = (p.color + p.specular).clamp().get32BitARGB();

		size_t pos = getBufferPos(p.x,p.y, mColorBuffer->getWidth(), mColorBuffer->getColourDepth());
		mColorBuffer->seek(pos);
		mColorBuffer->write(&color,sizeof(color));
		//zbuffer
		pos = getBufferPos(p.x,p.y, mColorBuffer->getWidth(), mZBuffer->getColourDepth());
		mZBuffer->seek(pos);
		//其实已经确定好是float.....还要查询有点多余
		mZBuffer->write(&p.z,mZBuffer->getColourDepth());
	}

	bool Rasterizer::depthTest(const Pixel& p)
	{
		float z = 0;
		size_t pos = getBufferPos(p.x,p.y, mColorBuffer->getWidth(), mZBuffer->getColourDepth());
		mZBuffer->seek(pos);
		mZBuffer->read(&z,sizeof (z));
		return p.z < z;

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
		return mAlphaRef <= p.color.a;
	}

	bool Rasterizer::pixelTest(const Pixel& p)
	{
		if (!scissorTest(p))
			return false;
		if (!alphaTest(p))
			return false;
		if (!depthTest(p))
			return false;
		return true;
	}

	template<class T>
	void Rasterizer::interpolate(T& output,float input0, float input1, float inputx, const T& value0, const T& value1)
	{
		interpolate(output,(inputx - input0) / (input1 - input0),value0,value1);
	}

	template<class T>
	void Rasterizer::interpolate(T& output,float ratio, const T& value0, const T& value1)
	{
		output = (value1 - value0) * ratio  + value0;
	}

	Colour Rasterizer::addressTex(const Texture* tex,float u,float v)
	{
		assert(tex);
		assert( u < 1.0f && v < 1.0f);

		//先不管mipmap
		RenderTarget* rt = tex->getRenderTarget(0);
		size_t pos = getBufferPos( (unsigned int )((rt->getWidth()-1) * u),
			(unsigned int )((rt->getHeight()-1) * v), rt->getWidth(),rt->getColourDepth());
		int c;
		rt->seek(pos);
		rt->read(&c,sizeof(int));
		Colour color;
		switch (tex->getPixelFormat())
		{
		case PF_A8R8G8B8 :
		case PF_X8R8G8B8 :
			color.getFromARGB(c);
			break;
		case PF_A8B8G8R8 :
		case PF_X8B8G8R8 :
			color.getFromABGR(c);
			break;
		case PF_B8G8R8A8 :
			color.getFromBGRA(c);
			break;
		case PF_R8G8B8A8 :
			color.getFromRGBA(c);
			break;
		default:
			assert(0);
		}

		return color;
	}



}