#ifndef _Renderer_H_
#define _Renderer_H_

#include"Prerequisites.h"
#include "RendererTypes.h"
#include "Matrix4X4.h"
#include "Material.h"
#include "Light.h"
#include "Viewport.h"



namespace RCP
{

	class Renderer
	{
	public :
		Renderer();
		~Renderer();

		//初始化渲染器，启动渲染器最低标准
		void initialize(RendererParameters rp);
		//设置渲染器基本属性
		void setup(RendererParameters rp);
		//释放渲染器资源
		void release();
		//绘制
		void renderNow();
	public:
		VertexBuffer* createVertexBuffer(unsigned int vertexCount, const VertexDeclaration& decl);
		IndexBuffer* createIndexBuffer();
		//createTexture();
		//createMaterial();
		//createLight();

		/*
		para:
			1.图元类型
			2.起始图元数
			3.所绘图元数
		*/
		void draw(Primitives type,unsigned int beginPrimitiveOffset,unsigned int primitiveCount);
		//我就不玩多流了。。
		void setVertexBuffer(VertexBuffer* vb);
		void setIndexBuffer(IndexBuffer* ib);
		void setTexture(unsigned int index,Texture* tex);
		void setMaterial(Material mat);
		void setMatrix(TransformStateType type,const Matrix4X4& mat);
		void setPaintingMethod(PaintingMethod* pm);
		void setLight(unsigned int index,const Light& l);
		void setViewport(const Viewport& vp);

	private:
		bool mIsInitialized;

		RenderQueue* mRenderQueue;
		//当前渲染流水线
		Pipeline* mPipeline;
		//默认渲染流水线
		Pipeline* mDefaultPipeline;
		//后备缓冲
		BackBuffer* mBackBuffer;
		//顶点管理器
		VertexBufferManager* mVertexBufferManager; 
		//索引管理器
		IndexBufferManager* mIndexBufferManager;

		//当前保存量
		//当前绘制内存空间
		RenderTarget* mRenderTarget;
		//当前绘制方法
		PaintingMethod* mPaitingMethod;
		//当前vb
		VertexBuffer* mVertexBuffer;
		//当前ib
		IndexBuffer* mIndexBuffer;
		//当前纹理,限定8层（不是有什么不可实现，只是模拟显卡最大数）
		Texture* mTexture[8];
		//当前材质
		Material mMaterial;
		//当前矩阵
		Matrix4X4 mMatrices[TS_BASALNUM];
		//灯光
		Light mLight[8];
		//视口
		Viewport mViewport;
	};
}

#endif//_Renderer_H_