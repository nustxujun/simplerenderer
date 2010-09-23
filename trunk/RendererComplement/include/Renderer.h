#ifndef _Renderer_H_
#define _Renderer_H_

#include"Prerequisites.h"
#include "RendererTypes.h"
#include "Matrix4X4.h"
#include "Material.h"
#include "Light.h"
#include "Viewport.h"
#include "Sampler.h"
#include "CommandLine.h"
#include "RenderData.h"


namespace RCP
{

	class Renderer
	{
	private:
		enum
		{
			DRAW,
			SETVERTEXBUFFER,
			SETINDEXBUFFER,
			SETTEXTURE,
			SETTEXTURESTATE,
			SETMATERIAL,
			SETMATRIX,
			SETPAINTINGMETHOD,
			SETLIGHT,			
			SETVIEWPORT,
			SETPIPELINE,
			SETRENDERSTATE,
			CLEARDEPTH,
			CLEARSTENCIL,
			CLEARCOLOR,
			SETPROPERTY,
			SETRENDERTARGET,

			FUNC_NUM,
		};
	public :
		Renderer();
		~Renderer();

		//��ʼ����Ⱦ����������Ⱦ����ͱ�׼
		void initialize(const RendererParameters& rp);
		//������Ⱦ����������
		void setup(RendererParameters rp);
		//�ͷ���Ⱦ����Դ
		void release();
		//����
		void renderNow();

		bool isInitialized();
	public:
		VertexBuffer* createVertexBuffer(unsigned int vertexCount, const VertexDeclaration& decl);
		IndexBuffer* createIndexBuffer(unsigned int indexCount, IndexFormat indexFormat);
		Texture* createTexture(unsigned int width, unsigned int height, unsigned int numMipmap,TextureType type, PixelFormat pf);

		/*
		para:
			1.ͼԪ����
			2.��ʼͼԪ��
			3.����ͼԪ��
		*/
		void draw(Primitives type,unsigned int beginPrimitiveOffset,unsigned int primitiveCount);
		//�ҾͲ�������ˡ���
		void setVertexBuffer(VertexBuffer* vb);
		void setIndexBuffer(IndexBuffer* ib);
		void setTexture(unsigned int index,Texture* tex);
		void setTextureState(unsigned int index, const TextureState& ts);
		void setMaterial(Material mat);
		void setMatrix(TransformStateType type,const Matrix4X4& mat);
		void setPaintingMethod(PaintingMethod* pm);
		void setLight(unsigned int index,const Light& l);
		void setViewport(const Viewport& vp);
		void setPipeline(Pipeline* pl);
		void setRenderState(const RenderState& rs);
		void clearDepth(float d);
		void clearStencil(unsigned int s);
		void clearColour(const Colour& c);
		void setProperty(const std::string& funcName,const Any& attribute);
		//indexĬ��0���ȵ�����MRT��ʱ����ʵ��
		void setRenderTarget(unsigned int index,RenderTarget* rt);
		RenderTarget* getBackBuffer();

	private:
		void initAssert();
		void processor(unsigned int index, const CommandLine::ParameterList& paras);

	private:
		bool mIsInitialized;

		//��ǰ��Ⱦ��ˮ��
		Pipeline* mPipeline;
		//Ĭ����Ⱦ��ˮ��
		Pipeline* mDefaultPipeline;
		//��������ֱ�룿��
		SwapChain* mSwapChain;
		//���������
		VertexBufferManager* mVertexBufferManager; 
		//����������
		IndexBufferManager* mIndexBufferManager;
		//���������
		TextureManager* mTextureManager;

		//��ǰ���Ʒ���
		PaintingMethod* mPaitingMethod;

		RenderParameter mRenderParameter;

		//commandLine
		CommandLine mCommandLine;


	};
}

#endif//_Renderer_H_