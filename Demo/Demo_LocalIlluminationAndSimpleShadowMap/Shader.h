#pragma once;

#include "..\..\RendererComplement\include\PipelinePlus.h"

using namespace RCP;
class BaseVS:public VertexShader
{
protected:
	Matrix4X4 matrix[TS_BASALNUM];
public:
	void setMatrix(TransformStateType type,const Matrix4X4& mat)
	{
		matrix[type] = mat;
	}

};

class SMMakerVS:public BaseVS
{
public:
	void execute(Vertex& vert)
	{
		vert.pos = matrix[TS_PROJECTION] * (matrix[TS_VIEW] * (matrix[TS_WORLD]*vert.pos));
		vert.color[0].r = vert.pos.z / vert.pos.w;

	}
};

class SMMakerPS:public PixelShader
{
public:
	Colour shade(const Pixel& p)
	{
		float z = p.color[0].r;
		return Colour().getFromARGB(*((int*)(&z)));
	}

};

class VS:public BaseVS
{
public:
	void execute(Vertex& vert)
	{
		Vector4 temp;
		//记录法线
		vert.color[0] = convert(matrix[TS_WORLD] * Vector4(vert.norm,0.0f));
		//记录该点在灯空间下的投影 TS_WORLD2代替lightView
		vert.color[1] = convert(matrix[TS_PROJECTION] * (matrix[TS_WORLD2] * vert.pos));
		//记录该点实际坐标
		vert.color[2] = convert(matrix[TS_WORLD] * vert.pos);

		vert.pos = matrix[TS_PROJECTION] * (matrix[TS_VIEW] *(matrix[TS_WORLD] * vert.pos));
	}
};


class PS:public PixelShader
{
public:
	Colour shade(const Pixel& p)
	{
		Vector4 temp(convert(p.color[2]));
		Vector3 realPos(temp.x,temp.y,temp.z);

		Vector3 L=lightPos - realPos;
		L.normalise();

		temp = convert(p.color[0]);
		Vector3 norm(temp.x,temp.y,temp.z);
		
		Vector3 V = cameraPos - realPos;
		V.normalise();

		Vector3 H = L+V;
		H.normalise();

		//计算高光
		Colour specular = pow(std::max<float>(0,H.dotProduct(norm)),power);
		Colour diffuse = std::max<float>(0,norm.dotProduct(L));
		specular.a = 1.0f;
		

		Vector4 posInL = convert(p.color[1]);
		if (posInL.w > 0 && fabs(posInL.x) < posInL.w && fabs(posInL.y) < posInL.w && fabs(posInL.z) <posInL.w && L.dotProduct(norm) > 0)
		{
			Vector2 stc;
			float invw = 1.0f / posInL.w;
			posInL *= invw;

			stc.x = 0.5 * posInL.x  + 0.5;
			stc.y = 0.5 * posInL.y  + 0.5;
			stc.y = 1.0f - stc.y;


			TextureState ts;
			ts.addresingModeU = TAM_CLAMP;
			ts.addresingModeV = TAM_CLAMP;
			sampler[0].setTextureState(ts);
			int c = tex2D(0,stc.x,stc.y).get32BitARGB();
			float depth[5];
			depth[0] = *(float*)(&c)  ;
			//float depth = sampler[0].sample(stc).r;
			float shadow=0.0f;
			float pixelSizeW = 1.0f/width;
			float pixelSizeH = 1.0f/height;

			c = tex2D(0,stc.x + pixelSizeW,stc.y).get32BitARGB();
			depth[1] = *(float*)(&c) ;
			c = tex2D(0,stc.x - pixelSizeW,stc.y).get32BitARGB();
			depth[2] = *(float*)(&c)  ;
			c = tex2D(0,stc.x,stc.y + pixelSizeH).get32BitARGB();
			depth[3] = *(float*)(&c)  ;
			c = tex2D(0,stc.x,stc.y - pixelSizeH).get32BitARGB();
			depth[4] = *(float*)(&c) ;

#define SHADOW_EPSILON 0.0005f

			float result = 0;
			for (int i = 0; i < 5; ++i)
			{
				
				depth[i] += SHADOW_EPSILON;
				shadow += (depth[i] < posInL.z)?0.0f:1.0f;
			}
			result *= 0.2f;
			//shadow = ( result  < posInL.z)? 0.0f:1.0f;
			shadow *= 0.2f;
			
			diffuse = diffuse * shadow;
			diffuse.saturate();
			
			//return shadow;
			return  (diffuse + specular).saturate();
			//return (diffuse * shadow).clamp();
		}
		else
		{
			return (diffuse + specular).saturate() ;
		}

		
	}

	Vector3 lightPos;
	Vector3 cameraPos;
	float power;
	float width,height;
};