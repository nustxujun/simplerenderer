#include "Renderer.h"
#include "DefaultPipeline.h"
#include "RenderQueue.h"
#include "BackBuffer.h"
#include "PaintingMethod.h"
#include "VertexBufferManager.h"
#include "IndexBufferManager.h"
#include "TextureManager.h"
#include "FrameBuffer.h"
#include "CommandLine.h"

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
		//��ʼ��CommandLine
		mCommandLine.addProcessorFunc(this,&Renderer::processor);
		//��Ⱦ��ˮ��
		mDefaultPipeline = new DefaultPipeline();
		mDefaultPipeline->initialize(rp);
		mPipeline = mDefaultPipeline;

		//���ֹ�����
		mVertexBufferManager = new VertexBufferManager();
		mIndexBufferManager = new IndexBufferManager();
		mTextureManager = new TextureManager();

		//backbuffer
		mBackBuffer = new BackBuffer();
		mBackBuffer->initialize(rp.backBufferWidth,rp.backBufferHeight,rp.backBufferPixelFormat);

		//assistantbuffer
		mRenderParameter.assistantBuffer[0] = new RenderTarget(rp.backBufferWidth,rp.backBufferHeight,4);
		mRenderParameter.assistantBuffer[1] = new RenderTarget(rp.backBufferWidth,rp.backBufferHeight,4);
		mRenderParameter.frameBuffer = new FrameBuffer(rp.backBufferWidth,rp.backBufferHeight);
		//���ú�FrameBuffer
		mRenderParameter.frameBuffer->setBuffer(BT_COLOUR,mBackBuffer->getRenderTarget());
		mRenderParameter.frameBuffer->setBuffer(BT_DEPTH,mRenderParameter.assistantBuffer[0]);
		mRenderParameter.frameBuffer->setBuffer(BT_STENCIL,mRenderParameter.assistantBuffer[1]);

		//���Ʒ�ʽ
		mPaitingMethod = NULL;



		//��������
		mRenderParameter.vertexBuffer = NULL;
		mRenderParameter.indexBuffer = NULL;
		memset(mRenderParameter.texture,0,sizeof(mRenderParameter.texture));
		mRenderParameter.material = Material::DEFAULT;
		for (unsigned int i = 0; i < 8 ; ++i)
			mRenderParameter.light[i].setEnable(false);
		mRenderParameter.viewport = Viewport(0,0, rp.backBufferWidth, rp.backBufferHeight,0.f,1.f);
		mRenderParameter.renderState.alphaTestFunc = CF_ALWAYS;
		mRenderParameter.renderState.alphaTestRef  = 0;
		mRenderParameter.renderState.zWriteEnable = true;
		mRenderParameter.renderState.zTestEnable = true;
		mRenderParameter.renderState.stencilTestFunc = CF_ALWAYS;
		mRenderParameter.renderState.stencilRef = 0;
		mRenderParameter.renderState.stencilMask = -1;
		mRenderParameter.renderState.stencilWriteMask = -1;
		mRenderParameter.renderState.stencilFail = SO_KEEP;
		mRenderParameter.renderState.stencilPass = SO_KEEP;
		mRenderParameter.renderState.stencilZFail = SO_KEEP;
		mRenderParameter.renderState.alphaBlendEnable = false;
		mRenderParameter.renderState.srcBlend = BM_SRCALPHA;
		mRenderParameter.renderState.destBlend = BM_INVSRCALPHA;
		mRenderParameter.renderState.cullMode = CM_CCW;
		mRenderParameter.renderState.fillMode = FM_SOLID;


		mIsInitialized = true;

	}

	//������Ⱦ����������
	void Renderer::setup(RendererParameters rp)
	{
		initAssert();
		//mRenderParameter.frameBuffer->setBuffer(BT_COLOUR, mBackBuffer->getRenderTarget());
	}

	//�ͷ���Ⱦ����Դ
	void Renderer::release()
	{
		SAFE_DELETE(mDefaultPipeline);
		SAFE_DELETE(mBackBuffer);
		SAFE_DELETE(mVertexBufferManager);
		SAFE_DELETE(mIndexBufferManager);
		SAFE_DELETE(mTextureManager);
		for (int i = 0; i < 2; ++i)
			SAFE_DELETE(mRenderParameter.assistantBuffer[i]);

	}

	void Renderer::renderNow()
	{
		initAssert();
		if (mPaitingMethod == NULL)
			THROW_EXCEPTION("δָ�����Ʒ�����");

		while(!mCommandLine.empty())
			mCommandLine.execute();			
		

		//���Ƶ���Ļ
		mPaitingMethod->paint(mBackBuffer);
	}

	bool Renderer::isInitialized()
	{
		return mIsInitialized;
	}

	void Renderer::draw(Primitives type,unsigned int beginPrimitiveOffset,unsigned int primitiveCount)
	{
		assert(primitiveCount);
		Matrix4X4 mat[TS_BASALNUM];
		memcpy(mat,mRenderParameter.matrices,sizeof (Matrix4X4)*TS_BASALNUM);

		mCommandLine.pushCommand(DRAW,type,beginPrimitiveOffset,primitiveCount);

	}

	void Renderer::setVertexBuffer(VertexBuffer* vb)
	{
		assert(vb);
		mCommandLine.pushCommand(SETVERTEXBUFFER,vb);
	}

	void Renderer::setIndexBuffer(IndexBuffer* ib)
	{
		assert(ib);
		mCommandLine.pushCommand(SETINDEXBUFFER,ib);
	}

	void Renderer::setTexture(unsigned int index,Texture* tex)
	{
		assert(index < 8);
		assert(tex);
		mCommandLine.pushCommand(SETTEXTURE,index,tex);
	}

	void Renderer::setTextureState(unsigned int index, const TextureState& ts)
	{
		assert(index < 8);
		mCommandLine.pushCommand(SETTEXTURESTATE,index,ts);
	}

	void Renderer::setMaterial(Material mat)
	{
		mCommandLine.pushCommand(SETMATERIAL,mat);
	}

	void Renderer::setMatrix(TransformStateType type,const Matrix4X4& mat)
	{
		assert(type < TS_BASALNUM);
		mCommandLine.pushCommand(SETMATRIX,type,mat);
	}


	VertexBuffer* Renderer::createVertexBuffer(unsigned int vertexCount, const VertexDeclaration& decl)
	{
		initAssert();
		return mVertexBufferManager->createVertexBuffer(vertexCount,decl);
	}

	IndexBuffer* Renderer::createIndexBuffer(unsigned int indexCount, IndexFormat indexFormat)
	{
		initAssert();
		return mIndexBufferManager->createIndexBuffer(indexCount,indexFormat);
	}

	Texture* Renderer::createTexture(unsigned int width, unsigned int height, unsigned int numMipmap,TextureType type, PixelFormat pf)
	{
		initAssert();
		return mTextureManager->createTexture( width, height, numMipmap,type,pf);
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
		mCommandLine.pushCommand(SETLIGHT,index,l);
	}

	void Renderer::setRenderState(const RenderState& rs)
	{
		mCommandLine.pushCommand(SETRENDERSTATE,rs);
	}

	void Renderer::clearDepth(float d)
	{
		mCommandLine.pushCommand(CLEARDEPTH,d);
	}

	void Renderer::clearStencil(unsigned int s)
	{
		mCommandLine.pushCommand(CLEARSTENCIL,s);
	}

	void Renderer::clearColour(const Colour& c)
	{
		mCommandLine.pushCommand(CLEARCOLOR,c.get32BitARGB());
	}

	void Renderer::setProperty(const std::string& funcName,const Any& attribute)
	{
		mCommandLine.pushCommand(SETPROPERTY,funcName,attribute);
	}

	void Renderer::setRenderTarget(unsigned int index,RenderTarget* rt)
	{
		mCommandLine.pushCommand(SETRENDERTARGET,index,rt);
	}

	RenderTarget* Renderer::getBackBuffer()
	{
		return mBackBuffer->getRenderTarget();
	}

	void Renderer::initAssert()
	{
		if (!mIsInitialized)
			THROW_EXCEPTION("��Ⱦ��δ��ʼ����");
	}

	void Renderer::processor(unsigned int index, const CommandLine::ParameterList& paras)
	{
		initAssert();
		switch (index)
		{
		case DRAW:
			{
				assert(mRenderParameter.vertexBuffer != NULL);
				RenderData data(any_cast<Primitives>(paras[0]), any_cast<unsigned int>(paras[1]), any_cast<unsigned int>(paras[2]), mRenderParameter);
				mPipeline->import(data);
				//�ָ�����ʼ����ֹ������
				mRenderParameter.vertexBuffer = NULL;
				mRenderParameter.indexBuffer = NULL;
				mRenderParameter.material = Material::DEFAULT;

				//viewportҲ�ָ���Ĭ�ϴ�С����ֹ�ı�renderTarget��ʱ������С����
				mRenderParameter.viewport.width = mRenderParameter.frameBuffer->getBuffer(BT_COLOUR)->getWidth();
				mRenderParameter.viewport.height = mRenderParameter.frameBuffer->getBuffer(BT_COLOUR)->getHeight();
				mRenderParameter.viewport.x = mRenderParameter.viewport.y = 0;
				mRenderParameter.viewport.zMax = 1.0f;
				mRenderParameter.viewport.zMin = 0.0f;
				break;
			}
			case SETVERTEXBUFFER:
			{
				mRenderParameter.vertexBuffer = any_cast<VertexBuffer*>(paras[0]);
				break;
			}
			case SETINDEXBUFFER:
			{
				mRenderParameter.indexBuffer = any_cast<IndexBuffer*>(paras[0]);
				break;
			}
			case SETTEXTURE:
			{
				mRenderParameter.sampler[any_cast<unsigned int >(paras[0])].texture = any_cast<Texture*>(paras[1]);
				break;
			}
			case SETTEXTURESTATE:
			{
				mRenderParameter.sampler[any_cast<unsigned int >(paras[0])].setTextureState(any_cast<TextureState>(paras[1]));
				break;
			}
			case SETMATERIAL:
			{
				mRenderParameter.material = any_cast<Material>(paras[0]);
				break;
			}
			case SETMATRIX:
			{
				mRenderParameter.matrices[any_cast<TransformStateType>(paras[0])] = any_cast<Matrix4X4>(paras[1]);
				break;
			}
			case SETLIGHT:
			{
				mRenderParameter.light[any_cast<unsigned int >(paras[0])] = any_cast<Light>(paras[1]);
				break;
			}
			case SETVIEWPORT:
			{
				mRenderParameter.viewport = any_cast<Viewport>(paras[0]);
				break;
			}
			case SETRENDERSTATE:
			{
				mRenderParameter.renderState = any_cast<RenderState>(paras[0]);
				break;
			}
			case CLEARDEPTH:
			{
				mRenderParameter.frameBuffer->clear(BT_DEPTH,any_cast<float>(paras[0]));
				break;
			}
			case CLEARSTENCIL:
			{				
				mRenderParameter.frameBuffer->clear(BT_STENCIL,any_cast<unsigned int>(paras[0]));
				break;
			}
			case CLEARCOLOR:
			{
				mRenderParameter.frameBuffer->clear(BT_COLOUR,any_cast<unsigned int>(paras[0]));
				break;
			}
			case SETPROPERTY:
			{
				mRenderParameter.propertys[any_cast<std::string>(paras[0])] = paras[1];
				break;
			}
			case SETRENDERTARGET:
			{
				//paras[0]������������mrt
				RenderTarget* rt = any_cast<RenderTarget*>(paras[1]);
				mRenderParameter.frameBuffer->setBuffer(BT_COLOUR,rt);
				mRenderParameter.viewport.width = rt->getWidth();
				mRenderParameter.viewport.height = rt->getHeight();
				mRenderParameter.viewport.x = mRenderParameter.viewport.y = 0;
				mRenderParameter.viewport.zMax = 1.0f;
				mRenderParameter.viewport.zMin = 0.0f;
				break;
			}
			default:
				THROW_EXCEPTION("Unknown operation.");
		}
	}


}