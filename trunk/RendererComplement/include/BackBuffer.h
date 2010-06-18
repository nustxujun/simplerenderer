#ifndef _BackBuffer_H_
#define _BackBuffer_H_

#include "Prerequisites.h"

namespace RCP
{
	class BackBuffer
	{
	public:
		BackBuffer();
		~BackBuffer();
		void initialize(int width,int height,PixelFormat pf);
		void release();
		RenderTarget* getRenderTarget()const;
		PixelFormat getPixelFormat()const;
	private:
		RenderTarget* mRenderTarget;
		PixelFormat mPixelFormat;

	};
}
#endif//_BackBuffer_H_
