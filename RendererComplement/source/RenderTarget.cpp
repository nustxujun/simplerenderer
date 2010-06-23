#include "RenderTarget.h"

namespace RCP
{
	RenderTarget::RenderTarget(unsigned int width, unsigned int height, unsigned int colourDepth):
		mWidth(width),mHeight(height),mColourDepth(colourDepth),MemoryBuffer(NULL,0,true)
	{
		mSize = width * height * colourDepth;
		mPos = mData = new unsigned char[mSize];
		mEnd = mData + mSize;
		memset(mData,0xff,mSize);		
		setAutoFree(true);
	}

	RenderTarget::~RenderTarget()
	{

	}


	unsigned int RenderTarget::getWidth()
	{
		return mWidth;
	}

	unsigned int RenderTarget::getHeight()
	{
		return mHeight;
	}

	unsigned int RenderTarget::getColourDepth()
	{
		return mColourDepth;
	}


}