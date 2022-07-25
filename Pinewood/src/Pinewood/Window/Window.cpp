#include "pch.h"

#if PW_PLATFORM_WINDOWS
#include "../Platform/Win32/Win32Window.h"
#else // ^^^ PW_PLATFORM_WINDOWS // Unsported platform vvv
#error "No valid/supported plaform was selected"
#endif // ^^^ Unsported platform
