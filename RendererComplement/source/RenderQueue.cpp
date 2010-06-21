#include "RenderQueue.h"

namespace RCP
{

	RenderQueue::RenderQueue():
		mPreparingRenderData(NULL)
	{
		mPreparingRenderData = new RenderData(this);
	}
	RenderQueue::~RenderQueue()
	{
		if (mPreparingRenderData)
			delete mPreparingRenderData;

		RenderDataList::iterator i, endi = mRenderDataList.end();
		for (i = mRenderDataList.begin(); i != endi ; ++i)
		{
			delete *i;
		}

	}

	bool RenderQueue::isRenderDataReady()
	{
		return !mPreparingRenderData->mRenderElementList.empty() ;
	}

	//����ǰȷ����RenderData����
	RenderData& RenderQueue::postRenderData()
	{
		//��postRenderData��ʱ�򣬵�ǰ��RenderState�ͱ�������������һ���͵���ˮ��
		//���������ʱ ������ݽṹ�ͻᱻֱ������
		RenderData* post = mPreparingRenderData;
		mRenderDataList.push_back(mPreparingRenderData);
		mPreparingRenderData = new RenderData(this);	
		return **mRenderDataList.begin();
	}

	void RenderQueue::destroyRenderData(RenderData* renderData)
	{
		if (mRenderDataList.empty())
			return;
		mRenderDataList.erase(std::remove(mRenderDataList.begin(),mRenderDataList.end(),renderData));
		delete renderData;
	}


	void RenderQueue::createRenderElement(unsigned int offset ,unsigned int c, Primitives type, VertexBuffer* vb,
			const Matrix4X4 world[TS_BASALNUM],Texture* tex[8],const Light light[8], IndexBuffer* ib  ,const Material& mat , const Viewport& vp)
	{
		mPreparingRenderData->insertRenderElement(offset,c,type,vb,world,tex,ib,mat,light,vp);
	}

}