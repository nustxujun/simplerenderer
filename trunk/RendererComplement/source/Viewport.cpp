#include "Viewport.h"

namespace RCP
{
	Viewport::Viewport():
		x(0),y(0),width(0),height(0)
	{
	
	}

	Viewport::Viewport(unsigned int X, unsigned int Y, unsigned int W, unsigned int H):
			x(X),y(Y),width(W),height(H)
	{}

	Viewport::~Viewport()
	{}

	void Viewport::mapping(Vector4& pos)const
	{
		//按理说 经过clip以后应该不会超出屏幕，不过这里还是做一下判断
		//反过来说就是如果这里出错，说明clip算法有问题
		assert(pos.x >= -1 && pos.x <= 1);
		assert(pos.y >= -1 && pos.y <= 1);
		assert(pos.z >= -1 && pos.z <= 1);
		assert(pos.w < 1.0001 && pos.w > 0.9999);
		
		float hw = width * 0.5f;
		float hh = height * 0.5f;
		//因为屏幕坐标是
		/* 0------------------>
		   |	        x
		   |
		   |
		 y |
		   |
		   |
		   V
			
		这样子。。。。而在空间里 y 正轴是朝上，所以这里是 -hh
		*/
		pos = pos * Vector4(hw,-hh,1,1) + Vector4(hw + x,hh + y,0,0);
	}
}