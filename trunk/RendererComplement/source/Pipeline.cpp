#include "Pipeline.h"

namespace RCP
{
	Pipeline::ResultListener::~ResultListener()
	{}

	Pipeline::Pipeline():
		mRenderData(NULL)
	{

	}

	Pipeline::~Pipeline()
	{

	}

	void Pipeline::initialize(const RendererParameters& rp)
	{
		mRendererParameters = rp;
		initImpl();
	}


	void Pipeline::import(RenderData& data,FrameBuffer* fb, const RenderState& rs)
	{
		//���������Ⱦ�С��Ͳ������µĲ���
		if (isWorking())
			return;
		mRenderData = &data;
		execute(data,fb,rs);
	}


	void Pipeline::copyState(const Pipeline& pipeline)
	{

	}

	bool Pipeline::isWorking()
	{
		return mRenderData != NULL;
	}

	const RendererParameters& Pipeline::getRendererParameters()const 
	{
		return mRendererParameters;
	}

	void Pipeline::notifyCompleted()
	{
		mRenderData->junk();
		mRenderData = NULL;
	}
}