#ifndef _IndexBuffer_H_
#define _IndexBuffer_H_
#include"Prerequisites.h"
#include "Resource.h"
namespace RCP
{
	class IndexBuffer:
		public Resource
	{
		friend class IndexBufferManager;
	public:
		~IndexBuffer();
		void fillIndex(unsigned int vertexIndex,unsigned int pos);
		void fillIndex(unsigned int count,unsigned int numBytesPerIndex,const void* indexData);
		const size_t getIndexCount()const;
		unsigned int operator [](unsigned int index)const;
		IndexFormat getIndexFormat()const;
		unsigned int getSizePerIndex()const;
		void* lock();
		void unlock();
	private :
		IndexBuffer(unsigned int indexCount, IndexFormat indexformat, IndexBufferManager* mgr);
		void initImpl() ;
	private:
		MemoryBuffer* mData;
		unsigned int mIndexCount;
		IndexFormat mIndexFormat;
	};
}
#endif//_IndexBuffer_H_
