#pragma once
#include "pch.h"
#include <Pinewood/Renderer/HL/HLRenderInterface.h>

namespace Pinewood
{
	class HLRenderInterface::Details
	{
	public:
		HLContext context;
		GladGLContext* gl; // So I don't need to get it from the context all the time

		HLShaderProgram program; // Used for uniforms

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

	Result HLRenderInterface::ClearTarget(ClearTargetFlags flags)
	{
		m_details->gl->Clear(
			static_cast<uint32_t>(flags & ClearTargetFlags::Color) ? GL_COLOR_BUFFER_BIT : 0 &
			static_cast<uint32_t>(flags & ClearTargetFlags::Depth) ? GL_DEPTH_BUFFER_BIT : 0 &
			static_cast<uint32_t>(flags & ClearTargetFlags::Stencil) ? GL_STENCIL_BUFFER_BIT : 0);
		return Result::Success;
	}

	Result HLRenderInterface::BindVertexBinding(const HLVertexBinding& vertexBinding)
	{
		// Get a copy that's not const
		auto vb = vertexBinding;

		m_details->gl->BindVertexArray(vb.GetNativeHandle());

		return Result::Success;
	}

	Result HLRenderInterface::BindShaderProgram(const HLShaderProgram& program)
	{
		m_details->program = program;
		m_details->gl->UseProgram(m_details->program.GetNativeHandle());

		return Result::Success;
	}

	Result HLRenderInterface::SetConstantBuffer(uint32_t index, const HLBuffer& buffer)
	{
		// non-const copy
		HLBuffer buf = buffer;

		m_details->gl->BindBufferBase(GL_UNIFORM_BUFFER, index, buf.GetNativeHandle());
		m_details->gl->UniformBlockBinding(m_details->program.GetNativeHandle(), index, index);

		return Result::Success;
	}

	Result HLRenderInterface::SetTexture2D(uint32_t location, uint32_t slot, const HLTexture2D& texture)
	{
		HLTexture2D tex = texture;

		m_details->gl->BindTextureUnit(slot, tex.GetNativeHandle());
		m_details->gl->Uniform1i(location, slot);

		return Result::Success;
	}

	Result HLRenderInterface::Draw(uint32_t startIndex, uint32_t count)
	{
		m_details->gl->DrawArrays(GL_TRIANGLES, startIndex, count);

		return Result::Success;
	}

	Result HLRenderInterface::DrawIndexed(uint32_t count)
	{
		m_details->gl->DrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

		return Result::Success;
	}

	Result HLRenderInterface::SetFramebuffer(const HLFramebuffer& framebuffer)
	{
		m_details->gl->BindFramebuffer(GL_FRAMEBUFFER, HLFramebuffer{ framebuffer }.GetNativeHandle());
		return Result::Success;
	}

	Result HLRenderInterface::ResetFramebuffer()
	{
		m_details->gl->BindFramebuffer(GL_FRAMEBUFFER, 0);
		return Result::Success;
	}

	HLContext HLRenderInterface::GetContext()
	{
		return m_details->context;
	}
	
	bool HLRenderInterface::IsInitialized()
	{
		return m_details && m_details->gl;
	}
}
