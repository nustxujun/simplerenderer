#ifndef _RenderData_H_
#define _RenderData_H_
#include "Prerequisites.h"
#include "RenderElement.h"
#include "Material.h"
#include "Viewport.h"

namespace RCP
{
	//渲染数据组,一个RenderData绘制出完整的一帧
	class RenderData
	{
		friend class RenderQueue;
	public:
		typedef std::list<RenderElement> RenderElementList;
	public:
		RenderData(RenderQueue* rq);
		~RenderData();
		
		//使用完丢弃
		//会造成类似delete this的效果，所以之后请别调用
		void junk();

		const RenderElementList& getRenderElementList() const;
		void insertRenderElement(unsigned int offset ,unsigned int c, Primitives type,
			VertexBuffer* vb, const Matrix4X4 world[TS_BASALNUM], Texture* tex[8], IndexBuffer* ib,const Material& mat,const Light light[8],const Viewport& vp);
	private:
		
		RenderElementList mRenderElementList;
		RenderQueue* mRenderQueue;
	};
}
#endif//_RenderData_H_
