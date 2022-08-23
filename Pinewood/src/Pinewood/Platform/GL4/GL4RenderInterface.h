#pragma once
#include "pch.h"
#include <Pinewood/Renderer/HLRenderInterface.h>

namespace Pinewood
{
	class HLRenderInterface::Details
	{
	public:
		HLContext context;
		GladGLContext* gl; // So I don't need to get it from the context all the time

		~Details();

		Result Destroy();
	};

	HLRenderInterface::Details::~Details()
	{
		Destroy();
	}
	
	Result HLRenderInterface::Details::Destroy()
	{
		gl = nullptr;
		context = HLContext{};

		return Result::Success;
	}

	Result HLRenderInterface::Create(const HLRenderInterfaceCreateInfo& createInfo)
	{
		// Save the context and cache the gl functions
		m_details = std::make_shared<Details>();
		m_details->context = createInfo.context;
		m_details->gl = static_cast<GladGLContext*>(m_details->context.GetNativeHandle().gl);

		return Result::Success;
	}

	Result HLRenderInterface::Destroy()
	{
		return m_details->Destroy();
	}

	Result HLRenderInterface::SetClearColor(const float(&color)[4])
	{
		m_details->gl->ClearColor(color[0], color[1], color[2], color[3]);
		return Result::Success;
	}

	Result HLRenderInterface::SetClearDepth(float depth)
	{
		m_details->gl->ClearDepthf(depth);
		return Result::Success;
	}

	Result HLRenderInterface::SetClearStencil(uint32_t stencil)
	{
		m_details->gl->ClearStencil(stencil);
		return Result::Success;
	}

	Result HLRenderInterface::ClearTarget(uint32_t flags)
	{
		m_details->gl->Clear(
			(flags & ClearTargetFlags::Color) ? GL_COLOR_BUFFER_BIT : 0 &
			(flags & ClearTargetFlags::Depth) ? GL_DEPTH_BUFFER_BIT : 0 &
			(flags & ClearTargetFlags::Stencil) ? GL_STENCIL_BUFFER_BIT : 0);
		return Result::Success;
	}

	HLContext HLRenderInterface::GetContext()
	{
		return m_details->context;
	}
}
