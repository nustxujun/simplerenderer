#ifndef _Prerequisites_H_
#define _Prerequisites_H_
/*
	内部使用的头文件系列
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
#include "Any.h"
#include "FastAlgorithm.h"
#include "RendererTypes.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Vector2.h"
#include "PixelUtil.h"

//stable classes

namespace RCP
{
	class Any;
	class BackBuffer;
	class Colour;
	class FrameBuffer;
	class IndexBuffer;
	class IndexBufferManager;
	class Light;
	class Material;
	class Matrix4X4;
	class MemoryBuffer;
	class PaintingMethod;
	class Pipeline;
	class Renderer;
	class RenderQueue;
	class RenderTarget;
	class Resource;
	class ResourceManager;
	class Sampler;
	class SharedPtr;
	class Texture;
	class TextureManager;
	class Vector3;
	class Vector4;
	class VertexBuffer;
	class VertexBufferManager;
	class VertexDeclaration;
	class Viewport;
}

#endif//_Prerequisites_H_