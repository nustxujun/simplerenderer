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

		//初始化渲染器，启动渲染器最低标准
		void initialize(const RendererParameters& rp);
		//设置渲染器基本属性
		void setup(RendererParameters rp);
		//释放渲染器资源
		void release();
		//绘制
		void renderNow();

		bool isInitialized();
	public:
		VertexBuffer* createVertexBuffer(unsigned int vertexCount, const VertexDeclaration& decl);
		IndexBuffer* createIndexBuffer(unsigned int indexCount, IndexFormat indexFormat);
		Texture* createTexture(unsigned int width, unsigned int height, unsigned int numMipmap,TextureType type, PixelFormat pf);

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
		//index默认0，等到完善MRT的时候再实现
		void setRenderTarget(unsigned int index,RenderTarget* rt);
		RenderTarget* getBackBuffer();

	private:
		void initAssert();
		void processor(unsigned int index, const CommandLine::ParameterList& paras);

	private:
		bool mIsInitialized;

		//当前渲染流水线
		Pipeline* mPipeline;
		//默认渲染流水线
		Pipeline* mDefaultPipeline;
		//交换链（直译？）
		SwapChain* mSwapChain;
		//顶点管理器
		VertexBufferManager* mVertexBufferManager; 
		//索引管理器
		IndexBufferManager* mIndexBufferManager;
		//纹理管理器
		TextureManager* mTextureManager;

		//当前绘制方法
		PaintingMethod* mPaitingMethod;

		RenderParameter mRenderParameter;

		//commandLine
		CommandLine mCommandLine;


	};
}

#endif//_Renderer_H_