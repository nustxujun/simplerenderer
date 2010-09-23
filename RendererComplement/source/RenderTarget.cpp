#include "RenderTarget.h"

namespace RCP
{
	RenderTarget::RenderTarget(unsigned int width, unsigned int height, unsigned int colourDepth):
		mWidth(width),mHeight(height),mColourDepth(colourDepth),MemoryBuffer(NULL,0,true)
	{
		mSize = width * height * colourDepth;
		mPos = mData = new unsigned char[mSize];
		mEnd = mData + mSize;
		memset(mData,0,mSize);		
		setAutoFree(true);
	}

	RenderTarget::~RenderTarget()
	{

	}


	unsigned int RenderTarget::getWidth()const 
	{
		return mWidth;
	}

	unsigned int RenderTarget::getHeight()const
	{
		return mHeight;
	}

	unsigned int RenderTarget::getColourDepth()const
	{
		return mColourDepth;
	}

	void RenderTarget::swap(RenderTarget& rt)
	{
		MemoryBuffer::swap(rt);
		std::swap(mWidth,rt.mWidth);
		std::swap(mHeight,rt.mHeight);
		std::swap(mColourDepth,rt.mColourDepth);
	}


}