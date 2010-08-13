#include "FrameBuffer.h"
#include "RenderTarget.h"

namespace RCP
{
	FrameBuffer::FrameBuffer():
		mIsDirty(false)
	{}

	FrameBuffer::FrameBuffer(const FrameBuffer& fb)
	{
		memcpy(mBuffers,fb.mBuffers,sizeof(mBuffers));

		mWidth = fb.mWidth;
		mHeight = fb.mHeight;
		
		mIsDirty = fb.mIsDirty;

		for (int i = 0; i < BT_COUNT; ++i)
		{
			mClearValue[i] = fb.mClearValue[i];
		}	
	}

	FrameBuffer::FrameBuffer(unsigned int w,unsigned int h):
		mWidth(w),mHeight(h),mIsDirty(false)
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

	RenderTarget* FrameBuffer::getBuffer(BufferTpye type)
	{
		assert(type < BT_COUNT);
		return mBuffers[type];
	}

	void FrameBuffer::clearImpl()
	{
		for (int index = 0; index < BT_COUNT; ++index)
		{
			if (mClearValue[index].isEmpty())
				continue;
			

			RenderTarget* rt = mBuffers[index];

			if (rt == NULL)
			{
				mClearValue[index].setEmpty();
				return ;
			}
			
			assert(rt->getColourDepth() == mClearValue[index].getSize());

			size_t size = rt->getWidth() * rt->getHeight();
			rt->seek(0);

			for (size_t i = 0; i < size; ++i)
			{
				rt->write(mClearValue[index].getContentPtr(),mClearValue[index].getSize());
			}
			
			mClearValue[index].setEmpty();
		}

		mIsDirty = false;
	}

	void FrameBuffer::reset()
	{
		mIsDirty = true;
		for (int i = 0; i < BT_COUNT; ++i)
		{
			mClearValue[i].setEmpty();
		}
	}


	void FrameBuffer::operator =(const FrameBuffer& fb)
	{
		memcpy(mBuffers,fb.mBuffers,sizeof(mBuffers));

		mWidth = fb.mWidth;
		mHeight = fb.mHeight;
		
		mIsDirty = fb.mIsDirty;

		for (int i = 0; i < BT_COUNT; ++i)
		{
			mClearValue[i] = fb.mClearValue[i];
		}
	}







}