#include "IndexBufferManager.h"
#include "IndexBuffer.h"

namespace RCP
{
	IndexBuffer* IndexBufferManager::createIndexBuffer(unsigned int indexCount, IndexFormat indexFormat)
	{
		IndexBuffer* ib = new IndexBuffer(indexCount,indexFormat,this);
		ib->initialize();
		ib->addRef();
		add((Resource*)ib);
		return ib;
	}
}