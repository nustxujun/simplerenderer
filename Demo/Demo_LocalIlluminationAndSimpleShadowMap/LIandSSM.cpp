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
	FILE* f = fopen("teapot.mesh","rb");
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

}

void App::destroy(Renderer& renderer, const AppParam& param)
{}
void App::renderOneFrame(Renderer& renderer, const AppParam& param) 
{
	//清空framebuffer
	renderer.clearColour(Colour(1,1,1,1));
	renderer.clearDepth(1.0f);

	Material mat;
	mat.diffuse.getFromARGB(0xffffffff);
	mat.specular.getFromARGB(0xffffffff);
	mat.power = 50;
	mat.ambient.getFromARGB(0xff030303);
	renderer.setMaterial(mat);

	Matrix4X4 world;
	static float time = 0;

	world.m[0][0] = cos(time);
	world.m[0][2] = sin(time);

	world.m[2][0] = -sin(time);
	world.m[2][2] = cos(time);
	time += 0.01f;

	renderer.setMatrix(TS_WORLD,world);

	mVS.setMatrix(TS_WORLD,world);
	renderer.setProperty("VertexShader",(VertexShader*)&mVS);
	renderer.setProperty("PixelShader",(PixelShader*)&mPS);


	renderer.setIndexBuffer(mIB);
	renderer.setVertexBuffer(mVB);
	renderer.draw(PT_TRIANGLESTRIP,0,mIB->getIndexCount() / 3 );


}
