#include "LIandSSM.h"


int WINAPI WinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	App app;
	app.start();
	return 0;
}

void App::init(Renderer& renderer, const AppParam& param) 
{

	//模型数据事先随便写的
	FILE* f = fopen("../Reasource/Model/teapot.mesh","rb");
	assert(f);
	struct MeshInfo
	{
		int vertexSize;
		int vertexCount;
		int indexCount;
	};
	MeshInfo info;
	fread(&info,sizeof(MeshInfo),1,f);

	//创建vb 和 ib
	VertexDeclaration vd;
	vd.addElement(VET_FLOAT3,VES_POSITION);
	vd.addElement(VET_FLOAT3,VES_NORMAL);
	mVB = renderer.createVertexBuffer(info.vertexCount,vd);
	mIB = renderer.createIndexBuffer(info.indexCount,IF_INDEX16);

	//写入数据
	void* data = mVB->lock();
	fread(data,info.vertexSize * info.vertexCount,1,f);
	mVB->unlock();

	data = mIB->lock();
	fread(data,mIB->getSizePerIndex() * info.indexCount,1,f);
	mIB->unlock();
	fclose(f);

	//创建Plane
	struct vertexFormat
	{
		float x,y,z;
		int color;
		float u,v,w;
	};
	vertexFormat vertexData[] = 
	{
		-10.0f,-1.0f,-10.0f,0xffffffff,0.0f,1.0f,0.0f,
		-10.0f,-1.0f,10.0f,0xffffffff,0.0f,0.0f,0.0f,
		10.0f,-1.0f,-10.0f,0xffffffff,0.0f,1.0f,0.0f,

		10.0f,-1.0f,-10.0f,0xffffffff,0.0f,1.0f,0.0f,
		-10.0f,-1.0f,10.0f,0xffffffff,0.0f,0.0f,0.0f,
		10.0f,-1.0f,10.0f,0xffffffff,0.0f,0.0f,0.0f,
	};
	VertexDeclaration vd2;
	vd2.addElement(VET_FLOAT3,VES_POSITION);
	vd2.addElement(VET_COLOUR,VES_DIFFUSE);
	vd2.addElement(VET_FLOAT3,VES_NORMAL);
	mPlaneVB = renderer.createVertexBuffer(6,vd2);
	mPlaneVB->fill(0,6,vertexData);

	//创建sm
	const RenderTarget* bb = renderer.getBackBuffer();
	mShadowMap = renderer.createTexture(bb->getWidth(),
		bb->getHeight(),0,TT_2D,PF_A8R8G8B8);


	//设置灯光
	Light l;
	l.diffuse.getFromARGB(0xffffffff);
	l.ambient.getFromARGB(0xffffffff);
	l.specular.getFromARGB(0xffffffff);
	l.position = Vector3(3,3,-3);
	renderer.setLight(0,l);

	mCameraPos = Vector3(3,3,3);
	mLightPos = Vector3(3,3,-3);

	Matrix4X4 view, projection,world,lightView;
	MatrixUtil::getViewSpace(view,mCameraPos,Vector3(0,0,0),Vector3(0,1,0));
	renderer.setMatrix(TS_VIEW,view);	

	MatrixUtil::getPerspectiveProjectionSpace(projection,3.14159265 / 4,(float)param.width / (float)param.height,0.1,10);
	renderer.setMatrix(TS_PROJECTION,projection);
	renderer.setMatrix(TS_WORLD,world);


	MatrixUtil::getViewSpace(lightView,mLightPos,Vector3(0,0,0),Vector3(0,1,0));
	mSMMakerVS.setMatrix(TS_VIEW,lightView);
	mSMMakerVS.setMatrix(TS_PROJECTION,projection);

	mVS.setMatrix(TS_VIEW,view);
	mVS.setMatrix(TS_PROJECTION,projection);
	mVS.setMatrix(TS_WORLD2,lightView);

	mPS.cameraPos = mCameraPos;
	mPS.lightPos = mLightPos;
	mPS.power = 20;
	mPS.width = param.width;
	mPS.height = param.height;



}

void App::destroy(Renderer& renderer, const AppParam& param)
{
	mVB->release();
	mIB->release();
	mShadowMap->release();
	mPlaneVB->release();
}
void App::renderOneFrame(Renderer& renderer, const AppParam& param) 
{


	//Material mat;
	//mat.diffuse.getFromARGB(0xffffffff);
	//mat.specular.getFromARGB(0xffffffff);
	//mat.power = 50;
	//mat.ambient.getFromARGB(0xff030303);
	//renderer.setMaterial(mat);

	Matrix4X4 world;
	static float time = 0;

	world.m[0][0] = cos(0.01);
	world.m[0][2] = sin(0.01);
	world.m[2][0] = -sin(0.01);
	world.m[2][2] = cos(0.01);
	time += 0.01f;

	renderer.setMatrix(TS_WORLD,world);

	mLightPos = world * mLightPos;
	Matrix4X4 lightView;
	MatrixUtil::getViewSpace(lightView,mLightPos,Vector3(0,0,0),Vector3(0,1,0));
	mSMMakerVS.setMatrix(TS_VIEW,lightView);
	mVS.setMatrix(TS_WORLD2,lightView);
	mPS.lightPos = mLightPos;


	RenderTarget* backBuffer = renderer.getBackBuffer();

	renderer.setRenderTarget(0,mShadowMap->getRenderTarget(0));
	union Value
	{
		float f;
		int i;
	};
	Value v;
	v.f = 1.0f;
	renderer.clearColour(Colour().getFromARGB(v.i));
	renderer.clearDepth(1.0f);
	mSMMakerVS.setMatrix(TS_WORLD,Matrix4X4());
	renderer.setProperty("VertexShader",(VertexShader*)&mSMMakerVS);
	renderer.setProperty("PixelShader",(PixelShader*)&mSMMakerPS);
	renderer.setIndexBuffer(mIB);
	renderer.setVertexBuffer(mVB);
	renderer.draw(PT_TRIANGLESTRIP,0,mIB->getIndexCount() / 3 );

	renderer.setRenderTarget(0,backBuffer);
	renderer.clearColour(Colour(1,1,1,1));
	renderer.clearDepth(1.0f);
	renderer.setTexture(0,mShadowMap);
	mVS.setMatrix(TS_WORLD,Matrix4X4());
	renderer.setProperty("VertexShader",(VertexShader*)&mVS);
	renderer.setProperty("PixelShader",(PixelShader*)&mPS);


	renderer.setVertexBuffer(mPlaneVB);
	renderer.draw(PT_TRIANGLESTRIP,0,2 );

	renderer.setIndexBuffer(mIB);
	renderer.setVertexBuffer(mVB);
	renderer.draw(PT_TRIANGLESTRIP,0,mIB->getIndexCount() / 3 );


}
