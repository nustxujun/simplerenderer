#include "Pipeline.h"

namespace RCP
{
	Pipeline::ResultListener::~ResultListener()
	{}

	Pipeline::Pipeline()
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


	void Pipeline::import(RenderData& data)
	{

		execute(data);
	}


	void Pipeline::copyState(const Pipeline& pipeline)
	{

	}

	const RendererParameters& Pipeline::getRendererParameters()const 
	{
		return mRendererParameters;
	}

}