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
			mClearValue[i].first = fb.mClearValue[i].first;
			if (fb.mClearValue[i].first == 0)
				continue;
			mClearValue[i].second = new unsigned char[mClearValue[i].first];
			memcpy(mClearValue[i].second, fb.mClearValue[i].second,mClearValue[i].first);
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

	RenderTarget* FrameBuffer::operator [](unsigned int index)
	{
		assert(index < BT_COUNT);
		return mBuffers[index];
	}

	void FrameBuffer::clearImpl()
	{
		for (int index = 0; index < BT_COUNT; ++index)
		{
			if (mClearValue[index].first == 0)
				continue;
			

			RenderTarget* rt = mBuffers[index];

			if (rt == NULL)
			{
				mClearValue[index].first = 0;
				SAFE_DELETE(mClearValue[index].second);
			}
			assert(rt->getColourDepth() == mClearValue[index].first);

			size_t size = rt->getWidth() * rt->getHeight();
			rt->seek(0);
			for (size_t i = 0; i < size; ++i)
			{
				rt->write(mClearValue[index].second,mClearValue[index].first);
			}
			
			mClearValue[index].first = 0;
			SAFE_DELETE(mClearValue[index].second);
		}

		mIsDirty = false;
	}

	void FrameBuffer::reset()
	{
		mIsDirty = true;
		for (int i = 0; i < BT_COUNT; ++i)
		{
			mClearValue[i].first = 0;
			SAFE_DELETE(mClearValue[i].second);
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
			if (mClearValue[i].first != 0)
			{
				mClearValue[i].first = 0;
				SAFE_DELETE(mClearValue[i].second);
			}
			mClearValue[i].first = fb.mClearValue[i].first;
			if (fb.mClearValue[i].first == 0)
				continue;
			mClearValue[i].second = new unsigned char[mClearValue[i].first];
			memcpy(mClearValue[i].second, fb.mClearValue[i].second,mClearValue[i].first);
		}
	}







}