#include "FrameBuffer.h"
#include "RenderTarget.h"

namespace RCP
{
	FrameBuffer::FrameBuffer()
	{}

	FrameBuffer::FrameBuffer(const FrameBuffer& fb)
	{
		memcpy(mBuffers,fb.mBuffers,sizeof(mBuffers));

		mWidth = fb.mWidth;
		mHeight = fb.mHeight;
	}

	FrameBuffer::FrameBuffer(unsigned int w,unsigned int h):
		mWidth(w),mHeight(h)
	{
		memset(mBuffers,0,sizeof(mBuffers));
	}

	FrameBuffer::~FrameBuffer()
	{}

	void FrameBuffer::setBuffer(BufferTpye type,RenderTarget* rt)
	{
		assert(type < BT_COUNT);
		if (rt->getWidth() > mWidth || rt->getHeight() > mHeight)
			THROW_EXCEPTION("buffer size is bigger than backbuffer.");
		mBuffers[type] = rt;
	}

	RenderTarget* FrameBuffer::getBuffer(BufferTpye type)
	{
		assert(type < BT_COUNT);
		return mBuffers[type];
	}


	void FrameBuffer::operator =(const FrameBuffer& fb)
	{
		memcpy(mBuffers,fb.mBuffers,sizeof(mBuffers));

		mWidth = fb.mWidth;
		mHeight = fb.mHeight;
		
	}







}