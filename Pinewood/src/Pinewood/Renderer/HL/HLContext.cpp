#include "pch.h"

#ifdef PW_RENDERER_OPENGL4
#include "../../Platform/WGL/WGLContext.h"
#else // ^^^ PW_RENDERER_OPENGL4 // Unsupported API vvv
#error "No valid/supported rendering API was selected"
#endif // ^^^ Unsupported API
