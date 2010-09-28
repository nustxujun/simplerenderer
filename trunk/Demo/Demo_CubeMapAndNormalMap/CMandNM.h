#pragma once;
#include "..\QuickStart\QuickStart.h"
#include <windows.h>
#include "..\..\RendererComplement\include\RendererHeader.h"
#include "Shader.h"

using namespace RCP;
class App:public QSApplication
{
	virtual void init(Renderer& renderer, const AppParam& param) ;
	virtual void reset(Renderer& renderer, const AppParam& param) {}
	virtual void destroy(Renderer& renderer, const AppParam& param);
	virtual void renderOneFrame(Renderer& renderer, const AppParam& param) ;

	void renderCubeMap(Renderer& renderer);
	void renderObject(Renderer& renderer);

	VertexBuffer* mVB;
	IndexBuffer* mIB;
	Texture* mNormalMap;
	Texture* mCubeMap;
	VertexBuffer* mPlaneVB;

	Vector3 mLightPos;
	Vector3 mCameraPos;

	Matrix4X4 mOrinViewMatrix[6];

	Matrix4X4 mView;
	Matrix4X4 mProjection;

	VS mVS;
	PS mPS;

	NMVS mNMVS;
	NMPS mNMPS;
};