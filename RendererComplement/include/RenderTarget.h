#ifndef _RenderTarget_H_
#define _RenderTarget_H_

#include "Prerequisites.h"
#include "MemoryBuffer.h"

namespace RCP
{
	class RenderTarget:
		public MemoryBuffer
	{
	public :
		RenderTarget(unsigned int width, unsigned int height, unsigned int colourDepth);
		virtual ~RenderTarget();
		
		unsigned int getWidth()const;
		unsigned int getHeight()const;
		unsigned int getColourDepth()const;

		void swap(RenderTarget& rt);
	private:
		unsigned int mWidth;
		unsigned int mHeight;
		unsigned int mColourDepth;
	};
}
#endif//_RenderTarget_H_
