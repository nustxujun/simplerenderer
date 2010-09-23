#ifndef _SwapChain_H_
#define _SwapChain_H_

#include "Prerequisites.h"

namespace RCP
{
	class SwapChain
	{
	public:
		SwapChain();
		virtual ~SwapChain();
		void initialize(int width,int height,PixelFormat pf,unsigned int count = 2);

		RenderTarget* getBackBuffer()const;
		//��buffer������2ʱ���ú��������swap
		RenderTarget* getFrontBuffer();

		void swap();
	private:
		typedef std::vector<RenderTarget*> RenderTargetList;
		RenderTargetList mRenderTargetList;
		RenderTargetList::iterator mIterBegin;
		RenderTargetList::iterator mSecondBufferIter;
		RenderTargetList::iterator mIterEnd;

	};
}
#endif//_SwapChain_H_
