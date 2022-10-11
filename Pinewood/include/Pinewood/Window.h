#pragma once
#include <Pinewood/Core.h>
#include <Pinewood/Error.h>
#include <Pinewood/EnumSupport.h>
#include <Pinewood/Input.h>

#include <bitset>

namespace Pinewood
{
	// Forward declaration
	class Window;

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

	namespace Operators
	{
		PW_DEFINE_ENUMCLASS_OPERATOR_OR(WindowCreateFlags);
		PW_DEFINE_ENUMCLASS_OPERATOR_AND(WindowCreateFlags);
		PW_DEFINE_ENUMCLASS_OPERATOR_NOT(WindowCreateFlags);
	}
	using namespace Operators;

	struct WindowCreateInfo
	{
		std::string_view title;	// Window title
		int16_t x, y;			// Position of the window
		uint16_t width, height;	// Size of the window client area
		WindowCreateFlags flags = WindowCreateFlags::DefaultStyle;	// Create flags and style
	};

	enum class WindowEventCode
	{
		Null,
		KeyDown,
		KeyUp,
		KeyChar,
		MouseButtonDown,
		MouseButtonUp,
		MouseScroll,
		MouseMove,
		WindowResize,		// Only when the window finishes resizing
		WindowResizing,		// Use WindowResize if you can, this one updates a lot during resize
		WindowMove,			// Only when the window finishes moving
		WindowMoving,		// Use WindowMove if you can, this one updates a lot when the window is moved
		WindowDestroy,
		// WindowCreate is not present because a window does not have event handlers on creation, hense it would be useless
	};

	// Base for window events
	struct WindowEvent {};

	// KeyDown and KeyUp
	struct WindowKeyEvent : WindowEvent
	{
		KeyCode key;
	};

	// KeyChar
	struct WindowKeyCharEvent : WindowEvent
	{
		char character;
	};
	
	// MouseButtonDown, MouseButtonUp, and MouseMove
	struct WindowMouseEvent : WindowEvent
	{
		// Relative to top-left corner
		uint16_t x, y;
		MouseButton buttons;
	};

	// MouseScroll
	struct WindowMouseScrollEvent : WindowEvent
	{
		// Relative to top-left corner
		uint16_t x, y;
		MouseButton buttons;
		float scroll;
	};

	// WindowResize and WindowResizing
	struct WindowResizeEvent : WindowEvent
	{
		uint16_t width, height;
		WindowShowMode showMode;
	};

	// WindowMove and WindowMoving
	struct WindowMoveEvent : WindowEvent
	{
		int16_t x, y;
	};

	struct WindowEventHandler
	{
		using Function = void(*)(const Window& window, const WindowEvent& event, void* userPointer);
		Function function;
		void* userPointer;
		WindowEventCode event;
	};

	template<typename TClass, void(TClass::* Function)(const Window&, const WindowEvent&)>
	WindowEventHandler WrapWindowEventHandler(TClass* object, WindowEventCode event)
	{
		static auto wrapperFunc = [](const Window& window, const WindowEvent& event, void* userPointer)
		{
			auto e = static_cast<const WindowKeyEvent&>(event);
			(static_cast<TClass*>(userPointer)->*Function)(window, event);
		};

		return WindowEventHandler{
			.function = wrapperFunc,
			.userPointer = static_cast<void*>(object),
			.event = event
		};
	}

	class Window
	{
	public:
		using NativeHandle = void*;

		Window() = default;
		Window(const Window& rhs) = default;
		Window(Window&& rhs) noexcept :m_details(std::move(rhs.m_details)) {}
		~Window() = default;

		Window& operator=(const Window&) = default;
		Window& operator=(Window&& rhs) noexcept { m_details = std::move(rhs.m_details); return *this; }

		Result Create(const WindowCreateInfo& createInfo);
		Result Destroy();

		Result Update();
		bool IsRunning();

		Result SetShowMode(WindowShowMode showMode);

		NativeHandle GetNativeHandle();

		bool IsInitialized();

		// If the eventHandler is already present, it will not be added and Result::InvalidParameter will be returned
		Result AddEventHandler(const WindowEventHandler& eventHandler);

		// If the eventHandler is not present, Result::InvalidParameter will be returned
		Result RemoveEventHandler(const WindowEventHandler& eventHandler);

	private:
		class Details;

		Window(std::shared_ptr<Details> details) :m_details(details) {}

		std::shared_ptr<Details> m_details;
	};
}
