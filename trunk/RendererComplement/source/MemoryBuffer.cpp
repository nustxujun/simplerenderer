#include "MemoryBuffer.h"

namespace RCP
{

	MemoryBuffer::~MemoryBuffer()
	{
		if (mAutoFree)
		{
			SAFE_DELETE(mData);
		}
	}


	MemoryBuffer::MemoryBuffer(unsigned char* data,size_t size,bool autoFree)
	{
		mPos = mData = data;
		mEnd = mData + size;
		mSize = size;
		mAutoFree = autoFree;
	}

	MemoryBuffer::MemoryBuffer(const MemoryBuffer& mem)
	{
		mSize = mem.getSizeInBytes();
		mPos = mData = new unsigned char[mSize];
		mEnd = mData + mSize;
		//mem.read(mData,mSize);
		memcpy(mData,mem.mData,mSize);
		mAutoFree = true;
	}

	template <typename T>
	MemoryBuffer& MemoryBuffer::operator >> (T& val)
	{
		read(static<void*>(&val),sizeof(T));
		return *this;
	}

	size_t MemoryBuffer::read(void * buf,size_t count)
	{
		assert(mEnd >= mPos + count);
		memcpy(buf,mPos,count);
		mPos += count;
		return (mPos - mData);
	}

	size_t MemoryBuffer::write(const void* buf,size_t count)
	{
		assert(mEnd >= mPos + count);
		memcpy(mPos,buf,count);
		mPos += count;
		return (mPos - mData);
	}

	void MemoryBuffer::seek(size_t pos)
	{
		assert(mEnd >= mData + pos);
		mPos = mData + pos;
	}

	size_t MemoryBuffer::tell()const 
	{
		return (mPos - mData);
	}

	bool MemoryBuffer::eof()const
	{
		return mPos >= mEnd;
	}

	unsigned char* MemoryBuffer::getData()const
	{
		return mData;
	}

	size_t MemoryBuffer::getSizeInBytes()const
	{
		return mSize;
	}

	void MemoryBuffer::setAutoFree(bool enable)
	{
		mAutoFree = enable;
	}

	bool MemoryBuffer::isAutoFree()
	{
		return mAutoFree;
	}

	void MemoryBuffer::swap(MemoryBuffer& mem)
	{
		std::swap(mData,mem.mData);
		std::swap(mPos,mem.mPos);
		std::swap(mEnd,mem.mEnd);
		std::swap(mSize,mem.mSize);
		std::swap(mAutoFree,mem.mAutoFree);
	}



}