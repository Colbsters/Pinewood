#ifndef PCH_H
#define PCH_H

#if PW_PLATFORM_WINDOWS

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <Windows.h>

#endif // ^^^ PW_PLATFORM_WINDOWS

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
