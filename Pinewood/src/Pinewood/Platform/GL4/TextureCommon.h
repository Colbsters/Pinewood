#pragma once
#include <Pinewood/Core.h>

#include <Pinewood/Renderer/HL/HLImageFormat.h>

namespace Pinewood::Impl
{
	struct GLFormat
	{
		uint32_t baseFormat, sizeFormat;
	};

	constexpr GLFormat GetGLFormat(HLImageFormat format)
	{
		constexpr GLFormat formatTable[]
		{
			{ 0, 0 }, // Unknown
			{ GL_RED,	GL_R8				}, // R8_UNorm
			{ GL_RG,	GL_RG8				}, // R8G8_UNorm
			{ GL_RGB,	GL_RGB8				}, // R8G8B8_UNorm
			{ GL_RGBA,	GL_RGBA8			}, // R8G8B8A8_UNorm
			{ GL_RED,	GL_R8_SNORM			}, // R8_SNorm
			{ GL_RG,	GL_RG8_SNORM		}, // R8G8_SNorm
			{ GL_RGB,	GL_RGB8_SNORM		}, // R8G8B8_SNorm
			{ GL_RGBA,	GL_RGBA8_SNORM		}, // R8G8B8A8_SNorm
			{ GL_RED,	GL_R8UI				}, // R8_UInt
			{ GL_RG,	GL_RG8UI			}, // R8G8_UInt
			{ GL_RGB,	GL_RGB8UI			}, // R8G8B8_UInt
			{ GL_RGBA,	GL_RGBA8UI			}, // R8G8B8A8_UInt
			{ GL_RED,	GL_R8I				}, // R8_SInt
			{ GL_RG,	GL_RG8I				}, // R8G8_SInt
			{ GL_RGB,	GL_RGB8I			}, // R8G8B8_SInt
			{ GL_RGBA,	GL_RGBA8I			}, // R8G8B8A8_SInt
			{ GL_RED,	GL_R16				}, // R16_UNorm
			{ GL_RG,	GL_RG16				}, // R16G16_UNorm
			{ GL_RGB,	GL_RGB16			}, // R16G16B16_UNorm
			{ GL_RGBA,	GL_RGBA16			}, // R16G16B16A16_UNorm
			{ GL_RED,	GL_R16_SNORM		}, // R16_SNorm
			{ GL_RG,	GL_RG16_SNORM		}, // R16G16_SNorm
			{ GL_RGB,	GL_RGB16_SNORM		}, // R16G16B16_SNorm
			{ GL_RGBA,	GL_RGBA16_SNORM		}, // R16G16B16A16_SNorm
			{ GL_RED,	GL_R16UI			}, // R16_UInt
			{ GL_RG,	GL_RG16UI			}, // R16G16_UInt
			{ GL_RGB,	GL_RGB16UI			}, // R16G16B16_UInt
			{ GL_RGBA,	GL_RGBA16UI			}, // R16G16B16A16_UInt
			{ GL_RED,	GL_R16I				}, // R16_SInt
			{ GL_RG,	GL_RG16I			}, // R16G16_SInt
			{ GL_RGB,	GL_RGB16I			}, // R16G16B16_SInt
			{ GL_RGBA,	GL_RGBA16I			}, // R16G16B16A16_SInt
			{ GL_RED,	GL_R16F				}, // R16_Float
			{ GL_RG,	GL_RG16F			}, // R16G16_Float
			{ GL_RGB,	GL_RGB16F			}, // R16G16B16_Float
			{ GL_RGBA,	GL_RGBA16F			}, // R16G16B16A16_Float
			{ GL_RED,	GL_R32UI			}, // R32_UInt
			{ GL_RG,	GL_RG32UI			}, // R32G32_UInt
			{ GL_RGB,	GL_RGB32UI			}, // R32G32B32_UInt
			{ GL_RGBA,	GL_RGBA32UI			}, // R32G32B32A32_UInt
			{ GL_RED,	GL_R32I				}, // R32_SInt
			{ GL_RG,	GL_RG32I			}, // R32G32_SInt
			{ GL_RGB,	GL_RGB32I			}, // R32G32B32_SInt
			{ GL_RGBA,	GL_RGBA32I			}, // R32G32B32A32_SInt
			{ GL_RED,	GL_R32F				}, // R32_Float
			{ GL_RG,	GL_RG32F			}, // R32G32_Float
			{ GL_RGB,	GL_RGB32F			}, // R32G32B32_Float
			{ GL_RGBA,	GL_RGBA32F			}, // R32G32B32A32_Float
			{ GL_RGB,	GL_RGB9_E5			}, // R9G9B9E5_SExp
			{ GL_RGBA,	GL_RGB10_A2			}, // R10G10B10A2_UNORM
			{ GL_RGBA,	GL_RGB10_A2UI		}, // R10G10B10A2_UInt
			{ GL_RGB,	GL_R11F_G11F_B10F	}, // R11G11B10_Float
			{ GL_DEPTH_STENCIL,	GL_DEPTH24_STENCIL8 } // D24S8_UInt
		};

		// Use the look-up table, and don't forget to do bounds checking
		return formatTable[static_cast<uint32_t>(format) < (sizeof(formatTable) / sizeof(GLFormat)) ? static_cast<uint32_t>(format) : 0];
	}

	constexpr GLFormat GetGLFormat2(HLImageFormat format)
	{
		constexpr GLFormat formatTable[]
		{
			{ 0, 0 }, // Unknown
			{ GL_RED,	GL_UNSIGNED_BYTE	}, // R8_UNorm
			{ GL_RG,	GL_UNSIGNED_BYTE	}, // R8G8_UNorm
			{ GL_RGB,	GL_UNSIGNED_BYTE	}, // R8G8B8_UNorm
			{ GL_RGBA,	GL_UNSIGNED_BYTE	}, // R8G8B8A8_UNorm
			{ GL_RED,	GL_BYTE				}, // R8_SNorm
			{ GL_RG,	GL_BYTE				}, // R8G8_SNorm
			{ GL_RGB,	GL_BYTE				}, // R8G8B8_SNorm
			{ GL_RGBA,	GL_BYTE				}, // R8G8B8A8_SNorm
			{ GL_RED,	GL_UNSIGNED_BYTE	}, // R8_UInt
			{ GL_RG,	GL_UNSIGNED_BYTE	}, // R8G8_UInt
			{ GL_RGB,	GL_UNSIGNED_BYTE	}, // R8G8B8_UInt
			{ GL_RGBA,	GL_UNSIGNED_BYTE	}, // R8G8B8A8_UInt
			{ GL_RED,	GL_BYTE				}, // R8_SInt
			{ GL_RG,	GL_BYTE				}, // R8G8_SInt
			{ GL_RGB,	GL_BYTE				}, // R8G8B8_SInt
			{ GL_RGBA,	GL_BYTE				}, // R8G8B8A8_SInt
			{ GL_RED,	GL_UNSIGNED_SHORT	}, // R16_UNorm
			{ GL_RG,	GL_UNSIGNED_SHORT	}, // R16G16_UNorm
			{ GL_RGB,	GL_UNSIGNED_SHORT	}, // R16G16B16_UNorm
			{ GL_RGBA,	GL_UNSIGNED_SHORT	}, // R16G16B16A16_UNorm
			{ GL_RED,	GL_SHORT			}, // R16_SNorm
			{ GL_RG,	GL_SHORT			}, // R16G16_SNorm
			{ GL_RGB,	GL_SHORT			}, // R16G16B16_SNorm
			{ GL_RGBA,	GL_SHORT			}, // R16G16B16A16_SNorm
			{ GL_RED,	GL_UNSIGNED_SHORT	}, // R16_UInt
			{ GL_RG,	GL_UNSIGNED_SHORT	}, // R16G16_UInt
			{ GL_RGB,	GL_UNSIGNED_SHORT	}, // R16G16B16_UInt
			{ GL_RGBA,	GL_UNSIGNED_SHORT	}, // R16G16B16A16_UInt
			{ GL_RED,	GL_SHORT			}, // R16_SInt
			{ GL_RG,	GL_SHORT			}, // R16G16_SInt
			{ GL_RGB,	GL_SHORT			}, // R16G16B16_SInt
			{ GL_RGBA,	GL_SHORT			}, // R16G16B16A16_SInt
			{ GL_RED,	GL_HALF_FLOAT		}, // R16_Float
			{ GL_RG,	GL_HALF_FLOAT		}, // R16G16_Float
			{ GL_RGB,	GL_HALF_FLOAT		}, // R16G16B16_Float
			{ GL_RGBA,	GL_HALF_FLOAT		}, // R16G16B16A16_Float
			{ GL_RED,	GL_UNSIGNED_INT		}, // R32_UInt
			{ GL_RG,	GL_UNSIGNED_INT		}, // R32G32_UInt
			{ GL_RGB,	GL_UNSIGNED_INT		}, // R32G32B32_UInt
			{ GL_RGBA,	GL_UNSIGNED_INT		}, // R32G32B32A32_UInt
			{ GL_RED,	GL_INT				}, // R32_SInt
			{ GL_RG,	GL_INT				}, // R32G32_SInt
			{ GL_RGB,	GL_INT				}, // R32G32B32_SInt
			{ GL_RGBA,	GL_INT				}, // R32G32B32A32_SInt
			{ GL_RED,	GL_FLOAT			}, // R32_Float
			{ GL_RG,	GL_FLOAT			}, // R32G32_Float
			{ GL_RGB,	GL_FLOAT			}, // R32G32B32_Float
			{ GL_RGBA,	GL_FLOAT			}, // R32G32B32A32_Float
			{ GL_RGB,	GL_FLOAT			}, // R9G9B9E5_SExp
			{ GL_RGBA,	GL_FLOAT			}, // R10G10B10A2_UNORM
			{ GL_RGBA,	GL_FLOAT			}, // R10G10B10A2_UInt
			{ GL_RGB,	GL_FLOAT			}, // R11G11B10_Float
			{ GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8 } // D24S8_UInt
		};

		// Use the look-up table, and don't forget to do bounds checking
		return formatTable[static_cast<uint32_t>(format) < (sizeof(formatTable) / sizeof(GLFormat)) ? static_cast<uint32_t>(format) : 0];
	}
}
