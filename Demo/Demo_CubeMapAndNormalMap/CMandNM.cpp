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

	//模型数据事先随便写的
	//FILE* f = fopen("../Reasource/Model/teapot.mesh","rb");
	FILE* f = fopen("../Reasource/Model/tiger.mesh","rb");
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
	//vd.addElement(VET_FLOAT3,VES_NORMAL);
	vd.addElement(VET_FLOAT2,VES_TEXTURE_COORDINATES);

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
		//float u,v,w;
		//float i,j,k;
		//float a,b,c;
		float m,n;
	};
	vertexFormat vertexData[] = 
	{
		-10.0f,-1.0f,-10.0f,0xffffffff,/*0.0f,1.0f,0.0f, 1.0f,0.0f,0.0f, 0.0f,0.0f,1.0f,*/ 0.0f,0.0f,
		-10.0f,-1.0f,10.0f,0xffffffff,/*0.0f,1.0f,0.0f, 1.0f,0.0f,0.0f, 0.0f,0.0f,1.0f,*/ 0.0f,1.0f,
		10.0f,-1.0f,-10.0f,0xffffffff,/*0.0f,1.0f,0.0f, 1.0f,0.0f,0.0f, 0.0f,0.0f,1.0f,*/ 1.0f,0.0f,

		10.0f,-1.0f,-10.0f,0xffffffff,/*0.0f,1.0f,0.0f, 1.0f,0.0f,0.0f, 0.0f,0.0f,1.0f,*/ 1.0f,0.0f,
		-10.0f,-1.0f,10.0f,0xffffffff,/*0.0f,1.0f,0.0f, 1.0f,0.0f,0.0f, 0.0f,0.0f,1.0f,*/ 0.0f,1.0f,
		10.0f,-1.0f,10.0f,0xffffffff,/*0.0f,1.0f,0.0f, 1.0f,0.0f,0.0f, 0.0f,0.0f,1.0f,*/ 1.0f,1.0f,
	};
	VertexDeclaration vd2;
	vd2.addElement(VET_FLOAT3,VES_POSITION);
	vd2.addElement(VET_COLOUR,VES_DIFFUSE);
	//vd2.addElement(VET_FLOAT3,VES_NORMAL);
	//vd2.addElement(VET_FLOAT3,VES_TANGENT);
	//vd2.addElement(VET_FLOAT3,VES_BINORMAL);
	vd2.addElement(VET_FLOAT2,VES_TEXTURE_COORDINATES);
	mPlaneVB = renderer.createVertexBuffer(6,vd2);
	mPlaneVB->fill(0,6,vertexData);

	
	VertexBuffer* tvb = TangentUtil::calculateTangentSpace(&renderer,mVB,mIB);
	mVB->release();
	mVB = tvb;
	tvb = TangentUtil::calculateTangentSpace(&renderer,mPlaneVB,NULL);
	mPlaneVB->release();
	mPlaneVB = tvb;


	//初始化cubemap
	mCubeMap =renderer.createTexture(CubeMapSize,CubeMapSize,0,TT_CUBE_MAP,PF_A8R8G8B8);
	Vector3 vEyePt   = Vector3( 0.0f, 0.0f, 0.0f );
	Vector3 vLookDir,vUpDir;
	for (int face = 0; face < 6; ++face)
	{
		MatrixUtil::getCubeMapViewMatrix(mOrinViewMatrix[face], face);
	}

	mNormalMap = TextureUtil::loadTextureFromFile(&renderer,"../Reasource/Texture/normalmap.bmp",0,PF_A8R8G8B8);
	//mNormalMap = renderer.createTexture(CubeMapSize,CubeMapSize,0,TT_CUBE_MAP,PF_A8R8G8B8);
	//RenderTarget* rt = temp->getRenderTarget(0);
	//for(int i = 0; i<6;++i)
	//{
	//	mNormalMap->getRenderTarget(0,i)->write(rt->getData(),rt->getSizeInBytes());
	//}
	//mNormalMap = temp;

	mCameraPos = Vector3(3,3,-3);
	mLightPos = Vector3(3,3,-3);



	//设置灯光
	Light l;
	l.diffuse.getFromARGB(0xffffffff);
	l.ambient.getFromARGB(0xffffffff);
	l.specular.getFromARGB(0xffffffff);
	l.position = mLightPos;
	renderer.setLight(0,l);

	Matrix4X4 view, projection,world,lightView;
	MatrixUtil::getViewSpace(mView,mCameraPos,Vector3(0,0,0),Vector3(0,1,0));
	renderer.setMatrix(TS_VIEW,mView);	

	MatrixUtil::getPerspectiveProjectionSpace(mProjection,3.14159265 / 4,(float)param.width / (float)param.height,0.1,10);
	renderer.setMatrix(TS_PROJECTION,mProjection);
	renderer.setMatrix(TS_WORLD,world);
	
}

void App::destroy(Renderer& renderer, const AppParam& param)
{
	mVB->release();
	mIB->release();
	mPlaneVB->release();
}

void App::renderObject(Renderer& renderer)
{


	Material mat;
	Matrix4X4 world;
	mat.diffuse.getFromARGB(0xffffffff);
	mat.specular.getFromARGB(0xffffffff);
	mat.power = 100;
	mat.ambient.getFromARGB(0xff030303);
	renderer.setMaterial(mat);
	renderer.setMatrix(TS_WORLD,world);
	renderer.setVertexBuffer(mPlaneVB);
	renderer.draw(PT_TRIANGLESTRIP,0,2 );


	
	world.m[0][3] = 1.5;
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
			
		renderer.clearColour(1.0f);
		renderer.clearDepth(1.0f);
		renderObject(renderer);
		
	}

	renderer.setRenderTarget(0,backbuffer);
}

void App::renderOneFrame(Renderer& renderer, const AppParam& param) 
{

	renderer.setTexture(0,NULL);
	renderer.setTexture(1,NULL);
	renderCubeMap(renderer);

	renderer.setMatrix(TS_VIEW,mView);
	renderer.setMatrix(TS_PROJECTION,mProjection);	
		
	renderer.clearColour(1.0f);
	renderer.clearDepth(1.0f);
	renderObject(renderer);

	renderer.setProperty("VertexShader",(VertexShader*)&mVS);
	renderer.setProperty("PixelShader",(PixelShader*)&mPS);
	renderer.setProperty("Matrix1",mView);
	renderer.setProperty("Matrix2",mProjection);
	renderer.setProperty("Vector0",Vector4(mCameraPos,1));
	renderer.setProperty("Vector1",Vector4(mLightPos,1));
	renderer.setTexture(0,mCubeMap);
	renderer.setTexture(1,mNormalMap);
	renderer.setIndexBuffer(mIB);
	renderer.setVertexBuffer(mVB);
	renderer.draw(PT_TRIANGLESTRIP,0,mIB->getIndexCount() / 3 );

	renderer.setProperty("VertexShader",(VertexShader*)NULL);
	renderer.setProperty("PixelShader",(PixelShader*)NULL);
	renderer.setTexture(0,NULL);
	renderer.setTexture(1,NULL);

}
