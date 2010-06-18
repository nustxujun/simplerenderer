#ifndef _VertexBufferManager_H_
#define _VertexBufferManager_H_
#include"Prerequisites.h"
#include "ResourceManager.h"

namespace RCP
{
	class VertexBufferManager:
		public ResourceManager
	{
	public:
		VertexBuffer* createVertexBuffer(unsigned int vertexCount, const VertexDeclaration& decl);
	};
}
#endif//_VertexBufferManager_H_
