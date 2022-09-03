#pragma once
#include <Pinewood/Core.h>
#include <exception>

namespace Pinewood
{
	enum class Result : uint32_t
	{
		// Success Values
		Success				= 0,

		// Error values (General)
		UnknownError		= 0x80000000,
		SystemError			= 0x80000001,
		InvalidParameter	= 0x80000002,
		OutOfMemory			= 0x80000003,
		NotInitialized		= 0x80000004,

		// Rendering subsystems
		BufferMapCorrupt	= 0x90000000,	// The buffer got corrupted while mapped, buffer must be reloaded
	};

	constexpr bool IsError(Result result)
	{
		return static_cast<uint32_t>(result) & 0x80000000;
	}
}
