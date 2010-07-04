#ifndef _RendererTypes_H_
#define _RendererTypes_H_

/*
	各种类型
*/


namespace RCP
{

	enum PixelFormat
	{
		/// Unknown pixel format.
		PF_UNKNOWN = 0,
		/// 8-bit pixel format, all bits luminace.
		PF_R5G6B5 ,
		/// 16-bit pixel format, 5 bits red, 6 bits green, 5 bits blue.
		PF_B5G6R5 ,
		/// 8-bit pixel format, 2 bits blue, 3 bits green, 3 bits red.
		PF_R3G3B2 ,
		/// 16-bit pixel format, 4 bits for alpha, red, green and blue.
		PF_A4R4G4B4 ,
		/// 16-bit pixel format, 5 bits for blue, green, red and 1 for alpha.
		PF_A1R5G5B5 ,
		/// 24-bit pixel format, 8 bits for red, green and blue.
		PF_R8G8B8 ,
		/// 24-bit pixel format, 8 bits for blue, green and red.
		PF_B8G8R8 ,
		/// 32-bit pixel format, 8 bits for alpha, red, green and blue.
		PF_A8R8G8B8 ,
		/// 32-bit pixel format, 8 bits for blue, green, red and alpha.
		PF_A8B8G8R8 ,
		/// 32-bit pixel format, 8 bits for blue, green, red and alpha.
		PF_B8G8R8A8 ,
		/// 32-bit pixel format, 8 bits for red, green, blue and alpha.
		PF_R8G8B8A8 ,
		/// 32-bit pixel format, 8 bits for red, 8 bits for green, 8 bits for blue
		/// like PF_A8R8G8B8, but alpha will get discarded
		PF_X8R8G8B8 ,
		/// 32-bit pixel format, 8 bits for blue, 8 bits for green, 8 bits for red
		/// like PF_A8B8G8R8, but alpha will get discarded
		PF_X8B8G8R8,
		/// 32-bit pixel format, 2 bits for alpha, 10 bits for red, green and blue.
		PF_A2R10G10B10 ,
		/// 32-bit pixel format, 10 bits for blue, green and red, 2 bits for alpha.
		PF_A2B10G10R10 ,
		/// DDS (DirectDraw Surface) DXT1 format
		PF_DXT1  ,
		/// DDS (DirectDraw Surface) DXT2 format
		PF_DXT2  ,
		/// DDS (DirectDraw Surface) DXT3 format
		PF_DXT3  ,
		/// DDS (DirectDraw Surface) DXT4 format
		PF_DXT4  ,
		/// DDS (DirectDraw Surface) DXT5 format
		PF_DXT5  ,
		// 16-bit pixel format, 16 bits (float) for red
		// Number of pixel formats currently defined
		PF_COUNT 
	};

    enum PixelFormatFlags {
        // This format has an alpha channel
        PFF_HASALPHA        = 0x00000001,      
        // This format is compressed. This invalidates the values in elemBytes,
        // elemBits and the bit counts as these might not be fixed in a compressed format.
        PFF_COMPRESSED    = 0x00000002,
        // This is a floating point format
        PFF_FLOAT           = 0x00000004,         
        // This is a depth format (for depth textures)
        PFF_DEPTH           = 0x00000008,
        // Format is in native endian. Generally true for the 16, 24 and 32 bits
        // formats which can be represented as machine integers.
        PFF_NATIVEENDIAN    = 0x00000010,
        // This is an intensity format instead of a RGB one. The luminance
        // replaces R,G and B. (but not A)
        PFF_LUMINANCE       = 0x00000020
    };

	//渲染器创建参数
	struct RendererParameters
	{
		unsigned int backBufferWidth;
		unsigned int backBufferHeight;
		PixelFormat backBufferPixelFormat;
	};

	enum CompareFunc
	{
		CF_NEVER,
		CF_LESS,
		CF_LESSEQUAL,
		CF_EQUAL,
		CF_GREATER,
		CF_GREATEREQUAL,
		CF_NOTEQUAL,
		CF_ALWAYS,
	};

	enum StencilOperation
	{
		SO_KEEP,
		SO_ZERO,
		SO_REPLACE,
		SO_INCRSAT ,
		SO_DECRSAT ,
		SOP_INVERT ,
		SO_INCR ,
		SO_DECR ,

	};

	struct RenderState
	{
		//z test
		bool zWriteEnable;
		bool zTestEnable;

		//alpha test
		float alphaTestRef;
		CompareFunc alphaTestFunc;
		
		//stencil test
		CompareFunc stencilTestFunc;
		unsigned int stencilRef;
		unsigned int stencilMask;
		unsigned int stencilWriteMask;
		StencilOperation stencilFail;
		StencilOperation stencilPass;
		StencilOperation stencilZFail;

		//alpha blend
		bool alphaBlendEnable;


	};

	
	//图元类型
	enum Primitives
	{
		PT_POINTLIST             = 1,
		PT_LINELIST              = 2,
		PT_LINESTRIP             = 3,
		PT_TRIANGLELIST          = 4,
		PT_TRIANGLESTRIP         = 5,
		PT_TRIANGLEFAN           = 6,
	};

	enum TransformStateType
	{
		TS_VIEW,
		TS_PROJECTION,
		TS_WORLD,
		TS_WORLD2,
		TS_WORLD3,
		TS_WORLD4,
		TS_BASALNUM,
		TS_FORCE_DWORD     = 0x7fffffff, /* 强行扩充，可能会用无限权重数的顶点混合 */
	};

	enum VertexElementSemantic {
		VES_POSITION = 1,
		VES_BLEND_WEIGHTS = 2,
        VES_BLEND_INDICES = 3,
		VES_NORMAL = 4,
		VES_DIFFUSE = 5,
		VES_SPECULAR = 6,
		VES_TEXTURE_COORDINATES = 7,
        VES_BINORMAL = 8,
        VES_TANGENT = 9
	};
		
	enum VertexElementType
    {
        VET_FLOAT1 = 0,
        VET_FLOAT2 = 1,
        VET_FLOAT3 = 2,
        VET_FLOAT4 = 3,
        /// alias to more specific colour type - use the current rendersystem's colour packing
		VET_COLOUR = 4,
		VET_SHORT1 = 5,
		VET_SHORT2 = 6,
		VET_SHORT3 = 7,
		VET_SHORT4 = 8,
        VET_UBYTE4 = 9,
        /// D3D style compact colour
        VET_COLOUR_ARGB = 10,
        /// GL style compact colour
        VET_COLOUR_ABGR = 11,
    };

	enum TextureAddresingMode
	{
		TAM_WRAP,
		TAM_MIRROR,
		TAM_CLAMP,
		TAM_BORDER,
		TAM_MIRRORONCE,
	};

	struct TextureState
	{
		TextureAddresingMode addresingModeU;
		TextureAddresingMode addresingModeV;

		TextureState():
			addresingModeU(TAM_WRAP),
			addresingModeV(TAM_WRAP)

		{}
	};

	//use in framebfuffer
	enum BufferTpye
	{
		BT_COLOUR,
		BT_DEPTH,
		BT_STENCIL,

		BT_COUNT
	};





}


#endif//_RendererTypes_H_