#include "Viewport.h"

namespace RCP
{
	Viewport::Viewport():
		x(0),y(0),width(0),height(0),zMax(0),zMin(0)
	{
	
	}

	Viewport::Viewport(unsigned int X, unsigned int Y, unsigned int W, unsigned int H, float ZMin, float ZMax):
		x(X),y(Y),width(W),height(H),zMin(ZMin), zMax(ZMax)
	{}

	Viewport::~Viewport()
	{}

	void Viewport::mapping(Vector4& pos)const
	{
		//按理说 经过clip以后应该不会超出屏幕，不过这里还是做一下判断
		//反过来说就是如果这里出错，说明clip算法有问题
		//当前检查没啥问题。。
		//assert(pos.x >= -1 && pos.x <= 1);
		//assert(pos.y >= -1 && pos.y <= 1);
		//assert(pos.z >= -1 && pos.z <= 1);
		//assert(pos.w <= 1 && pos.w >= -1);
		
		float hw = width * 0.5f;
		float hh = height * 0.5f;
		float hz = (zMax - zMin) * 0.5f;
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
		pos = pos * Vector4(hw,-hh,hz,1) + Vector4(hw + x,hh + y,hz + zMin,0);
	}
}