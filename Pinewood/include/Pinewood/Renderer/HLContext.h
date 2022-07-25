#pragma once

#include <Pinewood/Core.h>
#include <Pinewood/Error.h>
#include <Pinewood/Window.h>

namespace Pinewood
{
	struct HLContextCreateInfo
	{
		std::shared_ptr<Window> window;
		
		// Add more things when needed
	};

	// Context for high-level rendering APIs
	class HLContext
	{
	public:
		using NativeHandle = void*;

		Result Create(const HLContextCreateInfo& createInfo);
		Result Destroy();

		NativeHandle GetNativeHandle();

	private:
	};
}
