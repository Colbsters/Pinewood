#include "pch.h"

#if PW_RENDERER_OPENGL4
#include "../../Platform/GL4/GL4RenderInterface.h"
#else // ^^^ PW_RENDERER_OPENGL4 // Unsupported API vvv
#error "3No valid/supported rendering API was selected"
#endif
