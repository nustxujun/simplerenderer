#ifndef _MemoryBuffer_H_
#define _MemoryBuffer_H_
#include "Prerequisites.h"

namespace RCP
{

	class MemoryBuffer
	{
	public:
		virtual ~MemoryBuffer();

		MemoryBuffer(unsigned char* data,size_t size,bool autoFree = true);
		//���&�����ڲ�new���Ա����Զ�free
		MemoryBuffer(const MemoryBuffer& mem);
	public:
		template <typename T>
		MemoryBuffer& operator >> (T& val);
	
	public:
		//���ص�ǰ����λ��
		size_t read(void * buf,size_t count);
		size_t write(const void* buf,size_t count);
		void seek(size_t pos); 
		size_t tell()const ;
		bool eof()const;
		unsigned char* getData()const;
		size_t getSizeInBytes()const;

		void setAutoFree(bool enable);
		bool isAutoFree();

		void swap(MemoryBuffer& mem);
	protected:
		unsigned char* mData;
		unsigned char* mPos;
		unsigned char* mEnd;
		size_t mSize;

	private:
		bool mAutoFree;	

	private:
		MemoryBuffer();

	};
}
#endif//_MemoryBuffer_H_
