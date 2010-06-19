#include "RenderData.h"
#include "RenderQueue.h"
namespace RCP
{

	RenderData::RenderData(RenderQueue* rq):
		mRenderQueue(rq)
	{}

	RenderData::~RenderData()
	{}

	void RenderData::insertRenderElement(unsigned int offset ,unsigned int c, Primitives type, VertexBuffer* vb, const Matrix4X4 world[TS_BASALNUM], Texture* tex[8], IndexBuffer* ib,const Material& mat,const Light light[8])
	{
		mRenderElementList.push_back(RenderElement(offset,c,type,vb,world,tex,ib,mat,light));
	}

	void RenderData::junk()
	{
		//注意这里是进行了delete this这样的操作
		mRenderQueue->destroyRenderData(this);
	}


	const RenderData::RenderElementList& RenderData::getRenderElementList() const
	{
		return mRenderElementList;
	}
}