#include "FrameBuffer.h"
#include "RenderTarget.h"

namespace RCP
{
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
		assert(rt->getWidth() == mWidth);
		assert(rt->getHeight() == mHeight);
		mBuffers[type] = rt;
	}

	RenderTarget* FrameBuffer::operator [](unsigned int index)
	{
		assert(index < BT_COUNT);
		return mBuffers[index];
	}






}