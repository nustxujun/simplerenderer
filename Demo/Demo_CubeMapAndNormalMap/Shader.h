#pragma once;
#include "..\..\RendererComplement\include\PipelinePlus.h"
using namespace RCP;


class VS:public VertexShader
{
	void execute(Vertex& vert)
	{
		vert.pos = matrix4X4[1] * (matrix4X4[0] * vert.pos);
		Vector3 norm = matrix4X4[1] * (matrix4X4[0] * vert.norm);
		norm.normalise();
		Vector3 eyeR(-vert.pos.x,-vert.pos.y,-vert.pos.z);
		eyeR.normalise();

		Vector3 vRef = norm * 2 * eyeR.dotProduct(norm)  - eyeR;
		vert.color[0] = convert(vRef);
		vert.pos = matrix4X4[2] * vert.pos;
	}
};

class PS:public PixelShader
{
	Colour shade(const Pixel& p)
	{
		return texCube(0,p.color[0].r,p.color[0].g,p.color[0].b);
	}
};