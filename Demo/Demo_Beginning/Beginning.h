#include "..\QuickStart\QuickStart.h"
#include <windows.h>
#include "..\..\RendererComplement\include\RendererHeader.h"
using namespace RCP;
class App:public QSApplication
{
	virtual void init(Renderer& renderer) ;
	virtual void reset(Renderer& renderer) {}
	virtual void destroy(Renderer& renderer);
	virtual void renderOneFrame(Renderer& renderer) ;

	VertexBuffer* mVB;

};