#pragma once;
#include "..\..\RendererComplement\include\PipelinePlus.h"
using namespace RCP;


class VS:public VertexShader
{
	void execute(Vertex& vert)
	{
		vert.color[2] = convert(matrix4X4[0] * vert.pos);
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

		vert.color[1] = convert(matrix4X4[0] * Vector4(vert.norm,0));
	}
};

class PS:public PixelShader
{
	Colour shade(const Pixel& p)
	{
		Vector4 realPos(convert(p.color[2]));
		Vector4 temp = vector4[1] - realPos;

		Vector3 L(temp.x,temp.y,temp.z);
		L.normalise();

		temp = vector4[0] - realPos;
		Vector3 V(temp.x,temp.y,temp.z);
		V.normalise();

		Vector3 H = L + V;
		H.normalise();

		temp = convert(p.color[1]);
		Vector3 norm(temp.x,temp.y,temp.z);

		Colour specular = pow(std::max<float>(0,H.dotProduct(norm) * 1.1f ),30)  ;

		return ((texCube(0,p.color[0].r,p.color[0].g,p.color[0].b) + specular) * Colour(0.5f,0.375f,0.25f,1.0f) * 0.8f) .saturate();
	}
};