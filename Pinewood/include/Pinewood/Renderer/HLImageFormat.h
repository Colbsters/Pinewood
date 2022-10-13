#pragma once
#include <Pinewood/Core.h>

namespace Pinewood
{
	enum class HLImageFormat
	{
		Unknown			= 0,
		// 8 bit per channel formats
		R8_UNorm,
		R8G8_UNorm,
		R8G8B8_UNorm,
		R8G8B8A8_UNorm,
		R8_SNorm,
		R8G8_SNorm,
		R8G8B8_SNorm,
		R8G8B8A8_SNorm,
		R8_UInt,
		R8G8_UInt,
		R8G8B8_UInt,
		R8G8B8A8_UInt,
		R8_SInt,
		R8G8_SInt,
		R8G8B8_SInt,
		R8G8B8A8_SInt,
		
		// 16 bits per channel formats
		R16_UNorm,
		R16G16_UNorm,
		R16G16B16_UNorm,
		R16G16B16A16_UNorm,
		R16_SNorm,
		R16G16_SNorm,
		R16G16B16_SNorm,
		R16G16B16A16_SNorm,
		R16_UInt,
		R16G16_UInt,
		R16G16B16_UInt,
		R16G16B16A16_UInt,
		R16_SInt,
		R16G16_SInt,
		R16G16B16_SInt,
		R16G16B16A16_SInt,
		R16_Float,
		R16G16_Float,
		R16G16B16_Float,
		R16G16B16A16_Float,

		// 32 bits per channel
		R32_UInt,
		R32G32_UInt,
		R32G32B32_UInt,
		R32G32B32A32_UInt,
		R32_SInt,
		R32G32_SInt,
		R32G32B32_SInt,
		R32G32B32A32_SInt,
		R32_Float,
		R32G32_Float,
		R32G32B32_Float,
		R32G32B32A32_Float,

		// Special formats
		R9G9B9E5_SExp,		// Floating point with a 9-bit fraction per channel, and a 5-bit exponent shared between channels
		R10G10B10A2_UNORM,
		R10G10B10A2_UInt,
		R11G11B10_Float,

		// Depth + stencil formats
		D24S8_UInt
	};
}
