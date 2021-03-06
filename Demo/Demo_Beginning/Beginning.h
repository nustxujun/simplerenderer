#include "..\QuickStart\QuickStart.h"
#include <windows.h>
#include "..\..\RendererComplement\include\RendererHeader.h"
#pragma once;
using namespace RCP;
class App:public QSApplication
{
	virtual void init(Renderer& renderer, const AppParam& param) ;
	virtual void reset(Renderer& renderer, const AppParam& param) {}
	virtual void destroy(Renderer& renderer, const AppParam& param);
	virtual void renderOneFrame(Renderer& renderer, const AppParam& param) ;

	VertexBuffer* mVB;

};