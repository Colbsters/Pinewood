#pragma once
#include "pch.h"
#include <Pinewood/Renderer/HLContext.h>

// Used for creating a dummy context
#include <Pinewood/Window.h>

#include <mutex>

namespace Pinewood
{
	static std::mutex g_wglLoadMutex;
	static bool g_wglLoaded;

	thread_local static HLContext* g_currentContext;

	static GLADapiproc glLoadFunc(const char* name)
	{
		static HMODULE glInstance;

		if (auto proc = wglGetProcAddress(name))
			return reinterpret_cast<GLADapiproc>(proc);

		if (!glInstance)
		{
			glInstance = LoadLibraryW(L"opengl32.dll");
			if (!glInstance)
				return static_cast<GLADapiproc>(nullptr);
		}

		return reinterpret_cast<GLADapiproc>(GetProcAddress(glInstance, name));
	}

	Result HLContext::InitializeWGL()
	{
		Result result;

		// Create a dummy window
		Window window;
		result = window.Create({
			.title = "WGL dummy context window",
			.flags = 0
		});
		if (IsError(result))
			return result;

		// Get the device context
		HWND windowHandle = static_cast<HWND>(window.GetNativeHandle());
		HDC deviceContext = GetDC(windowHandle);
		if (!deviceContext)
			return Result::SystemError;

		// Set a pixel format (hopefully hardware accelerated)
		PIXELFORMATDESCRIPTOR pixelFormatDesc {
			.nSize = sizeof(PIXELFORMATDESCRIPTOR),
			.nVersion = 1,
			.dwFlags = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL,
			.iPixelType = PFD_TYPE_RGBA,
			.cColorBits = 32,
			.cDepthBits = 24,
			.cStencilBits = 8,
			.cAuxBuffers = 0,
			.iLayerType = PFD_MAIN_PLANE
		};

		int pixelFormat = ChoosePixelFormat(deviceContext, &pixelFormatDesc);
		if (!pixelFormat)
			return Result::SystemError;

		if (!SetPixelFormat(deviceContext, pixelFormat, &pixelFormatDesc))
			return Result::SystemError;

		// Create the dummy context and make it current
		HGLRC renderContext = wglCreateContext(deviceContext);
		if (!renderContext)
			return Result::SystemError;

		// I don't need to mark this context as current as no other context will be invalidated by it (no other context have been created yet)
		if (!wglMakeCurrent(deviceContext, renderContext))
		{
			wglDeleteContext(renderContext);
			return Result::SystemError;
		}

		// Load WGL
		gladLoadWGL(deviceContext, reinterpret_cast<GLADloadfunc>(wglGetProcAddress));

		// Make the context obsolete and delete it
		if (!wglMakeCurrent(nullptr, nullptr))
		{
			wglDeleteContext(renderContext);
			return Result::SystemError;
		}

		if (!wglDeleteContext(renderContext))
			return Result::SystemError;

		// Destroy the window (and make sure to update it so the messages clear)
		window.Destroy();
		window.Update();

		return Result::Success;
	}

	Result HLContext::MakeCurrent()
	{
		// The context is already current, don't need to do anything
		if (g_currentContext == this)
			return Result::Success;

		// If an error occurs, the previous context will be made invalid
		g_currentContext = nullptr;

		// Try to make it current
		if (!wglMakeCurrent(static_cast<HDC>(m_deviceContext), static_cast<HGLRC>(m_renderContext)))
			return Result::SystemError;

		// The context was successfully made current
		g_currentContext = this;

		return Result::Success;
	}

	Result HLContext::MakeObsolete()
	{
		// The context is already current, don't need to do anything
		if (g_currentContext == nullptr)
			return Result::Success;

		// If an error occurs, the previous context will be made invalid anyways
		g_currentContext = nullptr;

		// Try to make it current
		if (!wglMakeCurrent(nullptr, nullptr))
			return Result::SystemError;

		return Result::Success;
	}

	void __stdcall HLContext::DebugMessageCallback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int32_t length, const char* message, const void* userParam)
	{
		const char* messageSource;
		const char* messageType;
		const char* messageSeverity;
		bool isSevere = false;

		switch (source)
		{
		case GL_DEBUG_SOURCE_API:
			messageSource = "OpenGL";
			break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			messageSource = "Window System";
			break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			messageSource = "Shader Compiler";
			break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			messageSource = "Third Party";
			break;
		case GL_DEBUG_SOURCE_APPLICATION:
			messageSource = "Application";
			break;
		case GL_DEBUG_SOURCE_OTHER:
			messageSource = "Other";
			break;
		default:
			messageSource = "Unknown";
		}

		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR:
			messageType = "Error";
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			messageType = "Deprecated Behavior";
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			messageType = "Undefined Behavior";
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			messageType = "Portability";
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			messageType = "Performance";
			break;
		case GL_DEBUG_TYPE_MARKER:
			messageType = "Marker";
			break;
		case GL_DEBUG_TYPE_PUSH_GROUP:
			messageType = "Push Group";
			break;
		case GL_DEBUG_TYPE_POP_GROUP:
			messageType = "Pop Group";
			break;
		case GL_DEBUG_TYPE_OTHER:
			messageType = "Other";
			break;
		default:
			messageType = "Unknown";
		}

		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:
			messageSeverity = "High";
			isSevere = true;
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			messageSeverity = "Medium";
			isSevere = true;
			break;
		case GL_DEBUG_SEVERITY_LOW:
			messageSeverity = "Low";
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			messageSeverity = "Notification";
			break;
		default:
			messageSeverity = "Unknown";
		}

		std::printf("OpenGL Error (source = %s, type = %s, id = %u, severity = %s):\n%s\n\n", messageSource, messageType, id, messageSeverity, message);
	}

	HLContext::~HLContext()
	{
		Destroy();
	}

	Result HLContext::Create(const HLContextCreateInfo& createInfo)
	{
		{ // Load WGL if needed
			std::lock_guard<std::mutex> lock{ g_wglLoadMutex };
			if (!g_wglLoaded)
			{
				auto result = InitializeWGL();
				if (IsError(result))
					return result;

				g_wglLoaded = true;
			}
		}

		// Get the device context
		Window window = createInfo.window;
		HWND windowHandle = static_cast<HWND>(window.GetNativeHandle());
		HDC deviceContext = GetDC(windowHandle);
		if (!deviceContext)
			return Result::SystemError;

		constexpr int pixelFormatAttribs[] {
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB, 32,
			WGL_DEPTH_BITS_ARB, 24,
			WGL_STENCIL_BITS_ARB, 8,
			WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
			0, // End
		};
		
		int pixelFormat;
		uint32_t numFormats;
		if (!wglChoosePixelFormatARB(deviceContext, pixelFormatAttribs, nullptr, 1, &pixelFormat, &numFormats))
			return Result::SystemError;

		PIXELFORMATDESCRIPTOR pixelFormatDesc;
		if (!DescribePixelFormat(deviceContext, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pixelFormatDesc))
			return Result::SystemError;

		if (!SetPixelFormat(deviceContext, pixelFormat, &pixelFormatDesc))
			return Result::SystemError;

		constexpr int contextAttribs[]{
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 5,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
#if PW_DEBUG
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
#endif // ^^^ PW_DEBUG
			0, // End
		};

		HGLRC renderContext = wglCreateContextAttribsARB(deviceContext, nullptr, contextAttribs);
		if (!renderContext)
			return Result::SystemError;

		m_renderContext = renderContext;
		m_deviceContext = deviceContext;

		Result result = MakeCurrent();
		if (IsError(result))
		{
			wglDeleteContext(renderContext); // Cleanup the context
			return result;
		}

		GladGLContext* gl = new GladGLContext;
		if (!gladLoadGLContext(gl, glLoadFunc))
		{
			// Clean up
			delete gl;
			MakeObsolete();
			wglDeleteContext(renderContext);

			return Result::UnknownError;
		}
		m_gl = gl;

		gl->Enable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		gl->DebugMessageCallback(DebugMessageCallback, this);
#if !PW_DEBUG
		gl->DebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW, 0, nullptr, GL_FALSE);
		gl->DebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif // ^^^ !PW_DEBUG

		result = SetSwapInterval(createInfo.swapInterval);
		if (IsError(result))
			return result;

		return Result::Success;
	}
	
	Result HLContext::Destroy()
	{
		if ((!m_gl) || (!m_renderContext))
			return Result::NotInitialized;

		delete static_cast<GladGLContext*>(m_gl);

		MakeObsolete();
		wglDeleteContext(static_cast<HGLRC>(m_renderContext));

		// Prevent double free
		m_gl = nullptr;
		m_renderContext = nullptr;
		m_deviceContext = nullptr;

		return Result::Success;
	}
	
	Result HLContext::SwapBuffers()
	{
		if (!::SwapBuffers(static_cast<HDC>(m_deviceContext)))
			return Result::SystemError;

		return Result::Success;
	}

	Result HLContext::SetSwapInterval(uint32_t interval)
	{
		if (!wglSwapIntervalEXT(interval))
			return Result::SystemError;

		return Result::Success;
	}

	inline Result HLContext::ResizeSwapChain(uint16_t width, uint32_t height)
	{
		const auto* gl = static_cast<GladGLContext*>(m_gl);

		gl->Viewport(0, 0, width, height);

		if (gl->IsEnabled(GL_SCISSOR_TEST))
			gl->Scissor(0, 0, width, height);

		return Result::Success;
	}

	HLContext::NativeHandle HLContext::GetNativeHandle()
	{
		return NativeHandle{ m_renderContext, m_gl };
	}
}
