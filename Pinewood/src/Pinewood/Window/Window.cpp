#include "pch.h"

#if PW_PLATFORM_WINDOWS
#include "../Platform/Win32/Win32Window.h"
#else // ^^^ PW_PLATFORM_WINDOWS // Unsupported platform vvv
#error "No valid/supported plaform was selected"
#endif // ^^^ Unsupported platform
