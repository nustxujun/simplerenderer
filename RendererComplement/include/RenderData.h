#ifndef _RenderData_H_
#define _RenderData_H_
#include "Prerequisites.h"
#include "RenderElement.h"
#include "Material.h"
#include "Viewport.h"

namespace RCP
{
	//��Ⱦ������,һ��RenderData���Ƴ�������һ֡
	class RenderData
	{
		friend class RenderQueue;
	public:
		typedef std::list<RenderElement> RenderElementList;
	public:
		RenderData(RenderQueue* rq);
		~RenderData();
		
		//ʹ���궪��
		//���������delete this��Ч��������֮��������
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
