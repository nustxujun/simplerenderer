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


	void Pipeline::import(RenderData& data,RenderTarget* target)
	{
		//如果还在渲染中。就不进行新的操作
		if (isWorking())
			return;
		mRenderData = &data;
		execute(data,target);
	}


	void Pipeline::copyState(const Pipeline& pipeline)
	{

	}

	bool Pipeline::isWorking()
	{
		return mRenderData != NULL;
	}

	const RendererParameters& Pipeline::getRendererParameters()
	{
		return mRendererParameters;
	}

	void Pipeline::notifyCompleted()
	{
		mRenderData->junk();
		mRenderData = NULL;
	}
}