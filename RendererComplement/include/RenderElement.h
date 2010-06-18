#ifndef _RenderElement_H_
#define _RenderElement_H_

#include "Prerequisites.h"
#include "Matrix4X4.h"
#include "RendererTypes.h"

namespace RCP
{
	class RenderElement
	{
	public:
		VertexBuffer* vertexBuffer;
		IndexBuffer* indexBuffer;
		unsigned int beginPrimitiveOffset;
		unsigned int primitiveCount;
		Texture* texture[8];
		Material* material;
		Matrix4X4 matWorld[TS_BASALNUM];
		Primitives ptType;


		RenderElement(unsigned int offset ,unsigned int c, Primitives type, VertexBuffer* vb, const Matrix4X4 world[TS_BASALNUM],
			Texture* tex[4],IndexBuffer* ib = NULL, Material* mat = NULL):
			beginPrimitiveOffset(offset),
			primitiveCount(c),
			vertexBuffer(vb),
			indexBuffer(ib),
			material(mat),		
			ptType(type)
		{
			memcpy(texture,tex,sizeof(Texture*)*8);
			memcpy(matWorld,world,sizeof(Matrix4X4)*TS_BASALNUM);	
		}


	};
}
#endif//_RenderElement_H_
