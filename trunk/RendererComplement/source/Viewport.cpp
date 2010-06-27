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
		//����˵ ����clip�Ժ�Ӧ�ò��ᳬ����Ļ���������ﻹ����һ���ж�
		//������˵��������������˵��clip�㷨������
		//��ǰ���ûɶ���⡣��
		//assert(pos.x >= -1 && pos.x <= 1);
		//assert(pos.y >= -1 && pos.y <= 1);
		//assert(pos.z >= -1 && pos.z <= 1);
		//assert(pos.w <= 1 && pos.w >= -1);
		
		float hw = width * 0.5f;
		float hh = height * 0.5f;
		float hz = (zMax - zMin) * 0.5f;
		//��Ϊ��Ļ������
		/* 0------------------>
		   |	        x
		   |
		   |
		 y |
		   |
		   |
		   V
			
		�����ӡ����������ڿռ��� y �����ǳ��ϣ����������� -hh
		*/
		pos = pos * Vector4(hw,-hh,hz,1) + Vector4(hw + x,hh + y,hz + zMin,0);
	}
}