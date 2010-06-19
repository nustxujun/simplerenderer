#ifndef _RenderElement_H_
#define _RenderElement_H_

#include "Prerequisites.h"
#include "Matrix4X4.h"
#include "RendererTypes.h"
#include "Material.h"
#include "Light.h"
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
		Material material;
		Matrix4X4 matWorld[TS_BASALNUM];
		Primitives ptType;
		Light light[8];

		RenderElement(unsigned int offset ,unsigned int c, Primitives type, VertexBuffer* vb, const Matrix4X4 world[TS_BASALNUM],
			Texture* tex[4],IndexBuffer* ib,const Material& mat,const Light l[8]):
			beginPrimitiveOffset(offset),
			primitiveCount(c),
			vertexBuffer(vb),
			indexBuffer(ib),
			material(mat),		
			ptType(type)
		{
			memcpy(texture,tex,sizeof(Texture*)*8);
			memcpy(matWorld,world,sizeof(Matrix4X4)*TS_BASALNUM);	
			for (unsigned int i = 0; i < 8 ; ++i)
				light[i] = l[i];
		}


	};
}
#endif//_RenderElement_H_
