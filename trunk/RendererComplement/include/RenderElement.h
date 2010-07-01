#ifndef _RenderElement_H_
#define _RenderElement_H_

#include "Prerequisites.h"
#include "Matrix4X4.h"
#include "RendererTypes.h"
#include "Material.h"
#include "Light.h"
#include "Sampler.h"
#include "Viewport.h"
namespace RCP
{
	class RenderElement
	{
	public:
		VertexBuffer* vertexBuffer;
		IndexBuffer* indexBuffer;
		unsigned int beginPrimitiveOffset;
		unsigned int primitiveCount;
		Sampler sampler[8];
		Material material;
		Matrix4X4 matWorld[TS_BASALNUM];
		Primitives ptType;
		Light light[8];
		Viewport viewport;

		RenderElement(unsigned int offset ,unsigned int c, Primitives type, VertexBuffer* vb, const Matrix4X4 world[TS_BASALNUM],
			const Sampler spl[8],IndexBuffer* ib,const Material& mat,const Light l[8],const Viewport& vp):
			beginPrimitiveOffset(offset),
			primitiveCount(c),
			vertexBuffer(vb),
			indexBuffer(ib),
			material(mat),		
			ptType(type),
			viewport(vp)
		{
			
			memcpy(matWorld,world,sizeof(Matrix4X4)*TS_BASALNUM);	
			for (unsigned int i = 0; i < 8 ; ++i)
			{
				light[i] = l[i];
				sampler[i] = spl[i];
			}
		}


	};
}
#endif//_RenderElement_H_
