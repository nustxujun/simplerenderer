#include "BackBuffer.h"
#include "MemoryBuffer.h"
#include "RenderTarget.h"
namespace RCP
{
	
	BackBuffer::BackBuffer():
		mRenderTarget(NULL)
	{
	
	}
	BackBuffer::~BackBuffer()
	{
		release();
	}

	RenderTarget* BackBuffer::getRenderTarget()const
	{
		return mRenderTarget;
	}

	PixelFormat BackBuffer::getPixelFormat()const
	{
		return mPixelFormat;
	}

	void BackBuffer::release()
	{
		SAFE_DELETE(mRenderTarget);
	}

	void BackBuffer::initialize(int width,int height,PixelFormat pf)
	{
		release();
		mPixelFormat = pf;
		unsigned int colourDepth = PixelUtil::getNumElemBytes(pf);
		mRenderTarget = new RenderTarget(width, height, colourDepth);		
	}

	
}