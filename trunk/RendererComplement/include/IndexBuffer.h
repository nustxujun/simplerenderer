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
		void addIndex(unsigned int vertexIndex,unsigned int pos = -1);
		void addIndex(unsigned int count,unsigned int numBytesPerIndex,const void* indexData);
		const size_t getIndexCount()const;
		unsigned int operator [](unsigned int index)const;
	private :
		IndexBuffer(IndexBufferManager* mgr);
		void initImpl() ;
	private:
		typedef std::vector<unsigned int> IndexList;
		IndexList mIndexList;
	};
}
#endif//_IndexBuffer_H_
