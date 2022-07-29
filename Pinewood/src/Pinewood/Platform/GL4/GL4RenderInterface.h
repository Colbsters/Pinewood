#pragma once
#include "pch.h"
#include <Pinewood/Renderer/HLRenderInterface.h>

namespace Pinewood
{
	HLRenderInterface::~HLRenderInterface()
	{
		Destroy();
	}

	Result HLRenderInterface::Create(const HLRenderInterfaceCreateInfo& createInfo)
	{
		// Save the context and cache the gl functions
		m_context = createInfo.context;
		m_gl = m_context->GetNativeHandle().gl;

		return Result::Success;
	}

	Result HLRenderInterface::Destroy()
	{
		m_gl = nullptr;
		m_context = nullptr;

		return Result::Success;
	}

	Result HLRenderInterface::SetClearColor(const float(&color)[4])
	{
		static_cast<GladGLContext*>(m_gl)->ClearColor(color[0], color[1], color[2], color[3]);
		return Result::Success;
	}

	Result HLRenderInterface::SetClearDepth(float depth)
	{
		static_cast<GladGLContext*>(m_gl)->ClearDepthf(depth);
		return Result::Success;
	}

	Result HLRenderInterface::SetClearStencil(uint32_t stencil)
	{
		static_cast<GladGLContext*>(m_gl)->ClearStencil(stencil);
		return Result::Success;
	}

	Result HLRenderInterface::ClearTarget(uint32_t flags)
	{
		static_cast<GladGLContext*>(m_gl)->Clear(
			(flags & ClearTargetFlags::Color) ? GL_COLOR_BUFFER_BIT : 0 &
			(flags & ClearTargetFlags::Depth) ? GL_DEPTH_BUFFER_BIT : 0 &
			(flags & ClearTargetFlags::Stencil) ? GL_STENCIL_BUFFER_BIT : 0);
		return Result::Success;
	}

	std::shared_ptr<HLContext> HLRenderInterface::GetContext()
	{
		return m_context;
	}
}
