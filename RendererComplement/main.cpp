
#include "stdio.h"
#include "RendererHeader.h"
using namespace RCP;


float vertexData[] = 
{
	0.0f,0.0f,0.0f,0.0f,1.0f,
	1.0f,0.0f,10.0f,0.0f,1.0f,
	0.0f,1.0f,0.0f,10.0f,-1.0f,		
	1.0f,1.0f,10.0f,10.0f,1.0f,		
};

unsigned int IndexData[]=
{
	0,1,2,
	1,3,2,
};

Matrix4X4 mat;

void main()
{
	mat.m[0][3] = 1;
	
	Renderer r;
	RendererParameters rp;
	rp.backBufferWidth = 640;
	rp.backBufferHeight = 480;
	rp.backBufferPixelFormat = PF_A8R8G8B8;
	r.initialize(rp);

	VertexDeclaration vd;
	vd.addElement(VET_FLOAT2,VES_TEXTURE_COORDINATES);
	vd.addElement(VET_FLOAT3,VES_POSITION);

	

	VertexBuffer* vb = r.createVertexBuffer(4,vd);
	vb->fill(0,4,vertexData);
	IndexBuffer* ib = r.createIndexBuffer();
	ib->addIndex(6,IndexData);

	r.setMatrix(TS_WORLD,mat);
	r.setIndexBuffer(ib);
	r.setVertexBuffer(vb);
	r.draw(PT_TRIANGLESTRIP,0,2);
	r.renderNow();

}