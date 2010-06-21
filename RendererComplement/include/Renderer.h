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

		//��ʼ����Ⱦ����������Ⱦ����ͱ�׼
		void initialize(RendererParameters rp);
		//������Ⱦ����������
		void setup(RendererParameters rp);
		//�ͷ���Ⱦ����Դ
		void release();
		//����
		void renderNow();
	public:
		VertexBuffer* createVertexBuffer(unsigned int vertexCount, const VertexDeclaration& decl);
		IndexBuffer* createIndexBuffer();
		//createTexture();
		//createMaterial();
		//createLight();

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
		void setMaterial(Material mat);
		void setMatrix(TransformStateType type,const Matrix4X4& mat);
		void setPaintingMethod(PaintingMethod* pm);
		void setLight(unsigned int index,const Light& l);
		void setViewport(const Viewport& vp);

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
	};
}

#endif//_Renderer_H_