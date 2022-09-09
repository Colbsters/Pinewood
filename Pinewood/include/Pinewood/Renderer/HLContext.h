#pragma once

#include <Pinewood/Core.h>
#include <Pinewood/Error.h>
#include <Pinewood/Window.h>

namespace Pinewood
{
	struct HLContextCreateInfo
	{
		Window window;
		uint32_t swapInterval = 0;
	};

	// Context for high-level rendering APIs
	class HLContext
	{
	public:
		// OpenGL 4.5 native handle
		using NativeHandle = struct { void* renderContext, * gl; };
		
		HLContext() = default;
		HLContext(const HLContext&) = default;
		HLContext(HLContext&& rhs) noexcept :m_details(std::move(rhs.m_details)) {}
		~HLContext() = default;

		HLContext& operator=(const HLContext&) = default;
		HLContext& operator=(HLContext&& rhs) noexcept { m_details = std::move(rhs.m_details); return *this; }

		// Creates a context for a window
		Result Create(const HLContextCreateInfo& createInfo);

		// Destroys the context
		Result Destroy();

		// Swap the buffers
		Result SwapBuffers();

		// Swaps after interval amount of vsyncs, set to 0 to disable vsync
		// NOTE: interval may have a maximum depending on API
		Result SetSwapInterval(uint32_t interval);

		// Resize the swap chain (may change viewport and scissor settings)
		Result ResizeSwapChain(uint16_t width, uint32_t height);

		// Makes the context current, required for OpenGL only
		// NOTES:
		//  - Context is already made current during creation
		//  - Each thread can has a separate current context
		//  - The context must be current before any HLContext call (except Destroy)
		//  - The context must be current before any HLRenderInterface
		Result MakeCurrent();

		// Gets the native handle and information
		NativeHandle GetNativeHandle();

		bool IsInitialized();

	private:
		class Details;

		HLContext(std::shared_ptr<Details> details) :m_details(details) {}

		std::shared_ptr<Details> m_details;
	};
}
