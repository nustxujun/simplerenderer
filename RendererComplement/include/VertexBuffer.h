#ifndef _VertexBuffer_H_
#define _VertexBuffer_H_
#include"Prerequisites.h"
#include "RendererTypes.h"
#include "Resource.h"

namespace RCP
{
	

	struct VertexElement
	{
		unsigned int index;
		VertexElementType type;
		VertexElementSemantic semantic;
		
		VertexElement(unsigned int i,VertexElementType t, VertexElementSemantic s):
			index(i), type(t), semantic(s)
		{
			
		}

	};

	class VertexDeclaration
	{
	public:
		VertexDeclaration();
		~VertexDeclaration();

		void addElement(VertexElementType type, VertexElementSemantic semantic, 
			unsigned int index = 0,unsigned int position = -1);
		unsigned int getSizeInBytes() const;
		unsigned int getTypeSize(VertexElementType type) const;
		unsigned int getElementOffsetInBytes(VertexElementSemantic semantic, unsigned int index = 0)const;
		unsigned int getElementSizeInBytes(VertexElementSemantic semantic, unsigned int index = 0)const;
		bool hasElement(VertexElementSemantic semantic, unsigned int index = -1)const;
	private:
		unsigned int mSize;
		typedef std::list<VertexElement> VertexElementList;
		VertexElementList mVertexElementList;
	};

	class VertexBuffer:
		public Resource
	{
		friend class VertexBufferManager;
	public:
		~VertexBuffer();
		//ÌîÒ»‚€í”üc
		void fill(unsigned int vertexIndex,const void* vertexData);
		//Ìî¶à‚€í”üc
		void fill(unsigned int beginIndex,unsigned int vertexCount,const void* vertexData);

		const unsigned char* getData()const;
		size_t getDataSizeInBytes()const;
		const VertexDeclaration& getVertexDeclaration()const;
		const unsigned int getVertexCount()const;
		void* lock();
		void unlock();
	private :
		VertexBuffer(const VertexDeclaration& decl,unsigned int vertexCount,VertexBufferManager* mgr);
		void initImpl() ;
	private:
		MemoryBuffer* mData;
		VertexDeclaration mDeclaration;
		unsigned int mVertexCount;
	};


}
#endif//_VertexBuffer_H_
