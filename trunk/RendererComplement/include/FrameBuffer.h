#ifndef _FrameBuffer_H_
#define _FrameBuffer_H_

#include "Prerequisites.h"
#include "RenderTarget.h"

namespace RCP
{

	class FrameBuffer
	{
	public :
		//注意这里如果使用默认构造，将会出现无法setBuffer的情况（长宽不对）
		//所以正常创建请传长宽值
		//默认构造只有在只进行拷贝的时候使用
		FrameBuffer();
		FrameBuffer(unsigned int w,unsigned int h);
		FrameBuffer(const FrameBuffer& fb);
		~FrameBuffer();

		void setBuffer(BufferTpye type,RenderTarget* rt);
		RenderTarget* getBuffer(BufferTpye type);


		/*
		为了配合Renderer的统一渲染(渲染状态都保存到当renderNow再开始draw)
		clear操作也作为状态延时到实际写入之前进行。
		因此需要注意的当外部调用setValue getvalue的时候
		如果光栅化和renderer的drawPrimitive是异步的，可能会使framebuffer提前clear;
		因此禁止外部直接调用setValue getvalue，只能在光栅化的时候使用。
		同样禁止向外部提供frameBuffer;
		*/

		template<class T>
		void clear(BufferTpye type, const T& value)
		{
			assert(type < BT_COUNT);
			RenderTarget* rt = mBuffers[type];

			if (rt == NULL)
			{
				return ;
			}

			unsigned int valueSize = sizeof(value);
			if (rt->getColourDepth() != valueSize)
				THROW_EXCEPTION("value of clearing does not match colourDepth.");


			size_t size = rt->getWidth() * rt->getHeight();
			rt->seek(0);

			for (size_t i = 0; i < size; ++i)
			{
				rt->write(&value,valueSize);
			}

		}


		template<class T>
		inline void setValue(BufferTpye type,unsigned int x, unsigned int y,const T& value)
		{
			assert(type < BT_COUNT);
			RenderTarget* rt = mBuffers[type];

			if (rt == NULL)
				return;

			unsigned colorDepth = rt->getColourDepth();
			if (colorDepth != sizeof (value))
				THROW_EXCEPTION("value does not match colourDepth.");
			size_t pos = getBufferPos(x,y,rt->getWidth(),colorDepth);
			rt->seek(pos);
			rt->write(&value,sizeof (value));
		}

		template<class T>
		inline void getValue(T& t ,BufferTpye type,unsigned int x, unsigned int y)
		{
			assert(type < BT_COUNT);
			RenderTarget* rt = mBuffers[type];

			if (rt == NULL)
			{
				//不需要置空这样更能发现问题
				//memset(&t,0,sizeof(T));
				return;
			}

			unsigned colorDepth = rt->getColourDepth();
			if (colorDepth != sizeof (t))
				THROW_EXCEPTION("container does not match colourDepth.");
			size_t pos = getBufferPos(x,y,rt->getWidth(),colorDepth);
			rt->seek(pos);
			rt->read(&t,sizeof (T));
		}

		void operator = (const FrameBuffer& fb);
	private:


		inline size_t getBufferPos(unsigned int x, unsigned int y, unsigned int width, unsigned int colorDepth)
		{
			return (x + y * width) * colorDepth;
		}


	private:
		RenderTarget* mBuffers[BT_COUNT];

		unsigned int mWidth;
		unsigned int mHeight;

	};
}
#endif//_FrameBuffer_H_
