#ifndef _RenderQueue_H_
#define _RenderQueue_H_
#include "Prerequisites.h"
#include "RenderData.h"
#include "Material.h"
#include "Light.h"
#include "Viewport.h"
namespace RCP
{
	class RenderQueue
	{
		
	public:
		RenderQueue();
		~RenderQueue();

		//׼�������μӼ�������ݲ����ͳ�ȥ��
		//������̼߳���Ļ����ⲿ�����ݻ���֮����ӵ����ݷ��뿪
		//���ⲿ�����ݻ�������ٽ�����һ���Ļ���
		RenderData& postRenderData();
		//�����µ�RenderElement��RenderData
		void createRenderElement(unsigned int offset ,unsigned int c, Primitives type, VertexBuffer* vb,const Matrix4X4 world[TS_BASALNUM],
			const Sampler spl[8],const Light light[8], IndexBuffer* ib,const Material& mat, const Viewport& vp, const RenderState& rs,const FrameBuffer& fb,const std::map<std::string ,Any>& ps);
		void destroyRenderData(RenderData* renderData);
		bool isRenderDataReady();
	private:
	

		RenderData* mPreparingRenderData;
		typedef std::list<RenderData*> RenderDataList;
		RenderDataList mRenderDataList;
	};
}
#endif//_RenderQueue_H_
