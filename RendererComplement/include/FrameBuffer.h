#ifndef _FrameBuffer_H_
#define _FrameBuffer_H_

#include "Prerequisites.h"
#include "RenderTarget.h"

namespace RCP
{

	class FrameBuffer
	{
	public :
		FrameBuffer(unsigned int w,unsigned int h);
		~FrameBuffer();

		void setBuffer(BufferTpye type,RenderTarget* rt);

		RenderTarget* operator [](unsigned int index);

		template<class T>
		inline void clear(BufferTpye type, const T& t)
		{
			assert(type < BT_COUNT);
			RenderTarget* rt = mBuffers[type];

			if (rt == NULL)
				return;
			assert(rt->getColourDepth() == sizeof (t));
			size_t size = rt->getWidth() * rt->getHeight();
			rt->seek(0);
			for (size_t i = 0; i < size; ++i)
			{
				rt->write(&t,sizeof(T));
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
			assert(colorDepth == sizeof (value));
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
				memset(&t,0,sizeof(T));
				return;
			}
			unsigned colorDepth = rt->getColourDepth();
			assert(colorDepth == sizeof (T));
			size_t pos = getBufferPos(x,y,rt->getWidth(),colorDepth);
			rt->seek(pos);
			rt->read(&t,sizeof (T));
		}
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
