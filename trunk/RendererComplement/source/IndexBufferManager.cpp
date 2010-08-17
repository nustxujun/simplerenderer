#include "IndexBufferManager.h"
#include "IndexBuffer.h"

namespace RCP
{
	IndexBuffer* IndexBufferManager::createIndexBuffer()
	{
		IndexBuffer* ib = new IndexBuffer(this);
		ib->initialize();
		ib->addRef();
		add((Resource*)ib);
		return ib;
	}
}