#ifndef _DefaultPipeline_H_
#define _DefaultPipeline_H_
#include"Prerequisites.h"
#include "Pipeline.h"
#include "Rasterizer.h"
#include "PipelinePlus.h"

namespace RCP
{
	class DefaultPipeline:
		public Pipeline
	{

		typedef std::vector<Vertex> VertexVector;
		typedef std::list<VertexVector> VertexList;

	public:

	public:
		DefaultPipeline();
		virtual ~DefaultPipeline();

		virtual void initImpl() ;
		void execute(const RenderData& renderData,RenderTarget* target);

		void setVertexShader(VertexShader* vs);
		void setPixelShader(PixelShader* ps);
	protected:
	
		void vertexProcessing(const RenderElement& elem,VertexVector& verVec);
		
		void primitiveAssembly(const RenderElement& elem,const VertexVector& verVec);

		//返回true则说明通过，false则剔除
		bool culling(const Primitive& prim);
		bool checkPointInScreen(const Vector4& point);
		void generateNewVertex(Vertex& newVertex,const Vertex& vert1, const Vertex& vert2,float dist1, float dist2);
		template<class T>
		inline void Interpolate(T& result, const T& vec1, const T& vec2, float scale);
		void cliping(const Primitive& prim,Primitive prims[5]);
		void clipingLine(const Primitive& prim,Primitive& resultPrim);
		void clipingTriangle(const Primitive& prim,Primitive prims[5]);
		
		void rasterize(Primitive& prim);
		void homogeneousDivide(Vertex& vert);
		void viewportMapping(Vector4& pos,const Viewport* vp);
		
		void rasterization(RenderTarget* target);

	private:
		Rasterizer mRasterizer;
		DataCollector mDataCollector;
		VertexShader* mVertexShader;

		VertexList mVertexList;	

		//裁減空間
		Vector4 mPlane[6];
	};
}
#endif//_DefaultPipeline_H_
