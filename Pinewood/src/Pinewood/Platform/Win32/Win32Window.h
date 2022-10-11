#pragma once
#include <Pinewood/Window.h>
#include <thread>
#include <mutex>
#include <cwchar>

#include <windowsx.h>

namespace Pinewood
{
	static std::mutex g_win32InfoMutex;
	// Win32 info (guarded by g_win32InfoMutex)
	static ATOM g_wndClassAtom;
	static HINSTANCE g_win32Instance;

	// Used for creating windows in the async window thread
	static std::mutex g_windowCreateMutex;
	static const WindowCreateInfo* volatile g_windowCreateInfo;
	static Window* volatile g_windowCreatePtr; // I would've used Window::Detail, but it's private
	static volatile Result g_windowCreateResult;
	static std::atomic_bool g_windowCreateFinished;

	static std::atomic_bool g_windowThreadInitialized;
	static std::thread g_windowThread;

	static std::atomic_uint64_t g_numWindows;

	// Without the event code
	struct WindowEventHandler2
	{
		WindowEventHandler::Function function;
		void* userPointer;

		bool operator==(const WindowEventHandler2& rhs)
		{
			return (function == rhs.function) && (userPointer == rhs.userPointer);
		}
	};

	class Window::Details
		:public std::enable_shared_from_this<Window::Details>
	{
	public:
		HWND window;
		bool isRunning;

		std::array<std::vector<WindowEventHandler2>, 12> eventHandlers;

		Result CreateWindowImpl(const WindowCreateInfo& createInfo);
		Result CreateWindowAsync(const WindowCreateInfo& createInfo, Window* ptr);
		static void AsyncWindowThread();
		static intptr_t CALLBACK WindowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
	};

	static WindowShowMode SizingTypeToShowMode(uint32_t sizingType)
	{
		switch (sizingType)
		{
		case SIZE_MINIMIZED:
			return WindowShowMode::Minimized;
		case SIZE_MAXIMIZED:
			return WindowShowMode::Maximized;
		default:
			return WindowShowMode::Show;
		}
	}

	static MouseButton Win32MouseToMouseButtons(uint32_t buttons, MouseButton overrideButton /* Used to give mouse buttons presidence (ex: WM_LBUTTONDOWN) */)
	{
		MouseButton out = MouseButton::Null;
		if (buttons & MK_CONTROL) out |= MouseButton::Control;
		if (buttons & MK_SHIFT) out |= MouseButton::Shift;

		// If there is a presidence, return it with that
		if (overrideButton != MouseButton::Null)
			return out | overrideButton;

		if (buttons & MK_LBUTTON) out |= MouseButton::LeftButton;
		if (buttons & MK_MBUTTON) out |= MouseButton::MiddleButton;
		if (buttons & MK_RBUTTON) out |= MouseButton::RightButton;
		if (buttons & MK_XBUTTON1) out |= MouseButton::XButton1;
		if (buttons & MK_XBUTTON2) out |= MouseButton::XButton2;

		return out;
	}

	static KeyCode TranslateKey(uint32_t keyCode, uint32_t lparam)
	{
		// Deal with left/right keys
		if (keyCode == VK_CONTROL)
			return (lparam & 0x01000000) ? KeyCode::RightControl : KeyCode::LeftControl; // Use RightControl if the extended bit in lparam is enabled
		if (keyCode == VK_MENU)
			return (lparam & 0x01000000) ? KeyCode::RightAlt : KeyCode::LeftAlt; // Use RightAlt if the extended bit in lparam is enabled
		if (keyCode == VK_SHIFT)
		{
			// Map the scan code (bits 16-23 of lparam) to the keyCode
			uint32_t new_vk = MapVirtualKeyW((lparam >> 16) & 0xff, MAPVK_VSC_TO_VK_EX);
			return (new_vk == VK_RSHIFT) ? KeyCode::RightShift : KeyCode::LeftShift;
		}

		// For the rest we can use a translation table
		static constexpr std::array<KeyCode, 256> translationTable =
		{
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Backspace,
			KeyCode::Tab,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Clear,
			KeyCode::Enter,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Pause,
			KeyCode::CapsLock,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Escape,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Space,
			KeyCode::PageUp,
			KeyCode::PageDown,
			KeyCode::End,
			KeyCode::Home,
			KeyCode::Left,
			KeyCode::Up,
			KeyCode::Right,
			KeyCode::Down,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Insert,
			KeyCode::Delete,
			KeyCode::Help,
			KeyCode::Keyboard0,
			KeyCode::Keyboard1,
			KeyCode::Keyboard2,
			KeyCode::Keyboard3,
			KeyCode::Keyboard4,
			KeyCode::Keyboard5,
			KeyCode::Keyboard6,
			KeyCode::Keyboard7,
			KeyCode::Keyboard8,
			KeyCode::Keyboard9,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::A,
			KeyCode::B,
			KeyCode::C,
			KeyCode::D,
			KeyCode::E,
			KeyCode::F,
			KeyCode::G,
			KeyCode::H,
			KeyCode::I,
			KeyCode::J,
			KeyCode::K,
			KeyCode::L,
			KeyCode::M,
			KeyCode::N,
			KeyCode::O,
			KeyCode::P,
			KeyCode::Q,
			KeyCode::R,
			KeyCode::S,
			KeyCode::T,
			KeyCode::U,
			KeyCode::V,
			KeyCode::W,
			KeyCode::X,
			KeyCode::Y,
			KeyCode::Z,
			KeyCode::LeftOS,
			KeyCode::RightOS,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Numpad0,
			KeyCode::Numpad1,
			KeyCode::Numpad2,
			KeyCode::Numpad3,
			KeyCode::Numpad4,
			KeyCode::Numpad5,
			KeyCode::Numpad6,
			KeyCode::Numpad7,
			KeyCode::Numpad8,
			KeyCode::Numpad9,
			KeyCode::Multiply,
			KeyCode::Add,
			KeyCode::Separator,
			KeyCode::Substract,
			KeyCode::Decimal,
			KeyCode::Divide,
			KeyCode::F1,
			KeyCode::F2,
			KeyCode::F3,
			KeyCode::F4,
			KeyCode::F5,
			KeyCode::F6,
			KeyCode::F7,
			KeyCode::F8,
			KeyCode::F9,
			KeyCode::F10,
			KeyCode::F11,
			KeyCode::F12,
			KeyCode::F13,
			KeyCode::F14,
			KeyCode::F15,
			KeyCode::F16,
			KeyCode::F17,
			KeyCode::F18,
			KeyCode::F19,
			KeyCode::F20,
			KeyCode::F21,
			KeyCode::F22,
			KeyCode::F23,
			KeyCode::F24,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::NumLock,
			KeyCode::ScrollLock,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::LeftShift,
			KeyCode::RightShift,
			KeyCode::LeftControl,
			KeyCode::RightControl,
			KeyCode::LeftAlt,
			KeyCode::RightAlt,
			KeyCode::BrowserBack,
			KeyCode::BrowserForward,
			KeyCode::BrowserRefresh,
			KeyCode::BrowserStop,
			KeyCode::BrowserSearch,
			KeyCode::BrowserFavorites,
			KeyCode::BrowserHome,
			KeyCode::VolumeMute,
			KeyCode::VolumeDown,
			KeyCode::VolumeUp,
			KeyCode::MediaNext,
			KeyCode::MediaPrevious,
			KeyCode::MediaStop,
			KeyCode::MediaPlayPause,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Semicolon,
			KeyCode::Plus,
			KeyCode::Comma,
			KeyCode::Minus,
			KeyCode::Period,
			KeyCode::ForwardSlash,
			KeyCode::Backtick,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::OpenBracket,
			KeyCode::Backslash,
			KeyCode::CloseBracket,
			KeyCode::SingleQuote,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Play,
			KeyCode::Zoom,
			KeyCode::Null,
			KeyCode::Null,
			KeyCode::Clear,
			KeyCode::Null,
		};

		return (translationTable[keyCode] != KeyCode::Null) ? translationTable[keyCode] : static_cast<KeyCode>(keyCode | 0x100);
	}

	Result Window::Details::CreateWindowImpl(const WindowCreateInfo& createInfo)
	{
		// Could've use std::codecvt, but that's deprecated
		// Might replace it with a better solution, but it works for now
		// The reason I did the std::string thing is because the output string might be smaller than expected and
		// more characters may be read from the input (remember, this is a string_view)
		const std::string narrowTitle{ createInfo.title };
		const char* narrowTitleCStr = narrowTitle.c_str();
		mbstate_t state = {};
		size_t len;
		mbsrtowcs_s(&len, NULL, 0, &narrowTitleCStr, 0, &state);
		std::vector<wchar_t> wideTitle(len + 1);
		mbsrtowcs_s(&len, wideTitle.data(), len + 1, &narrowTitleCStr, len, &state);


		uint32_t windowStyle =
			WS_OVERLAPPED |
			WS_THICKFRAME |
			WS_CAPTION |
			(static_cast<uint32_t>(createInfo.flags & WindowCreateFlags::SystemMenu) ? WS_SYSMENU : 0) |
			(static_cast<uint32_t>(createInfo.flags & WindowCreateFlags::MaximizeButton) ? WS_MAXIMIZEBOX : 0) |
			(static_cast<uint32_t>(createInfo.flags & WindowCreateFlags::MinimizeButton) ? WS_MINIMIZEBOX : 0);
		RECT windowRect{ createInfo.x, createInfo.y, createInfo.x + createInfo.width, createInfo.y + createInfo.height };
		AdjustWindowRectEx(&windowRect, windowStyle, false, WS_EX_OVERLAPPEDWINDOW);

		// m_window will be set by WindowProc()
		if (!CreateWindowExW(
			WS_EX_OVERLAPPEDWINDOW,
			reinterpret_cast<LPCWSTR>(static_cast<uintptr_t>(g_wndClassAtom)),
			wideTitle.data(),
			windowStyle,
			windowRect.left, windowRect.top,
			windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
			nullptr, nullptr, g_win32Instance, this))
		{
			g_numWindows--; // Nevermind, a window wasn't created
			return Result::SystemError;
		}

		WindowCreateFlags show = (createInfo.flags & WindowCreateFlags::ShowBitMask);
		if (static_cast<uint32_t>(show))
			ShowWindow(reinterpret_cast<HWND>(window),
				(show == WindowCreateFlags::Show) ? SW_SHOW :
				((show == WindowCreateFlags::Minimized) ? SW_MINIMIZE : SW_MAXIMIZE));

		return Result::Success;
	}

	Result Window::Details::CreateWindowAsync(const WindowCreateInfo& createInfo, Window* ptr)
	{
		std::lock_guard lock{ g_windowCreateMutex };

		// Setup parameters
		g_windowCreateInfo = &createInfo;
		g_windowCreatePtr = ptr;
		g_windowCreateFinished = false;

		// Create the thread if it wasn't already created
		if (!g_windowThreadInitialized)
		{
			g_windowThread = std::move(std::thread{ AsyncWindowThread });
			g_windowThread.detach();
			g_windowThreadInitialized = true;
		}

		// Wait until the window creation was finished
		while (!g_windowCreateFinished)	Sleep(0);

		return g_windowCreateResult;
	}

	void Window::Details::AsyncWindowThread()
	{
		MSG msg;
		while (g_numWindows > 0)
		{
			while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				// Handle the messages
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}

			if (g_windowCreateFinished == false) // Create a new window if needed
			{
				g_windowCreateResult = g_windowCreatePtr->m_details->CreateWindowImpl(*g_windowCreateInfo);
				g_windowCreateFinished = true;
			}

			WaitMessage();
		}

		g_windowThreadInitialized = false;
	}

	intptr_t CALLBACK Window::Details::WindowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
	{
		Window::Details* windowDetails = nullptr;

		if (message == WM_NCCREATE)
		{
			// Setup window user pointer and window information (m_window and m_isRunning)
			const auto* createStruct = reinterpret_cast<CREATESTRUCTW*>(lparam);
			windowDetails = reinterpret_cast<Window::Details*>(createStruct->lpCreateParams);

			SetWindowLongPtrW(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(windowDetails));

			windowDetails->window = window;
			windowDetails->isRunning = true;
		}
		else
			// Get window user pointer
			windowDetails = reinterpret_cast<Window::Details*>(GetWindowLongPtrW(window, GWLP_USERDATA));

		if (windowDetails)
		{
			switch (message)
			{
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			{
				Window windowObj{ windowDetails->shared_from_this() };
				WindowKeyEvent event{
					.key = TranslateKey(static_cast<uint32_t>(wparam), static_cast<uint32_t>(lparam))
				};

				for (auto& eventHandler : windowDetails->eventHandlers[static_cast<uint32_t>(WindowEventCode::KeyDown) - 1])
					eventHandler.function(windowObj, event, eventHandler.userPointer);

				break;
			}
			case WM_KEYUP:
			case WM_SYSKEYUP:
			{
				Window windowObj{ windowDetails->shared_from_this() };
				WindowKeyEvent event{
					.key = TranslateKey(static_cast<uint32_t>(wparam), static_cast<uint32_t>(lparam))
				};

				for (auto& eventHandler : windowDetails->eventHandlers[static_cast<uint32_t>(WindowEventCode::KeyUp) - 1])
					eventHandler.function(windowObj, event, eventHandler.userPointer);

				break;
			}
			case WM_CHAR:
			{
				Window windowObj{ windowDetails->shared_from_this() };
				WindowKeyCharEvent event{
					.character = static_cast<char>(wparam)
				};

				for (auto& eventHandler : windowDetails->eventHandlers[static_cast<uint32_t>(WindowEventCode::KeyChar) - 1])
					eventHandler.function(windowObj, event, eventHandler.userPointer);

				break;
			}
			case WM_LBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_XBUTTONDOWN:
			{
				MouseButton button;

				switch (message)
				{
				case WM_LBUTTONDOWN: button = MouseButton::LeftButton; break;
				case WM_MBUTTONDOWN: button = MouseButton::MiddleButton; break;
				case WM_RBUTTONDOWN: button = MouseButton::RightButton; break;
				case WM_XBUTTONDOWN: button = (HIWORD(wparam) == XBUTTON1) ? MouseButton::XButton1 : MouseButton::XButton2; break;
				}

				Window windowObj{ windowDetails->shared_from_this() };
				WindowMouseEvent event{
					.x = static_cast<uint16_t>(GET_X_LPARAM(lparam)),
					.y = static_cast<uint16_t>(GET_Y_LPARAM(lparam)),
					.buttons = Win32MouseToMouseButtons(LOWORD(wparam), button)
				};

				for (auto& eventHandler : windowDetails->eventHandlers[static_cast<uint32_t>(WindowEventCode::MouseButtonDown) - 1])
					eventHandler.function(windowObj, event, eventHandler.userPointer);

				break;
			}
			case WM_LBUTTONUP:
			case WM_MBUTTONUP:
			case WM_RBUTTONUP:
			case WM_XBUTTONUP:
			{
				MouseButton button;

				switch (message)
				{
				case WM_LBUTTONUP: button = MouseButton::LeftButton; break;
				case WM_MBUTTONUP: button = MouseButton::MiddleButton; break;
				case WM_RBUTTONUP: button = MouseButton::RightButton; break;
				case WM_XBUTTONUP: button = (HIWORD(wparam) == XBUTTON1) ? MouseButton::XButton1 : MouseButton::XButton2; break;
				}

				Window windowObj{ windowDetails->shared_from_this() };
				WindowMouseEvent event{
					.x = static_cast<uint32_t>(GET_X_LPARAM(lparam)),
					.y = static_cast<uint32_t>(GET_Y_LPARAM(lparam)),
					.buttons = Win32MouseToMouseButtons(LOWORD(wparam), button)
				};

				for (auto& eventHandler : windowDetails->eventHandlers[static_cast<uint32_t>(WindowEventCode::MouseButtonUp) - 1])
					eventHandler.function(windowObj, event, eventHandler.userPointer);

				break;
			}
			case WM_MOUSEWHEEL:
			{
				Window windowObj{ windowDetails->shared_from_this() };
				WindowMouseScrollEvent event{
					.x = static_cast<uint32_t>(GET_X_LPARAM(lparam)),
					.y = static_cast<uint32_t>(GET_Y_LPARAM(lparam)),
					.buttons = Win32MouseToMouseButtons(GET_KEYSTATE_WPARAM(wparam), MouseButton::Null),
					.scroll = static_cast<float>(GET_WHEEL_DELTA_WPARAM(wparam)) / 120.0f
				};

				for (auto& eventHandler : windowDetails->eventHandlers[static_cast<uint32_t>(WindowEventCode::MouseScroll) - 1])
					eventHandler.function(windowObj, event, eventHandler.userPointer);

				break;
			}
			case WM_MOUSEMOVE:
			{
				Window windowObj{ windowDetails->shared_from_this() };
				WindowMouseEvent event{
					.x = static_cast<uint32_t>(GET_X_LPARAM(lparam)),
					.y = static_cast<uint32_t>(GET_Y_LPARAM(lparam)),
					.buttons = Win32MouseToMouseButtons(static_cast<uint32_t>(wparam), MouseButton::Null)
				};

				for (auto& eventHandler : windowDetails->eventHandlers[static_cast<uint32_t>(WindowEventCode::MouseMove) - 1])
					eventHandler.function(windowObj, event, eventHandler.userPointer);

				break;
			}
			case WM_SIZE:
			{
				Window windowObj{ windowDetails->shared_from_this() };
				WindowResizeEvent event{
					.width = LOWORD(lparam),
					.height = HIWORD(lparam),
					.showMode = SizingTypeToShowMode(static_cast<uint32_t>(wparam)),
				};

				for (auto& eventHandler : windowDetails->eventHandlers[static_cast<uint32_t>(WindowEventCode::WindowResize) - 1])
					eventHandler.function(windowObj, event, eventHandler.userPointer);

				break;
			}
			case WM_SIZING:
			{
				Window windowObj{ windowDetails->shared_from_this() };
				WindowResizeEvent event{
					.width = LOWORD(lparam),
					.height = HIWORD(lparam),
					.showMode = SizingTypeToShowMode(static_cast<uint32_t>(wparam)),
				};

				for (auto& eventHandler : windowDetails->eventHandlers[static_cast<uint32_t>(WindowEventCode::WindowResizing) - 1])
					eventHandler.function(windowObj, event, eventHandler.userPointer);

				break;
			}
			case WM_MOVE:
			{
				Window windowObj{ windowDetails->shared_from_this() };
				WindowMoveEvent event{
					.x = GET_X_LPARAM(lparam),
					.y = GET_Y_LPARAM(lparam),
				};

				for (auto& eventHandler : windowDetails->eventHandlers[static_cast<uint32_t>(WindowEventCode::WindowMove) - 1])
					eventHandler.function(windowObj, event, eventHandler.userPointer);

				break;
			}
			case WM_MOVING:
			{
				Window windowObj{ windowDetails->shared_from_this() };
				WindowMoveEvent event{
					.x = GET_X_LPARAM(lparam),
					.y = GET_Y_LPARAM(lparam),
				};

				for (auto& eventHandler : windowDetails->eventHandlers[static_cast<uint32_t>(WindowEventCode::WindowMoving) - 1])
					eventHandler.function(windowObj, event, eventHandler.userPointer);

				break;
			}
			case WM_DESTROY:
			{
				Window windowObj{ windowDetails->shared_from_this() };
				for (auto& eventHandler : windowDetails->eventHandlers[static_cast<uint32_t>(WindowEventCode::WindowDestroy) - 1])
					eventHandler.function(windowObj, WindowEvent{}, eventHandler.userPointer);

				PostQuitMessage(0);
				windowDetails->isRunning = false;
				g_numWindows--;
				break;
			}
			}
		}

		return DefWindowProcW(window, message, wparam, lparam);

	}

	Result Window::Create(const WindowCreateInfo& createInfo)
	{
		// Register a window class if one wasn't already
		{
			std::lock_guard<std::mutex> lock{ g_win32InfoMutex };

			if (!g_win32Instance)
				GetModuleHandleW(nullptr);

			if (!g_wndClassAtom)
			{
				WNDCLASSEXW wndClass{
					.cbSize = sizeof(wndClass),
					.style = CS_OWNDC,
					// It works, just HWND and void* aren't really compatible (they are both pointers). For more info, see WindowProc()'s definition
					.lpfnWndProc = Details::WindowProc,
					.cbWndExtra = sizeof(Window*),
					.hInstance = g_win32Instance,
					.hIcon = LoadIconW(0, IDI_APPLICATION),
					.hCursor = LoadCursorW(0, IDC_ARROW),
					.lpszClassName = L"Pinewood Window Class",
					.hIconSm = LoadIconW(0, IDI_APPLICATION)
				};

				if (!(g_wndClassAtom = RegisterClassExW(&wndClass)))
					return Result::SystemError;
			}
		}

		m_details = std::make_shared<Details>();
		g_numWindows++;
		// Create the thread only if WindowCreateFlags::Async was specified and we are not the window thread (if we were then a dead lock would occur)
		if (static_cast<uint32_t>(createInfo.flags & WindowCreateFlags::Async) && (g_windowThread.get_id() != std::this_thread::get_id()))
			return m_details->CreateWindowAsync(createInfo, this);
		else
			return m_details->CreateWindowImpl(createInfo);
	}

	Result Window::Destroy()
	{
		if (!DestroyWindow(m_details->window))
			return Result::SystemError;

		m_details->window = nullptr;

		return Result::Success;
	}

	Result Window::Update()
	{
		MSG msg;
		while (PeekMessageW(&msg, m_details->window, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		return Result::Success;
	}

	bool Window::IsRunning()
	{
		return m_details->isRunning;
	}

	Result Window::SetShowMode(WindowShowMode showMode)
	{
		ShowWindow(m_details->window,
			(showMode == WindowShowMode::Hide) ? SW_HIDE :
			((showMode == WindowShowMode::Show) ? SW_SHOW :
				((showMode == WindowShowMode::Minimized) ? SW_MINIMIZE : SW_MAXIMIZE)));

		return Result::Success;
	}

	Window::NativeHandle Window::GetNativeHandle()
	{
		return m_details->window;
	}

	bool Window::IsInitialized()
	{
		return m_details && m_details->window;
	}

	Result Window::AddEventHandler(const WindowEventHandler& eventHandler)
	{
		// We search with the smaller structure
		WindowEventHandler2 smallHandler{
			.function = eventHandler.function,
			.userPointer = eventHandler.userPointer
		};

		// Make sure the event handler isn't already present
		auto& eventHandlers = m_details->eventHandlers[static_cast<uint32_t>(eventHandler.event) - 1];
		if (std::find(eventHandlers.begin(), eventHandlers.end(), smallHandler) != eventHandlers.end())
			return Result::InvalidParameter;

		eventHandlers.push_back(smallHandler);
		return Result::Success;

	}

	Result Window::RemoveEventHandler(const WindowEventHandler& eventHandler)
	{
		// We search with the smaller structure
		WindowEventHandler2 smallHandler{
			.function = eventHandler.function,
			.userPointer = eventHandler.userPointer
		};

		// Make sure the event handler isn't already present
		auto& eventHandlers = m_details->eventHandlers[static_cast<uint32_t>(eventHandler.event) - 1];
		if (std::erase(eventHandlers, smallHandler) == 0)
			return Result::InvalidParameter;
		
		return Result::Success;
	}
}
