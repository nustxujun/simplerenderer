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

		typedef std::vector<Vertex> VertexList;


	public:

	public:
		DefaultPipeline();
		virtual ~DefaultPipeline();

		virtual void initImpl() ;
		void execute(const RenderData& renderData);

	protected:
	
		void vertexProcessing(const RenderData& elem,VertexList& verVec);
		
		void primitiveAssembly(const RenderData& elem,const VertexList& verVec);

		//返回true则说明通过，false则剔除
		bool culling(const Primitive& prim,CullMode fm);
		bool checkPointInScreen(const Vector4& point);
		void generateNewVertex(Vertex& newVertex,const Vertex& vert1, const Vertex& vert2,float dist1, float dist2);
		void clipping(const Primitive& prim,Primitive prims[5]);
		void clippingPoint(const Primitive& prim,Primitive& resultPrim);
		void clippingLine(const Primitive& prim,Primitive& resultPrim);
		void clippingTriangle(const Primitive& prim,Primitive prims[5]);
		
		void rasterize(Primitive& prim);
		void homogeneousDivide(Vertex& vert);
		void viewportMapping(Vector4& pos,const Viewport* vp);


		void setOtherState(const std::map<std::string,Any>& p);
		void setVertexShader(VertexShader* vs);
		void setPixelShader(PixelShader* ps);

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
