#include "SwapChain.h"
#include "RenderTarget.h"


namespace RCP
{
	SwapChain::SwapChain()
	{
	}

	SwapChain::~SwapChain()
	{
		RenderTargetList::iterator iter;
		for (iter = mIterBegin; iter != mIterEnd; ++iter)
			delete (*iter);
	}

	RenderTarget* SwapChain::getFrontBuffer()
	{
		return *mIterBegin;
	}

	RenderTarget* SwapChain::getBackBuffer()const
	{
		return *mSecondBufferIter;
	}

	void SwapChain::swap()
	{
		if (mSecondBufferIter == mIterBegin)
			return;

		RenderTargetList::iterator first,second;
		for(first = mIterBegin, second = mSecondBufferIter; second != mIterEnd; ++first,++second)
		{
			(*first)->swap(**second);
		}

	}

	void SwapChain::initialize(int width,int height,PixelFormat pf,unsigned int count)
	{
		unsigned int colourDepth = PixelUtil::getNumElemBytes(pf);

		if (count == 0)
			count = 2;
		mRenderTargetList.resize(count);

		for (unsigned int i = 0; i < count; ++i)
			mRenderTargetList[i] = new RenderTarget(width, height, colourDepth);		
		mIterBegin = mRenderTargetList.begin();
		mIterEnd = mRenderTargetList.end();
		mSecondBufferIter = mIterBegin + 1;
		if (mSecondBufferIter == mIterEnd)
			mSecondBufferIter = mIterBegin;

	}
}