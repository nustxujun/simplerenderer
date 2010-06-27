#ifndef _DefaultPipeline_H_
#define _DefaultPipeline_H_
#include"Prerequisites.h"
#include "Pipeline.h"
#include "Rasterizer.h"

namespace RCP
{
	class DefaultPipeline:
		public Pipeline
	{
		struct Position
		{
			float x,y,z;
		};

		struct Color
		{
			float r,g,b,a;
		};

		struct TexCrood
		{
			float u,v;
		};

		struct Pixel
		{
			Position pos;
		};

		typedef std::vector<Vertex> VertexVector;
		typedef std::list<VertexVector> VertexList;
		typedef std::vector<Primitive> PrimitiveVector;
		typedef std::vector<Pixel> PixelVector;

	public:
		DefaultPipeline();
		virtual ~DefaultPipeline();

		virtual void initImpl() ;
		void execute(const RenderData& renderData,RenderTarget* target);

	
		//void tessellation();
		void vertexProcessing(const RenderElement& elem,VertexVector& verVec);
		void primitiveAssembly(const RenderElement& elem,const VertexVector& verVec);

		//����true��˵��ͨ����false���޳�
		bool culling(const Primitive& prim);
		bool checkPointInScreen(const Vector4& point);
		void generateNewVertex(Vertex& newVertex,const Vertex& vert1, const Vertex& vert2,float dist1, float dist2);
		template<class T>
		inline void Interpolate(T& result, const T& vec1, const T& vec2, float scale);
		void cliping(const Primitive& prim,Primitive prims[5]);
		void clipingLine(const Primitive& prim,Primitive& resultPrim);
		void clipingTriangle(const Primitive& prim,Primitive prims[5]);
		
		void rasterize(Primitive& prim);
		void homogeneousDivide(Vector4& pos);
		void viewportMapping(Vector4& pos,const Viewport* vp);
		
		void geometryProcessing(const RenderElement& elem,const VertexVector& verVec);
		void pixelProcessing(const RenderData::RenderElementList& elems);
		void pixelRendering(const RenderData::RenderElementList& elems);

		void rasterization(RenderTarget* target);

	private:
		Rasterizer mRasterizer;



		VertexList mVertexList;


		
		//�Üpǰ��
		PrimitiveVector mPrimitiveVector;
		
	

		//�Üp���g
		Vector4 mPlane[6];
	};
}
#endif//_DefaultPipeline_H_