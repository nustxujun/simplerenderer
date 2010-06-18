#include "PixelUtil.h"

namespace RCP
{
	struct PixelFormatDescription {
		/* Name of the format, as in the enum */
		char *name;
		/* Number of bytes one element (colour value) takes. */
		unsigned char elemBytes;
		/* Pixel format flags, see enum PixelFormatFlags for the bit field
		* definitions
		*/
		unsigned int flags;

		/* Number of bits for red(or luminance), green, blue, alpha
		*/
		unsigned char rbits,gbits,bbits,abits; /*, ibits, dbits, ... */

	};
	//-----------------------------------------------------------------------
	/** Pixel format database */
	PixelFormatDescription _pixelFormats[PF_COUNT] = {
		//-----------------------------------------------------------------------
		{"PF_UNKNOWN",
		/* Bytes per element */
		0,
		/* Flags */
		0,
		/* rbits, gbits, bbits, abits */
		0, 0, 0, 0,
		},
		//-----------------------------------------------------------------------
		{"PF_R5G6B5",
		/* Bytes per element */
		2,
		/* Flags */
		PFF_NATIVEENDIAN,
		/* rbits, gbits, bbits, abits */
		5, 6, 5, 0,
		},
		//-----------------------------------------------------------------------
		{"PF_B5G6R5",
		/* Bytes per element */
		2,
		/* Flags */
		PFF_NATIVEENDIAN,
		/* rbits, gbits, bbits, abits */
		5, 6, 5, 0,
		},
		
		//-----------------------------------------------------------------------
		{"PF_R3G3B2",
		/* Bytes per element */
		1,
		/* Flags */
		PFF_NATIVEENDIAN,
		/* rbits, gbits, bbits, abits */
		3, 3, 2, 0,
		},
		//-----------------------------------------------------------------------
		{"PF_A4R4G4B4",
		/* Bytes per element */
		2,
		/* Flags */
		PFF_HASALPHA | PFF_NATIVEENDIAN,
		/* rbits, gbits, bbits, abits */
		4, 4, 4, 4,
		},
		//-----------------------------------------------------------------------
		{"PF_A1R5G5B5",
		/* Bytes per element */
		2,
		/* Flags */
		PFF_HASALPHA | PFF_NATIVEENDIAN,
		/* rbits, gbits, bbits, abits */
		5, 5, 5, 1,
		},
		//-----------------------------------------------------------------------
		{"PF_R8G8B8",
		/* Bytes per element */
		3,  // 24 bit integer -- special
		/* Flags */
		PFF_NATIVEENDIAN,
		/* rbits, gbits, bbits, abits */
		8, 8, 8, 0,
		},
		//-----------------------------------------------------------------------
		{"PF_B8G8R8",
		/* Bytes per element */
		3,  // 24 bit integer -- special
		/* Flags */
		PFF_NATIVEENDIAN,
		/* rbits, gbits, bbits, abits */
		8, 8, 8, 0,
		},
		//-----------------------------------------------------------------------
		{"PF_A8R8G8B8",
		/* Bytes per element */
		4,
		/* Flags */
		PFF_HASALPHA | PFF_NATIVEENDIAN,
		/* rbits, gbits, bbits, abits */
		8, 8, 8, 8,
		},
		//-----------------------------------------------------------------------
		{"PF_A8B8G8R8",
		/* Bytes per element */
		4,
		/* Flags */
		PFF_HASALPHA | PFF_NATIVEENDIAN,
		/* rbits, gbits, bbits, abits */
		8, 8, 8, 8,
		},
		//-----------------------------------------------------------------------
		{"PF_B8G8R8A8",
		/* Bytes per element */
		4,
		/* Flags */
		PFF_HASALPHA | PFF_NATIVEENDIAN,
		/* rbits, gbits, bbits, abits */
		8, 8, 8, 8,
		},
		//-----------------------------------------------------------------------
		{"PF_R8G8B8A8",
		/* Bytes per element */
		4,
		/* Flags */
		PFF_HASALPHA | PFF_NATIVEENDIAN,
		/* rbits, gbits, bbits, abits */
		8, 8, 8, 8,
		},
		//-----------------------------------------------------------------------
		{"PF_X8R8G8B8",
		/* Bytes per element */
		4,
		/* Flags */
		PFF_NATIVEENDIAN,
		/* rbits, gbits, bbits, abits */
		8, 8, 8, 0,
		},
		//-----------------------------------------------------------------------
		{"PF_X8B8G8R8",
		/* Bytes per element */
		4,
		/* Flags */
		PFF_NATIVEENDIAN,
		/* rbits, gbits, bbits, abits */
		8, 8, 8, 0,
		},
		//-----------------------------------------------------------------------
		{"PF_A2R10G10B10",
		/* Bytes per element */
		4,
		/* Flags */
		PFF_HASALPHA | PFF_NATIVEENDIAN,
		/* rbits, gbits, bbits, abits */
		10, 10, 10, 2,
		},
		//-----------------------------------------------------------------------
		{"PF_A2B10G10R10",
		/* Bytes per element */
		4,
		/* Flags */
		PFF_HASALPHA | PFF_NATIVEENDIAN,
		/* rbits, gbits, bbits, abits */
		10, 10, 10, 2,
		},
		//-----------------------------------------------------------------------
		{"PF_DXT1",
		/* Bytes per element */
		0,
		/* Flags */
		PFF_COMPRESSED | PFF_HASALPHA,
		/* rbits, gbits, bbits, abits */
		0, 0, 0, 0,
		},
		//-----------------------------------------------------------------------
		{"PF_DXT2",
		/* Bytes per element */
		0,
		/* Flags */
		PFF_COMPRESSED | PFF_HASALPHA,
		/* rbits, gbits, bbits, abits */
		0, 0, 0, 0,
		},
		//-----------------------------------------------------------------------
		{"PF_DXT3",
		/* Bytes per element */
		0,
		/* Flags */
		PFF_COMPRESSED | PFF_HASALPHA,
		/* rbits, gbits, bbits, abits */
		0, 0, 0, 0,
		},
		//-----------------------------------------------------------------------
		{"PF_DXT4",
		/* Bytes per element */
		0,
		/* Flags */
		PFF_COMPRESSED | PFF_HASALPHA,
		/* rbits, gbits, bbits, abits */
		0, 0, 0, 0,
		},
		//-----------------------------------------------------------------------
		{"PF_DXT5",
		/* Bytes per element */
		0,
		/* Flags */
		PFF_COMPRESSED | PFF_HASALPHA,
		/* rbits, gbits, bbits, abits */
		0, 0, 0, 0,
		},

	};

	static inline const PixelFormatDescription &getDescriptionFor(const PixelFormat fmt)
	{
		const int ord = (int)fmt;
		assert(ord>=0 && ord<PF_COUNT);

		return _pixelFormats[ord];
	}
	//-----------------------------------------------------------------------
	unsigned int PixelUtil::getNumElemBytes( PixelFormat format )
	{
		return getDescriptionFor(format).elemBytes;
	}
	//-----------------------------------------------------------------------
	size_t PixelUtil::getMemorySize(size_t width, size_t height, size_t depth, PixelFormat format)
	{
		if((getFlags(format) & PFF_COMPRESSED) != 0)
		{
			switch(format)
			{
				// DXT formats work by dividing the image into 4x4 blocks, then encoding each
				// 4x4 block with a certain number of bytes. DXT can only be used on 2D images.
			case PF_DXT1:
				assert(depth == 1);
				return ((width+3)/4)*((height+3)/4)*8;
			case PF_DXT2:
			case PF_DXT3:
			case PF_DXT4:
			case PF_DXT5:
				assert(depth == 1);
				return ((width+3)/4)*((height+3)/4)*16;
			default:
				assert(0);
				return 0;
			}
		}
		else
		{
			return width*height*depth*getNumElemBytes(format);
		}
	}
	//-----------------------------------------------------------------------
	size_t PixelUtil::getNumElemBits( PixelFormat format )
	{
		return getDescriptionFor(format).elemBytes * 8;
	}
	//-----------------------------------------------------------------------
	unsigned int PixelUtil::getFlags( PixelFormat format )
	{
		return getDescriptionFor(format).flags;
	}
	//-----------------------------------------------------------------------
	bool PixelUtil::hasAlpha(PixelFormat format)
	{
		return (PixelUtil::getFlags(format) & PFF_HASALPHA) > 0;
	}
	//-----------------------------------------------------------------------
	bool PixelUtil::isDepth(PixelFormat format)
	{
		return (PixelUtil::getFlags(format) & PFF_DEPTH) > 0;
	}

	//-----------------------------------------------------------------------
	void PixelUtil::getBitDepths(PixelFormat format, int rgba[4])
	{
		const PixelFormatDescription &des = getDescriptionFor(format);
		rgba[0] = des.rbits;
		rgba[1] = des.gbits;
		rgba[2] = des.bbits;
		rgba[3] = des.abits;
	}




}