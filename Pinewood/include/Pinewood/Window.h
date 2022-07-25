#pragma once
#include <Pinewood/Core.h>
#include <Pinewood/Error.h>
#include <Pinewood/EnumSupport.h>

#include <atomic>

namespace Pinewood
{
	enum class WindowCreateFlags : uint32_t
	{
		// Window Styles
		MinimizeButton	= 0x0001,
		MaximizeButton	= 0x0002,
		SystemMenu		= 0x0004,
		Resizeable		= 0x0008,
		Show			= 0x0010,
		Minimized		= 0x0020,
		Maximized		= 0x0030,

		DefaultStyle	= MinimizeButton | MaximizeButton | SystemMenu | Resizeable,

		// Other flags
		Async			= 0x0100, // Not yet implemented. Creates and updates window on separate thread (message handlers will need o be thread-safe)

		// Other values
		ShowBitMask		= 0x0030 // Bitmask for the show field
	};

	enum class WindowShowMode : uint32_t
	{
		Hide		= 0,
		Show		= 1,
		Minimized	= 2,
		Maximized	= 3
	};

	namespace operators
	{
		PW_DEFINE_ENUMCLASS_OPERATOR_OR(WindowCreateFlags);
		PW_DEFINE_ENUMCLASS_OPERATOR_AND(WindowCreateFlags);
		PW_DEFINE_ENUMCLASS_OPERATOR_EQUALS(WindowCreateFlags);
		PW_DEFINE_ENUMCLASS_OPERATOR_NOT(WindowCreateFlags);
	}
	using namespace operators;

	struct WindowCreateInfo
	{
		std::string_view title;	// Window title
		int16_t x, y;			// Position of the window
		uint16_t width, height;	// Size of the window client area
		uint32_t flags = static_cast<uint32_t>(WindowCreateFlags::DefaultStyle);	// Create flags and style
	};

	class Window
	{
	public:
		using NativeHandle = void*;

		Result Create(const WindowCreateInfo& createInfo);
		Result Destroy();

		Result Update();
		bool IsRunning();

		Result SetShowMode(WindowShowMode showMode);

		NativeHandle GetNativeHandle();

	private:
#if PW_PLATFORM_WINDOWS
		static intptr_t __stdcall WindowProc(void*, uint32_t, uintptr_t, intptr_t);
		static void AsyncWindowThread();
		Result CreateWindowImpl(const WindowCreateInfo&);
		Result CreateWindowAsync(const WindowCreateInfo&);

		void* m_window;
		std::atomic_bool m_isRunning;
#endif // PW_PLATFORM_WINDOWS
	};
}
