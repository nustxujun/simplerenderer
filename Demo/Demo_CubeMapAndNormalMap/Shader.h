#pragma once;
#include "..\..\RendererComplement\include\PipelinePlus.h"
using namespace RCP;


class VS:public VertexShader
{
	void execute(Vertex& vert)
	{
		vert.pos = matrix4X4[1] * (matrix4X4[0] * vert.pos);
		Vector4 norm = matrix4X4[1] * (matrix4X4[0] * Vector4(vert.norm,0));
		//norm.normalise();
		Vector3 eyeR(-vert.pos.x,-vert.pos.y,-vert.pos.z);
		eyeR.normalise();
		Vector3 vRef(norm.x,norm.y,norm.z);
		vRef = vRef * 2 * eyeR.dotProduct(vRef)  - eyeR;
		//vRef.normalise();
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