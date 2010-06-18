#ifndef _PixelUtil_H_
#define _PixelUtil_H_

#include "Prerequisites.h"

namespace RCP
{
	class PixelUtil
	{
	public:
		/** Returns the size in bytes of an element of the given pixel format.
		@returns
		The size in bytes of an element. See Remarks.
		@remarks
		Passing PF_UNKNOWN will result in returning a size of 0 bytes.
		*/
		static unsigned int getNumElemBytes( PixelFormat format );

		/** Returns the size in bits of an element of the given pixel format.
		@returns
		The size in bits of an element. See Remarks.
		@remarks
		Passing PF_UNKNOWN will result in returning a size of 0 bits.
		*/
		static size_t getNumElemBits( PixelFormat format );

		/** Returns the size in memory of a region with the given extents and pixel
		format with consecutive memory layout.
		@param width
		The width of the area
		@param height
		The height of the area
		@param depth
		The depth of the area
		@param format
		The format of the area
		@returns
		The size in bytes
		@remarks
		In case that the format is non-compressed, this simply returns
		width*height*depth*PixelUtil::getNumElemBytes(format). In the compressed
		case, this does serious magic.
		*/
		static size_t getMemorySize(size_t width, size_t height, size_t depth, PixelFormat format);

		/** Returns the property flags for this pixel format
		@returns
		A bitfield combination of PFF_HASALPHA, PFF_ISCOMPRESSED,
		PFF_FLOAT, PFF_DEPTH, PFF_NATIVEENDIAN, PFF_LUMINANCE
		@remarks
		This replaces the seperate functions for formatHasAlpha, formatIsFloat, ...
		*/
		static unsigned int getFlags( PixelFormat format );

		/** Shortcut method to determine if the format has an alpha component */
		static bool hasAlpha(PixelFormat format);
		/** Shortcut method to determine if the format is floating point */
		static bool isDepth(PixelFormat format);
		/** Shortcut method to determine if the format is in native endian format. */
		static bool isNativeEndian(PixelFormat format);
		/** Shortcut method to determine if the format is a luminance format. */
		static bool isLuminance(PixelFormat format);

		/** Return wether a certain image extent is valid for this image format.
		@param width
		The width of the area
		@param height
		The height of the area
		@param depth
		The depth of the area
		@param format
		The format of the area
		@remarks For non-compressed formats, this is always true. For DXT formats,
		only sizes with a width and height multiple of 4 and depth 1 are allowed.
		*/
		static bool isValidExtent(size_t width, size_t height, size_t depth, PixelFormat format);

		/** Gives the number of bits (RGBA) for a format. See remarks.          
		@remarks      For non-colour formats (dxt, depth) this returns [0,0,0,0].
		*/
		static void getBitDepths(PixelFormat format, int rgba[4]);

		/** Returns wether the format can be packed or unpacked with the packColour()
		and unpackColour() functions. This is generally not true for compressed and
		depth formats as they are special. It can only be true for formats with a
		fixed element size.
		@returns 
		true if yes, otherwise false
		*/
		static bool isAccessible(PixelFormat srcformat);




	};
}
#endif//_PixelUtil_H_
