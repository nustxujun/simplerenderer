#include "VertexBufferManager.h"
#include "VertexBuffer.h"

namespace RCP
{
	VertexBuffer* VertexBufferManager::createVertexBuffer(unsigned int vertexCount, const VertexDeclaration& decl)
	{
		VertexBuffer* vb = new VertexBuffer(decl,vertexCount,this);
		vb->initialize();
		vb->addRef();
		add((Resource*)vb);
		return vb;
	}
}