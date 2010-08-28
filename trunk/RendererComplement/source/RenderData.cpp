#include "RenderData.h"
#include "RenderQueue.h"
namespace RCP
{
	RenderData::RenderData(Primitives type, unsigned int offset, unsigned int count, const RenderParameter& paras):
		ptType(type),beginPrimitiveOffset(offset),primitiveCount(count),renderParameter(paras)
	{
	}

}