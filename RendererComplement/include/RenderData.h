#ifndef _RenderData_H_
#define _RenderData_H_
#include "Prerequisites.h"
#include "Material.h"
#include "Light.h"
#include "Viewport.h"
#include "Sampler.h"

namespace RCP
{
	struct RenderParameter
	{
		//当前vb
		VertexBuffer* vertexBuffer;
		//当前ib
		IndexBuffer* indexBuffer;
		//当前纹理,限定8层（不是有什么不可实现，只是模拟显卡最大数）
		Texture* texture[8];
		//当前材质
		Material material;
		//当前矩阵
		Matrix4X4 matrices[TS_BASALNUM];
		//灯光
		Light light[8];
		//视口
		Viewport viewport;
		//纹理状态
		Sampler sampler[8];
		//渲染状态
		RenderState renderState;
		//frameBuffer
		FrameBuffer* frameBuffer;
		//assistantBuffer,暂时是z + stencil
		RenderTarget* assistantBuffer[2];
		//其他属性状态
		typedef std::map<std::string , Any> Propertys;
		Propertys propertys;
	};

	class RenderData
	{
	public :
		RenderData(Primitives type, unsigned int offset, unsigned int count, const RenderParameter& paras);


		Primitives ptType;
		unsigned int beginPrimitiveOffset;
		unsigned int primitiveCount;
		const RenderParameter& renderParameter;

		



	};
}
#endif//_RenderData_H_
