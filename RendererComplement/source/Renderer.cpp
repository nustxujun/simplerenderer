#include "Renderer.h"
#include "DefaultPipeline.h"
#include "RenderQueue.h"
#include "BackBuffer.h"
#include "PaintingMethod.h"
#include "VertexBufferManager.h"
#include "IndexBufferManager.h"


namespace RCP 
{
	Renderer::Renderer():
		mIsInitialized(false)
	{

	}

	Renderer::~Renderer()
	{
		release();
	}


	//初始化渲染器，启动渲染器最低标准
	void Renderer::initialize(RendererParameters rp)
	{
		//渲染流水线
		mRenderQueue = new RenderQueue();
		mDefaultPipeline = new DefaultPipeline();
		mDefaultPipeline->initialize(rp);
		mPipeline = mDefaultPipeline;

		//RenderTarget
		mBackBuffer = new BackBuffer();
		mBackBuffer->initialize(rp.backBufferWidth,rp.backBufferHeight,rp.backBufferPixelFormat);
		mRenderTarget = mBackBuffer->getRenderTarget();

		//绘制方式
		mPaitingMethod = NULL;

		//各种管理器
		mVertexBufferManager = new VertexBufferManager();
		mIndexBufferManager = new IndexBufferManager();

		//各种属性
		mVertexBuffer = NULL;
		mIndexBuffer = NULL;
		memset(mTexture,0,sizeof(mTexture));
		mMaterial = Material::DEFAULT;
		for (unsigned int i = 0; i < 8 ; ++i)
			mLight[i].setEnable(false);

		mIsInitialized = true;
	}

	//设置渲染器基本属性
	void Renderer::setup(RendererParameters rp)
	{
		assert(mIsInitialized);
		mRenderTarget = mBackBuffer->getRenderTarget();
	}

	//释放渲染器资源
	void Renderer::release()
	{
		SAFE_DELETE(mRenderQueue);
		SAFE_DELETE(mDefaultPipeline);
		SAFE_DELETE(mBackBuffer);

	}

	void Renderer::renderNow()
	{
		assert(mIsInitialized);
		assert(mPaitingMethod);
		//输入流水线
		if (mRenderQueue->isRenderDataReady())
			mPipeline->import(mRenderQueue->postRenderData(),mRenderTarget);

		//绘制道屏幕
		mPaitingMethod->paint(mBackBuffer);
	}

	void Renderer::draw(Primitives type,unsigned int beginPrimitiveOffset,unsigned int primitiveCount)
	{
		assert(mIsInitialized);
		assert(mVertexBuffer != NULL);
		assert(primitiveCount);
		Matrix4X4 mat[TS_BASALNUM];
		memcpy(mat,mMatrices,sizeof (Matrix4X4)*TS_BASALNUM);
		mRenderQueue->createRenderElement(beginPrimitiveOffset,
			primitiveCount,type,mVertexBuffer,mat,mTexture,mLight,mIndexBuffer,mMaterial);

		//恢复到初始，防止被再用
		mVertexBuffer = NULL;
		mIndexBuffer = NULL;

	}

	void Renderer::setVertexBuffer(VertexBuffer* vb)
	{
		assert(vb);
		mVertexBuffer = vb;
	}

	void Renderer::setIndexBuffer(IndexBuffer* ib)
	{
		assert(ib);
		mIndexBuffer = ib;
	}

	void Renderer::setTexture(unsigned int index,Texture* tex)
	{
		assert(index < 8);
		assert(tex);
		mTexture[index] = tex;
	}

	void Renderer::setMaterial(Material mat)
	{
		mMaterial = mat;
	}

	void Renderer::setMatrix(TransformStateType type,const Matrix4X4& mat)
	{
		assert(type < TS_BASALNUM);
		mMatrices[type] = mat;
	}


	VertexBuffer* Renderer::createVertexBuffer(unsigned int vertexCount, const VertexDeclaration& decl)
	{
		assert(mIsInitialized);
		return mVertexBufferManager->createVertexBuffer(vertexCount,decl);
	}

	IndexBuffer* Renderer::createIndexBuffer()
	{
		assert(mIsInitialized);
		return mIndexBufferManager->createIndexBuffer();
	}

	void Renderer::setPaintingMethod(PaintingMethod* pm)
	{
		mPaitingMethod = pm;
	}

	void Renderer::setLight(unsigned int index,const Light& l)
	{
		assert(index < 8);
		mLight[index] = l;
	}

}