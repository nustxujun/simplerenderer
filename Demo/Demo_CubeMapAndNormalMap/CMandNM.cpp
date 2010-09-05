#include "CMandNM.h"

const unsigned int CubeMapSize = 256;

int WINAPI WinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	App app;
	app.start();
	return 0;
}

void App::init(Renderer& renderer, const AppParam& param) 
{

	//ģ�������������д��
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

	//����vb �� ib
	VertexDeclaration vd;
	vd.addElement(VET_FLOAT3,VES_POSITION);
	vd.addElement(VET_FLOAT3,VES_NORMAL);
	mVB = renderer.createVertexBuffer(info.vertexCount,vd);
	mIB = renderer.createIndexBuffer(info.indexCount,IF_INDEX16);

	//д������
	void* data = mVB->lock();
	fread(data,info.vertexSize * info.vertexCount,1,f);
	mVB->unlock();

	data = mIB->lock();
	fread(data,mIB->getSizePerIndex() * info.indexCount,1,f);
	mIB->unlock();
	fclose(f);

	//����Plane
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

	//��ʼ��cubemap
	mCubeMap =renderer.createTexture(CubeMapSize,CubeMapSize,0,TT_CUBE_MAP,PF_A8R8G8B8);
	Vector3 vEyePt   = Vector3( 0.0f, 0.0f, 0.0f );
	Vector3 vLookDir,vUpDir;
	for (int face = 0; face < 6; ++face)
	{
		MatrixUtil::getCubeMapViewMatrix(mOrinViewMatrix[face], face);
	}


	//���õƹ�
	Light l;
	l.diffuse.getFromARGB(0xffffffff);
	l.ambient.getFromARGB(0xffffffff);
	l.specular.getFromARGB(0xffffffff);
	l.position = Vector3(3,3,-3);
	renderer.setLight(0,l);

	mCameraPos = Vector3(3,3,3);
	mLightPos = Vector3(3,3,-3);

	Matrix4X4 view, projection,world,lightView;
	MatrixUtil::getViewSpace(mView,mCameraPos,Vector3(0,0,0),Vector3(0,1,0));
	renderer.setMatrix(TS_VIEW,mView);	

	MatrixUtil::getPerspectiveProjectionSpace(mProjection,3.14159265 / 4,(float)param.width / (float)param.height,0.1,10);
	renderer.setMatrix(TS_PROJECTION,mProjection);
	renderer.setMatrix(TS_WORLD,world);


	//Texture* tex = TextureUtil::loadTextureFromFile(&renderer,"tex1.bmp",0,PF_A8R8G8B8);
	//RenderTarget* rt = tex->getRenderTarget(0);

	//for (int i = 0; i < 6; ++i)
	//{
	//	void *data = mCubeMap->getRenderTarget(0,i)->getData();
	//	memcpy(data,rt->getData(),rt->getSizeInBytes());

	//}

	//RenderTarget* rt = NULL;
	//int i = 0;
	//int c = 0;

	//rt = mCubeMap->getRenderTarget(0,i++);
	//c = 0xffff0000;
	//for (int j = 0; j < 256 * 256; ++j)
	//	rt->write(&c,4);

	//rt = mCubeMap->getRenderTarget(0,i++);
	//c = 0xff00ff00;
	//for (int j = 0; j < 256 * 256; ++j)
	//	rt->write(&c,4);

	//rt = mCubeMap->getRenderTarget(0,i++);
	//c = 0xff0000ff;
	//for (int j = 0; j < 256 * 256; ++j)
	//	rt->write(&c,4);

	//rt = mCubeMap->getRenderTarget(0,i++);
	//c = 0xffffff00;
	//for (int j = 0; j < 256 * 256; ++j)
	//	rt->write(&c,4);

	//rt = mCubeMap->getRenderTarget(0,i++);
	//c = 0xffff00ff;
	//for (int j = 0; j < 256 * 256; ++j)
	//	rt->write(&c,4);
	

	
}

void App::destroy(Renderer& renderer, const AppParam& param)
{
	mVB->Release();
	mIB->Release();
	mPlaneVB->Release();
}

void App::renderObject(Renderer& renderer)
{
	
		renderer.clearColour(0.5f);
		renderer.clearDepth(1.0f);

	Material mat;
	Matrix4X4 world;
	mat.diffuse.getFromARGB(0xffffffff);
	mat.specular.getFromARGB(0xffffffff);
	mat.power = 50;
	mat.ambient.getFromARGB(0xff030303);
	renderer.setMaterial(mat);
	renderer.setMatrix(TS_WORLD,world);
	renderer.setVertexBuffer(mPlaneVB);
	renderer.draw(PT_TRIANGLESTRIP,0,2 );


	
	world.m[0][3] = -1.5;
	world.m[2][3] = 2;
	renderer.setMatrix(TS_WORLD,world);
	renderer.setMaterial(mat);
	renderer.setIndexBuffer(mIB);
	renderer.setVertexBuffer(mVB);
	renderer.draw(PT_TRIANGLESTRIP,0,mIB->getIndexCount() / 3 );
}

void App::renderCubeMap(Renderer& renderer)
{
	Matrix4X4 projection;
	MatrixUtil::getPerspectiveProjectionSpace(projection,3.14159265 * 0.5f,1,0.1,10);
	renderer.setMatrix(TS_PROJECTION,projection);
	Matrix4X4 viewDir = mView;
	viewDir.m[0][3] = viewDir.m[1][3] = viewDir.m[2][3] = 0.0f;

	
	RenderTarget* backbuffer = renderer.getBackBuffer();

	RenderTarget* rt;
	Matrix4X4 view;
	for (int i = 0; i < 6 ;++i)
	{
		rt = mCubeMap->getRenderTarget(0,i);
		renderer.setRenderTarget(0,rt);
		view =  mOrinViewMatrix[i] * viewDir ;
		renderer.setMatrix(TS_VIEW,view);
		renderObject(renderer);
		
	}

	renderer.setRenderTarget(0,backbuffer);
}

void App::renderOneFrame(Renderer& renderer, const AppParam& param) 
{

	renderer.setTexture(0,NULL);
	renderCubeMap(renderer);

	renderer.setMatrix(TS_VIEW,mView);
	renderer.setMatrix(TS_PROJECTION,mProjection);	
	renderObject(renderer);

		//renderer.clearColour(1.0f);
		//renderer.clearDepth(1.0f);
	renderer.setProperty("VertexShader",(VertexShader*)&mVS);
	renderer.setProperty("PixelShader",(PixelShader*)&mPS);
	renderer.setProperty("Matrix1",mView);
	renderer.setProperty("Matrix2",mProjection);
	renderer.setTexture(0,mCubeMap);
	TextureState ts;
	ts.addresingModeU = TAM_MIRROR;
	ts.addresingModeV = TAM_MIRROR;
	renderer.setTextureState(0,ts);
	renderer.setIndexBuffer(mIB);
	renderer.setVertexBuffer(mVB);
	renderer.draw(PT_TRIANGLESTRIP,0,mIB->getIndexCount() / 3 );

	renderer.setProperty("VertexShader",(VertexShader*)NULL);
	renderer.setProperty("PixelShader",(PixelShader*)NULL);

}