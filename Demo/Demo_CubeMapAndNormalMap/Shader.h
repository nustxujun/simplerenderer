#pragma once;
#include "..\..\RendererComplement\include\PipelinePlus.h"
using namespace RCP;


class VS:public VertexShader
{
	void execute(Vertex& vert)
	{
		//world space
		vert.color[2] = convert(matrix4X4[0] * vert.pos);
		vert.pos = matrix4X4[1] * (matrix4X4[0] * vert.pos);
		vert.texCrood[0] = vert.texCrood[0];

		Vector4 norm = matrix4X4[1] * matrix4X4[0] * Vector4(vert.norm,0);
		//norm.normalise();
		Vector3 eyeR(-vert.pos.x,-vert.pos.y,-vert.pos.z);
		eyeR.normalise();
		Vector3 vRef(norm.x,norm.y,norm.z);
		vRef = vRef * 2 * eyeR.dotProduct(vRef)  - eyeR;
		//vRef.normalise();
		//reflect
		vert.color[0] = convert(vRef);
		vert.pos = matrix4X4[2] * vert.pos;
		//worldspace normal
		vert.color[1] = convert(matrix4X4[0] * Vector4(vert.norm,0));


		//light dir
		Vector4 lightDir = vector4[1] - vert.pos;
		Vector4 cameraDir = vector4[0] - vert.pos;
		lightDir.w = 0;
		cameraDir.w = 0;
		//转换到模型空间
		lightDir = matrix4X4[3] * lightDir;
		cameraDir = matrix4X4[3] * cameraDir;

		Matrix4X4 tangentSpace;
		tangentSpace.m[0][0] = vert.tan.x;
		tangentSpace.m[0][1] = vert.tan.y;
		tangentSpace.m[0][2] = vert.tan.z;

		tangentSpace.m[1][0] = vert.bino.x;
		tangentSpace.m[1][1] = vert.bino.y;
		tangentSpace.m[1][2] = vert.bino.z;

		tangentSpace.m[2][0] = vert.norm.x;
		tangentSpace.m[2][1] = vert.norm.y;
		tangentSpace.m[2][2] = vert.norm.z;

		//tangentSpace
		lightDir = tangentSpace * lightDir;
		cameraDir = tangentSpace* cameraDir;

		//方向，在这里做完变换后直接和采出的normal进行运算
		Vector3 temp(lightDir.x,lightDir.y,lightDir.z);
		temp.normalise();
		//tangent space lightdir
		vert.color[3] = convert(temp);
		temp = Vector3(cameraDir.x,cameraDir.y,cameraDir.z);
		temp.normalise();
		//tangent space cameradir
		vert.color[4] = convert(temp);
	}
};

class PS:public PixelShader
{
	Colour shade(const Pixel& p)
	{
		//Vector4 realPos(convert(p.color[2]));
		//Vector4 temp = vector4[1] - realPos;

		Vector3 L(p.color[3].r,p.color[3].g,p.color[3].b);
		//L.normalise();

		//temp = vector4[0] - realPos;
		Vector3 V(p.color[4].r,p.color[4].g,p.color[4].b);
		//V.normalise();

		Vector3 H = L + V;
		H.normalise();

		Colour temp = tex2D(1,p.u[0] ,p.v[0] );
		temp = temp * 2.0f - 1.0f;
		Vector3 norm(temp.r,temp.g,temp.b);

		Colour specular = pow(std::max<float>(0,H.dotProduct(norm) * 1.1f ),10)  ;
		Colour diffuse = std::max<float>(0,norm.dotProduct(L));

		//return diffuse;
		return ((texCube(0,p.color[0].r,p.color[0].g,p.color[0].b)*diffuse + specular) * Colour(0.5f,0.375f,0.25f,1.0f)  ) .saturate();
	}
};


