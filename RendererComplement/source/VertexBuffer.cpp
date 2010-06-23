#include "VertexBuffer.h"
#include "MemoryBuffer.h"
namespace RCP
{
	VertexBuffer::~VertexBuffer()
	{
		SAFE_DELETE(mData);
	}

	VertexBuffer::VertexBuffer(const VertexDeclaration& decl,unsigned int vertexCount,VertexBufferManager* mgr):
		mDeclaration(decl),	mVertexCount(vertexCount), Resource((ResourceManager*)mgr)
	{
		size_t size = decl.getSizeInBytes() * vertexCount;
		unsigned char* data = new unsigned char[size];
		mData = new MemoryBuffer(data,size);
	}

	void VertexBuffer::fill(unsigned int vertexIndex, const void* vertexData)
	{
		unsigned int vertexSize = mDeclaration.getSizeInBytes();
		mData->seek(vertexIndex * vertexSize);
		mData->write(vertexData,vertexSize);
	}

	void VertexBuffer::fill(unsigned int beginIndex,unsigned int vertexCount, const void* vertexData)
	{
		unsigned int vertexSize = mDeclaration.getSizeInBytes();
		mData->seek(beginIndex *  vertexSize);
		mData->write(vertexData,vertexCount * vertexSize);
	}

	const unsigned char* VertexBuffer::getData()const
	{
		return mData->getData();
	}

	size_t VertexBuffer::getDataSizeInBytes()const
	{
		return mData->getSizeInBytes();
	}

	const VertexDeclaration& VertexBuffer::getVertexDeclaration()const
	{
		return mDeclaration;
	}
	const unsigned int VertexBuffer::getVertexCount()const
	{
		return mVertexCount;
	}

	VertexDeclaration::VertexDeclaration():
		mSize(0)
	{
	
	}

	VertexDeclaration::~VertexDeclaration()
	{
	
	}

	void VertexDeclaration::addElement(VertexElementType type, VertexElementSemantic semantic,unsigned int index,unsigned int position)	
	{
		if (position >= mVertexElementList.size())
		{
			mVertexElementList.push_back(VertexElement(index,type,semantic));
		}
		else
		{
			VertexElementList::iterator iter = mVertexElementList.begin();
			for (unsigned int i = 0; i < position; ++i)
				++iter;
			mVertexElementList.insert(iter, VertexElement(index,type,semantic));
		}
		mSize += getTypeSize(type);
	}

	unsigned int VertexDeclaration::getTypeSize(VertexElementType type)const 
	{
		switch(type)
		{
		case VET_COLOUR:
		case VET_COLOUR_ABGR:
		case VET_COLOUR_ARGB:
			return sizeof(unsigned char)*4 ;
		case VET_FLOAT1:
			return sizeof(float);
		case VET_FLOAT2:
			return sizeof(float)*2;
		case VET_FLOAT3:
			return sizeof(float)*3;
		case VET_FLOAT4:
			return sizeof(float)*4;
		case VET_SHORT1:
			return sizeof(short);
		case VET_SHORT2:
			return sizeof(short)*2;
		case VET_SHORT3:
			return sizeof(short)*3;
		case VET_SHORT4:
			return sizeof(short)*4;
        case VET_UBYTE4:
            return sizeof(unsigned char)*4;
		}
		return 0;
	}

	unsigned int VertexDeclaration::getSizeInBytes() const
	{
		return mSize;
	}

	unsigned int VertexDeclaration::getElementOffsetInBytes(VertexElementSemantic semantic, unsigned int index )const
	{
		VertexElementList::const_iterator i,endi = mVertexElementList.end();
		unsigned int offset = 0;
		for (i = mVertexElementList.begin(); i != endi; ++i)
		{
			if (i->semantic == semantic && i->index == index)
				return offset;
			offset += getTypeSize(i->type);
		}
		return -1;
	}

	unsigned int VertexDeclaration::getElementSizeInBytes(VertexElementSemantic semantic, unsigned int index )const
	{
		VertexElementList::const_iterator i,endi = mVertexElementList.end();
		for (i = mVertexElementList.begin(); i != endi; ++i)
		{
			if (i->semantic == semantic && i->index == index)
				return getTypeSize(i->type);
			
		}
		return -1;
	}

}