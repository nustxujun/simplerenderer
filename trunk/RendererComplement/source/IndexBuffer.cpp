#include "IndexBuffer.h"
#include "MemoryBuffer.h"
namespace RCP
{
	IndexBuffer::~IndexBuffer()
	{
		SAFE_DELETE(mData);
	}

	IndexBuffer::IndexBuffer(unsigned int indexCount, IndexFormat indexformat,IndexBufferManager* mgr):
		mIndexCount(indexCount),mIndexFormat(indexformat),Resource((ResourceManager*)mgr)
	{

	}

	void IndexBuffer::initImpl() 
	{


		size_t size = getSizePerIndex() * mIndexCount;
		unsigned char* data = new unsigned char[size];
		mData = new MemoryBuffer(data,size);	
	}


	void IndexBuffer::fillIndex(unsigned int vertexIndex,unsigned int pos )
	{
		mData->seek(pos *  getSizePerIndex());
		mData->write(&vertexIndex,getSizePerIndex());
	}

	void IndexBuffer::fillIndex(unsigned int beginIndex,unsigned int indexCount,const void* indexData)
	{
		mData->seek(beginIndex *  getSizePerIndex());
		mData->write(indexData,indexCount * getSizePerIndex());
	}

	const size_t IndexBuffer::getIndexCount()const
	{
		return mIndexCount;
	}

	unsigned int IndexBuffer::operator [](unsigned int index)const
	{
		if (index >= mIndexCount)
			THROW_EXCEPTION("Ô½½ç");
		unsigned int value = 0;
		memcpy(&value,mData->getData() + index * getSizePerIndex(),getSizePerIndex());
		return value;
	}

	IndexFormat IndexBuffer::getIndexFormat()const
	{
		return mIndexFormat;
	}

	unsigned int IndexBuffer::getSizePerIndex()const
	{
		switch(mIndexFormat)
		{
		case IF_INDEX16:
			return 2;
			break;
		case IF_INDEX32:
			return 4;
			break;
		default:
			assert(0);
			return 0;
		}
	
	}

	void* IndexBuffer::lock()
	{
		return mData->getData();
	}

	void IndexBuffer::unlock()
	{}

}