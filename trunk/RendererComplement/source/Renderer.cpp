#include "Renderer.h"
#include "DefaultPipeline.h"
#include "RenderQueue.h"
#include "BackBuffer.h"
#include "PaintingMethod.h"
#include "VertexBufferManager.h"
#include "IndexBufferManager.h"
#include "TextureManager.h"

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


	//��ʼ����Ⱦ����������Ⱦ����ͱ�׼
	void Renderer::initialize(const RendererParameters& rp)
	{
		//��Ⱦ��ˮ��
		mRenderQueue = new RenderQueue();
		mDefaultPipeline = new DefaultPipeline();
		mDefaultPipeline->initialize(rp);
		mPipeline = mDefaultPipeline;

		//RenderTarget
		mBackBuffer = new BackBuffer();
		mBackBuffer->initialize(rp.backBufferWidth,rp.backBufferHeight,rp.backBufferPixelFormat);
		mRenderTarget = mBackBuffer->getRenderTarget();

		//���Ʒ�ʽ
		mPaitingMethod = NULL;

		//���ֹ�����
		mVertexBufferManager = new VertexBufferManager();
		mIndexBufferManager = new IndexBufferManager();
		mTextureManager = new TextureManager();

		//��������
		mVertexBuffer = NULL;
		mIndexBuffer = NULL;
		memset(mTexture,0,sizeof(mTexture));
		mMaterial = Material::DEFAULT;
		for (unsigned int i = 0; i < 8 ; ++i)
			mLight[i].setEnable(false);
		mViewport = Viewport(0,0, rp.backBufferWidth, rp.backBufferHeight,0.f,1.f);

		mIsInitialized = true;
	}

	//������Ⱦ����������
	void Renderer::setup(RendererParameters rp)
	{
		assert(mIsInitialized);
		mRenderTarget = mBackBuffer->getRenderTarget();
	}

	//�ͷ���Ⱦ����Դ
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
		//������ˮ��
		if (mRenderQueue->isRenderDataReady())
			mPipeline->import(mRenderQueue->postRenderData(),mRenderTarget);

		//���Ƶ���Ļ
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
		mRenderQueue->createRenderElement(beginPrimitiveOffset,
			primitiveCount,type,mVertexBuffer,mat,mSampler,mLight,mIndexBuffer,mMaterial,mViewport);

		//�ָ�����ʼ����ֹ������
		mVertexBuffer = NULL;
		mIndexBuffer = NULL;
		mMaterial = Material::DEFAULT;

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

	void Renderer::SetTextureState(unsigned int index, const TextureState& ts)
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

	IndexBuffer* Renderer::createIndexBuffer()
	{
		assert(mIsInitialized);
		return mIndexBufferManager->createIndexBuffer();
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

}