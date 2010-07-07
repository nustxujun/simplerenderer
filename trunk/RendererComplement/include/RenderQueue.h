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

		//准备即将参加计算的数据并发送出去，
		//如果多线程计算的话，这部分数据会与之后添加的数据分离开
		//当这部分数据绘制完后再进行下一批的绘制
		RenderData& postRenderData();
		//插入新的RenderElement到RenderData
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
