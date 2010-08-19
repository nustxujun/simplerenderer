#include "Beginning.h"

int WINAPI WinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	App app;
	app.start();
	return 0;
}

void App::init(Renderer& renderer, const AppParam& param) 
{
	Matrix4X4 view, projection,world;
	MatrixUtil::getViewSpace(view,Vector3(0,0,-3),Vector3(0,0,0),Vector3(0,1,0));
	renderer.setMatrix(TS_VIEW,view);	

	MatrixUtil::getPerspectiveProjectionSpace(projection,3.14159265 / 4,param.width / param.height,0.1,10);
	renderer.setMatrix(TS_PROJECTION,projection);	


	renderer.setMatrix(TS_WORLD,world);

	VertexDeclaration vd2;
	vd2.addElement(VET_FLOAT3,VES_POSITION);
	vd2.addElement(VET_COLOUR,VES_DIFFUSE);
	vd2.addElement(VET_FLOAT2,VES_TEXTURE_COORDINATES);

	struct vertexFormat
	{
		float x,y,z;
		int color;
		float u,v;
	};
	vertexFormat vertexData[] = 
	{
		-1.0f,-1.0f,0.0f,0x50ff0000,1.0f,1.0f,
		-1.0f,1.0f,0.0f,0x5000ff00,1.0f,0.0f,
		1.0f,-1.0f,0.f,0x500000ff,0.0f,1.0f,

		1.0f,-1.0f,0.f,0x50ff00ff,0.0f,1.0f,
		-1.0f,1.0f,0.0f,0x5000ff00,1.0f,0.0f,
		1.0f,1.0f,0.0f,0x500000ff,0.0f,0.0f,
	};

	mVB = renderer.createVertexBuffer(6,vd2);
	mVB->fill(0,6,vertexData);

}

void App::destroy(Renderer& renderer, const AppParam& param)
{}
void App::renderOneFrame(Renderer& renderer, const AppParam& param) 
{
		renderer.clearColour(Colour(1,1,1));
		renderer.clearDepth(1.0f);
		Matrix4X4 world;
		static float i = 0;
		i += 0.1f;
		float r  = sin(i);
		world = world * r;
		world.m[3][3] = 1;
		renderer.setMatrix(RCP::TS_WORLD,world);


		renderer.clearColour(Colour(1));
		renderer.setVertexBuffer(mVB);
		renderer.draw(PT_TRIANGLESTRIP,0,2);
}
