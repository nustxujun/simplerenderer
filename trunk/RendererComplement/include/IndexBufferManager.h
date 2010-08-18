#ifndef _IndexBufferManager_H_
#define _IndexBufferManager_H_

#include"Prerequisites.h"
#include "ResourceManager.h"

namespace RCP
{
	class IndexBufferManager:
		public ResourceManager
	{
	public:
		IndexBuffer* createIndexBuffer(unsigned int indexCount, IndexFormat indexFormat);

	};
}
#endif//_IndexBufferManager_H_
