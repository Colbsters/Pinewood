#pragma once

#include <Pinewood/Core.h>
#include <Pinewood/Error.h>
#include <Pinewood/Window.h>

namespace Pinewood
{
	struct HLContextCreateInfo
	{
		std::shared_ptr<Window> window;
		uint32_t swapInterval = 0;
	};

	// Context for high-level rendering APIs
	class HLContext
	{
	public:
		// OpenGL 4.5 native handle
		using NativeHandle = struct { void* renderContext, * gl; };
		
		~HLContext();

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

	private:
#if PW_RENDERER_OPENGL4

		static Result InitializeWGL();
		Result MakeObsolete();
		static void __stdcall DebugMessageCallback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int32_t length, const char* message, const void* userParam);

		void* m_deviceContext;
		void* m_renderContext;
		void* m_gl;
#endif // ^^^ PW_RENDERER_OPENGL4
	};
}
