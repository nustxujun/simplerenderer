#ifndef _Viewport_H_
#define _Viewport_H_

#include"Prerequisites.h"

namespace RCP
{
	class Viewport
	{
	public:
		Viewport();
		Viewport(unsigned int X, unsigned int Y, unsigned int W, unsigned int H, float ZMin, float ZMax);
		~Viewport();

		unsigned int x ;
		unsigned int y ;
		unsigned int width;
		unsigned int height;
		float zMax,zMin;

		void mapping(Vector4& pos)const ;
	};
}
#endif//_Viewport_H_
