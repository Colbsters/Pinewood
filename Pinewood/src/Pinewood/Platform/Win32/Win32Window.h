#pragma once
#include <Pinewood/Window.h>
#include <thread>
#include <mutex>
#include <cwchar>

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

	class Window::Details
		:std::enable_shared_from_this<Window::Details>
	{
	public:
		HWND window;
		bool isRunning;

		Result CreateWindowImpl(const WindowCreateInfo& createInfo);
		Result CreateWindowAsync(const WindowCreateInfo& createInfo);
		Result CreateWindowAsync(const WindowCreateInfo& createInfo, Window* ptr);
		static void AsyncWindowThread();
		static intptr_t CALLBACK WindowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
	};

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
				// TODO: More message handling
			case WM_DESTROY:
				PostQuitMessage(0);
				windowDetails->isRunning = false;
				g_numWindows--;
				break;
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
}
