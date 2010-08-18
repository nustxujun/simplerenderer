#include "Renderer.h"
#include "DefaultPipeline.h"
#include "RenderQueue.h"
#include "BackBuffer.h"
#include "PaintingMethod.h"
#include "VertexBufferManager.h"
#include "IndexBufferManager.h"
#include "TextureManager.h"
#include "FrameBuffer.h"

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
	void Renderer::initialize(const RendererParameters& rp)
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
		mAssistantBuffer[0] = new RenderTarget(rp.backBufferWidth,rp.backBufferHeight,4);
		mAssistantBuffer[1] = new RenderTarget(rp.backBufferWidth,rp.backBufferHeight,4);
		mFrameBuffer = new FrameBuffer(rp.backBufferWidth,rp.backBufferHeight);
		//设置好FrameBuffer
		mFrameBuffer->setBuffer(BT_COLOUR,mRenderTarget);
		mFrameBuffer->setBuffer(BT_DEPTH,mAssistantBuffer[0]);
		mFrameBuffer->setBuffer(BT_STENCIL,mAssistantBuffer[1]);

		//绘制方式
		mPaitingMethod = NULL;

		//各种管理器
		mVertexBufferManager = new VertexBufferManager();
		mIndexBufferManager = new IndexBufferManager();
		mTextureManager = new TextureManager();

		//各种属性
		mVertexBuffer = NULL;
		mIndexBuffer = NULL;
		memset(mTexture,0,sizeof(mTexture));
		mMaterial = Material::DEFAULT;
		for (unsigned int i = 0; i < 8 ; ++i)
			mLight[i].setEnable(false);
		mViewport = Viewport(0,0, rp.backBufferWidth, rp.backBufferHeight,0.f,1.f);
		mIsInitialized = true;
		mRenderState.alphaTestFunc = CF_ALWAYS;
		mRenderState.alphaTestRef  = 0;
		mRenderState.zWriteEnable = true;
		mRenderState.zTestEnable = true;
		mRenderState.stencilTestFunc = CF_ALWAYS;
		mRenderState.stencilRef = 0;
		mRenderState.stencilMask = -1;
		mRenderState.stencilWriteMask = -1;
		mRenderState.stencilFail = SO_KEEP;
		mRenderState.stencilPass = SO_KEEP;
		mRenderState.stencilZFail = SO_KEEP;
		mRenderState.alphaBlendEnable = false;
		mRenderState.srcBlend = BM_SRCALPHA;
		mRenderState.destBlend = BM_INVSRCALPHA;
		mRenderState.cullMode = CM_CCW;
		mRenderState.fillMode = FM_SOLID;


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
		SAFE_DELETE(mVertexBufferManager);
		SAFE_DELETE(mIndexBufferManager);
		SAFE_DELETE(mTextureManager);
		for (int i = 0; i < 2; ++i)
			SAFE_DELETE(mAssistantBuffer[i]);

	}

	void Renderer::renderNow()
	{
		assert(mIsInitialized);
		assert(mPaitingMethod);
		//输入流水线
		if (mRenderQueue->isRenderDataReady())
			mPipeline->import(mRenderQueue->postRenderData());

		//绘制道屏幕
		mPaitingMethod->paint(mBackBuffer);
	}

	bool Renderer::isInitialized()
	{
		return mIsInitialized;
	}

	void Renderer::draw(Primitives type,unsigned int beginPrimitiveOffset,unsigned int primitiveCount)
	{
		assert(mIsInitialized);
		assert(mVertexBuffer != NULL);
		assert(primitiveCount);
		Matrix4X4 mat[TS_BASALNUM];
		memcpy(mat,mMatrices,sizeof (Matrix4X4)*TS_BASALNUM);
		mRenderQueue->createRenderElement(beginPrimitiveOffset,primitiveCount,type,mVertexBuffer,
			mat,mSampler,mLight,mIndexBuffer,mMaterial,mViewport,mRenderState,*mFrameBuffer,mPropertys);

		//恢复到初始，防止被再用
		mVertexBuffer = NULL;
		mIndexBuffer = NULL;
		mMaterial = Material::DEFAULT;
		//去掉清除標識
		mFrameBuffer->reset();

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
		mSampler[index].texture = tex;
	}

	void Renderer::setTextureState(unsigned int index, const TextureState& ts)
	{
		assert(index < 8);
		mSampler[index].setTextureState(ts);
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

	IndexBuffer* Renderer::createIndexBuffer(unsigned int indexCount, IndexFormat indexFormat)
	{
		assert(mIsInitialized);
		return mIndexBufferManager->createIndexBuffer(indexCount,indexFormat);
	}

	Texture* Renderer::createTexture(unsigned int width, unsigned int height, unsigned int numMipmap, PixelFormat pf)
	{
		assert(mIsInitialized);
		return mTextureManager->createTexture( width, height, numMipmap, pf);
	}

	void Renderer::setPaintingMethod(PaintingMethod* pm)
	{
		assert(pm);
		mPaitingMethod = pm;
	}

	void Renderer::setPipeline(Pipeline* pl)
	{
		assert(pl);
		pl->initialize(mPipeline->getRendererParameters());
		mPipeline = pl;

	}

	void Renderer::setLight(unsigned int index,const Light& l)
	{
		assert(index < 8);
		mLight[index] = l;
	}

	void Renderer::setRenderState(const RenderState& rs)
	{
		mRenderState = rs;
	}

	void Renderer::clearDepth(float d)
	{
		mFrameBuffer->clear(BT_DEPTH,d);
	}

	void Renderer::clearStencil(unsigned int s)
	{
		mFrameBuffer->clear(BT_STENCIL,s);
	}

	void Renderer::clearColour(const Colour& c)
	{
		mFrameBuffer->clear(BT_COLOUR,c.get32BitARGB());
	}

	void Renderer::setProperty(const std::string& funcName,const Any& attribute)
	{
		mPropertys[funcName] = attribute;
	}

	void Renderer::setRenderTarget(unsigned int index,RenderTarget* rt)
	{
		mFrameBuffer->setBuffer(BT_COLOUR, rt);
	}

	RenderTarget* Renderer::getRenderTarget(unsigned int index )
	{
		return mFrameBuffer->getBuffer(BT_COLOUR);
	}


}