#pragma once

#include <Pinewood/Core.h>
#include <Pinewood/Error.h>
#include <Pinewood/Window.h>
#include <Pinewood/Input.h>
#include <Pinewood/InputX.h>

#if PW_RENDERER_OPENGL4
#include <Pinewood/Renderer/HL/HLContext.h>
#include <Pinewood/Renderer/HL/HLRenderInterface.h>
#include <Pinewood/Renderer/HL/HLBuffer.h>
#include <Pinewood/Renderer/HL/HLLayout.h>
#include <Pinewood/Renderer/HL/HLVertexBinding.h>
#include <Pinewood/Renderer/HL/HLShaderModule.h>
#include <Pinewood/Renderer/HL/HLShaderProgram.h>
#include <Pinewood/Renderer/HL/HLTexture2D.h>
#include <Pinewood/Renderer/HL/HLFramebuffer.h>
#endif // ^^^ PW_RENDERER_OPENGL4
