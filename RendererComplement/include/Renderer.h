#ifndef _Renderer_H_
#define _Renderer_H_

#include"Prerequisites.h"
#include "RendererTypes.h"
#include "Matrix4X4.h"
#include "Material.h"
#include "Light.h"
#include "Viewport.h"
#include "Sampler.h"


namespace RCP
{

	class Renderer
	{
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
		IndexBuffer* createIndexBuffer();
		Texture* createTexture(unsigned int width, unsigned int height, unsigned int numMipmap, PixelFormat pf);

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
		void SetTextureState(unsigned int index, const TextureState& ts);
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

	private:
		bool mIsInitialized;

		RenderQueue* mRenderQueue;
		//��ǰ��Ⱦ��ˮ��
		Pipeline* mPipeline;
		//Ĭ����Ⱦ��ˮ��
		Pipeline* mDefaultPipeline;
		//�󱸻���
		BackBuffer* mBackBuffer;
		//���������
		VertexBufferManager* mVertexBufferManager; 
		//����������
		IndexBufferManager* mIndexBufferManager;
		//���������
		TextureManager* mTextureManager;

		//��ǰ������
		//��ǰ�����ڴ�ռ�
		RenderTarget* mRenderTarget;
		//��ǰ���Ʒ���
		PaintingMethod* mPaitingMethod;
		//��ǰvb
		VertexBuffer* mVertexBuffer;
		//��ǰib
		IndexBuffer* mIndexBuffer;
		//��ǰ����,�޶�8�㣨������ʲô����ʵ�֣�ֻ��ģ���Կ��������
		Texture* mTexture[8];
		//��ǰ����
		Material mMaterial;
		//��ǰ����
		Matrix4X4 mMatrices[TS_BASALNUM];
		//�ƹ�
		Light mLight[8];
		//�ӿ�
		Viewport mViewport;
		//����״̬
		Sampler mSampler[8];
		//��Ⱦ״̬
		RenderState mRenderState;
		//frameBuffer
		FrameBuffer* mFrameBuffer;
		//assistantBuffer,��ʱ��z + stencil
		RenderTarget* mAssistantBuffer[2];

	};
}

#endif//_Renderer_H_