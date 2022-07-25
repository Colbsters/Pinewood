#ifndef PCH_H
#define PCH_H

#if PW_PLATFORM_WINDOWS

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <Windows.h>

#endif // ^^^ PW_PLATFORM_WINDOWS

#if PW_RENDERER_OPENGL4

#include <glad/gl.h>			// OpenGL loader

#if PW_PLATFORM_WINDOWS
#include <glad/wgl.h>		// WGL loader
#endif // ^^^ PW_PLATFORM_WINDOWS

#endif // ^^^ PW_RENDERER_OPENGL4

#include <Pinewood/Core.h>

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <memory>
#include <algorithm>
#include <utility>
#include <limits>

#endif // PCH_H
