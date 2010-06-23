#include "IndexBuffer.h"

namespace RCP
{
	IndexBuffer::~IndexBuffer()
	{

	}

	IndexBuffer::IndexBuffer(IndexBufferManager* mgr):
		Resource((ResourceManager*)mgr)
	{

	}


	void IndexBuffer::addIndex(unsigned int vertexIndex,unsigned int pos )
	{
		if (pos > mIndexList.size())
		{
			mIndexList.push_back(vertexIndex);
		}
		else
		{
			IndexList::iterator i,endi = mIndexList.end();
			unsigned int index = 0;
			for (i = mIndexList.begin(); i != endi; ++i,++index)
			{
				if (pos == index)
					mIndexList.insert(i,vertexIndex);
			}
		}
	}

	void IndexBuffer::addIndex(unsigned int count,unsigned int numBytesPerIndex,const void* indexData)
	{
		assert(numBytesPerIndex <= 4);
		unsigned int index ;
		const char* data = (const char*)indexData;
		for (unsigned int i = 0; i <  count; ++i)
		{
			index = 0;
			memcpy(&index,data,numBytesPerIndex);
			mIndexList.push_back(index);
			data = data + numBytesPerIndex;
				
		}
	}

	const size_t IndexBuffer::getIndexCount()const
	{
		return mIndexList.size();
	}

	unsigned int IndexBuffer::operator [](unsigned int index)const
	{
		assert(index < mIndexList.size());
		return mIndexList[index];
	}

}