#pragma once
#include "pch.h"
#include <Pinewood/Renderer/HL/HLFramebuffer.h>

namespace Pinewood
{
	class HLFramebuffer::Details
	{
	public:
		HLContext context;
		const GladGLContext* gl; // So I don't need to get it from the context all the time
		
		uint32_t framebuffer;

		std::vector<HLTexture2D> textures; // To make sure they don't disappear

		~Details();

		Result Destroy();
	};

	static uint32_t GetGLAttachment(HLFramebufferAttachment attachment)
	{
		constexpr static uint32_t translationTable[] =
		{
			GL_COLOR_ATTACHMENT0,
			GL_COLOR_ATTACHMENT1,
			GL_COLOR_ATTACHMENT2,
			GL_COLOR_ATTACHMENT3,
			GL_COLOR_ATTACHMENT4,
			GL_COLOR_ATTACHMENT5,
			GL_COLOR_ATTACHMENT6,
			GL_COLOR_ATTACHMENT7,
			GL_COLOR_ATTACHMENT8,
			GL_COLOR_ATTACHMENT9,
			GL_COLOR_ATTACHMENT10,
			GL_COLOR_ATTACHMENT11,
			GL_COLOR_ATTACHMENT12,
			GL_COLOR_ATTACHMENT13,
			GL_COLOR_ATTACHMENT14,
			GL_COLOR_ATTACHMENT15,
			GL_DEPTH_STENCIL_ATTACHMENT
		};

		return translationTable[static_cast<uint32_t>(attachment) - 1];
	}

	HLFramebuffer::Details::~Details()
	{
		Destroy();
	}
	
	Result HLFramebuffer::Details::Destroy()
	{
		gl->DeleteFramebuffers(1, &framebuffer);

		gl = nullptr;
		context = HLContext{};
		textures.clear();

		return Result::Success;
	}

	Result HLFramebuffer::Create(const HLFramebufferCreateInfo& createInfo)
	{
		if (createInfo.textures.size() != createInfo.attachments.size())
			return Result::InvalidParameter;

		m_details = std::make_shared<Details>();
		m_details->context = createInfo.context;
		m_details->gl = static_cast<const GladGLContext*>(m_details->context.GetNativeHandle().gl);

		m_details->gl->CreateFramebuffers(1, &m_details->framebuffer);

		for (uint32_t i = 0; i < createInfo.textures.size(); i++)
			m_details->gl->NamedFramebufferTexture(m_details->framebuffer,
				GetGLAttachment(createInfo.attachments[i]), createInfo.textures[i].GetNativeHandle(), 0);

		std::copy(createInfo.textures.begin(), createInfo.textures.end(), std::back_inserter(m_details->textures));

		if (m_details->gl->CheckNamedFramebufferStatus(m_details->framebuffer, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			return Result::SystemError;

		return Result::Success;
	}

	Result HLFramebuffer::Destroy()
	{
		return m_details->Destroy();
	}

	HLFramebuffer::NativeHandle HLFramebuffer::GetNativeHandle()
	{
		return m_details->framebuffer;
	}
	
	bool HLFramebuffer::IsInitialized()
	{
		return m_details && m_details->gl;
	}
}
