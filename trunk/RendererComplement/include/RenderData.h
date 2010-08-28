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
		//��ǰvb
		VertexBuffer* vertexBuffer;
		//��ǰib
		IndexBuffer* indexBuffer;
		//��ǰ����,�޶�8�㣨������ʲô����ʵ�֣�ֻ��ģ���Կ��������
		Texture* texture[8];
		//��ǰ����
		Material material;
		//��ǰ����
		Matrix4X4 matrices[TS_BASALNUM];
		//�ƹ�
		Light light[8];
		//�ӿ�
		Viewport viewport;
		//����״̬
		Sampler sampler[8];
		//��Ⱦ״̬
		RenderState renderState;
		//frameBuffer
		FrameBuffer* frameBuffer;
		//assistantBuffer,��ʱ��z + stencil
		RenderTarget* assistantBuffer[2];
		//��������״̬
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
