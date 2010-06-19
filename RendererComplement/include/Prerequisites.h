#ifndef _Prerequisites_H_
#define _Prerequisites_H_
/*
	�ڲ�ʹ�õ�ͷ�ļ�ϵ��
*/

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) {if(p) delete p;p = NULL;}
#endif

//basal headers
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <assert.h>
#include <list>
#include <set>
#include <math.h>

//basal headers EX
#include "RendererTypes.h"
#include "PixelUtil.h"
//stable classes

namespace RCP
{
	class BackBuffer;
	class Colour;
	class IndexBuffer;
	class IndexBufferManager;
	class Light;
	class Material;
	class Matrix4X4;
	class MemoryBuffer;
	class PaintingMethod;
	class Pipeline;
	class RenderQueue;
	class RenderTarget;
	class Resource;
	class ResourceManager;
	class SharedPtr;
	class Texture;
	class Vector4;
	class VertexBuffer;
	class VertexBufferManager;
	class VertexDeclaration;
}

#endif//_Prerequisites_H_