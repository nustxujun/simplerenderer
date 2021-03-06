#ifndef _PaintingMethod_H_
#define _PaintingMethod_H_

namespace RCP
{
	class PaintingMethod
	{
	public:
		virtual ~PaintingMethod(){}

		virtual void paint(const RenderTarget* renderTarget) = 0;
	};
}
#endif//_PaintingMethod_H_
