#pragma once
#include "pch.h"
#include "TextureCommon.h"

#include <Pinewood/Renderer/HL/HLTexture2D.h>

namespace Pinewood
{
	class HLTexture2D::Details
	{
	public:
		HLContext context;
		const GladGLContext* gl; // So I don't need to get it from the context all the time
		
		HLImageFormat format;
		uint32_t texture;

		~Details();

		Result Destroy();
	};

	HLTexture2D::Details::~Details()
	{
		Destroy();
	}
	
	Result HLTexture2D::Details::Destroy()
	{
		gl->DeleteTextures(1, &texture);

		gl = nullptr;
		context = HLContext{};

		return Result::Success;
	}

	Result HLTexture2D::Create(const HLTexture2DCreateInfo& createInfo)
	{
		m_details = std::make_shared<Details>();
		m_details->context = createInfo.context;
		m_details->gl = static_cast<const GladGLContext*>(m_details->context.GetNativeHandle().gl);
		m_details->format = createInfo.format;
		auto glFormat = Impl::GetGLFormat(createInfo.format);

		m_details->gl->CreateTextures(GL_TEXTURE_2D, 1, &m_details->texture);

		// Set the wrap mode
		switch (createInfo.wrapMode)
		{
		case HLTextureWrapMode::White:
		{
			float color[4] { 1.0f, 1.0f, 1.0f, 1.0f };
			m_details->gl->TextureParameterfv(m_details->texture, GL_TEXTURE_BORDER_COLOR, color);
			[[fallthrough]];
		}
		case HLTextureWrapMode::Black:
			// The border color is black if it wasn't set (i.e. we didn't set it to white)
			m_details->gl->TextureParameteri(m_details->texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			m_details->gl->TextureParameteri(m_details->texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			break;
		case HLTextureWrapMode::Repeat:
			m_details->gl->TextureParameteri(m_details->texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
			m_details->gl->TextureParameteri(m_details->texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
			break;
		case HLTextureWrapMode::MirroredRepeat:
			m_details->gl->TextureParameteri(m_details->texture, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			m_details->gl->TextureParameteri(m_details->texture, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			break;
		case HLTextureWrapMode::ClampToEdge:
			m_details->gl->TextureParameteri(m_details->texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			m_details->gl->TextureParameteri(m_details->texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			break;
		default:
			return Result::InvalidParameter;
		}

		// Sampling filter
		switch (createInfo.sampleFilter)
		{
		case HLTextureFilter::Nearest:
			m_details->gl->TextureParameteri(m_details->texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			m_details->gl->TextureParameteri(m_details->texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		case HLTextureFilter::Linear:
			m_details->gl->TextureParameteri(m_details->texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			m_details->gl->TextureParameteri(m_details->texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
		default:
			return Result::InvalidParameter;
		}

		// Get the number of mipLevels if 0 was specified
		// Note: 'std::bit_width(x)' is similar to 'std::floor(std::log2(x))', but for integers
		uint32_t mipLevels = (createInfo.mipLevels == 0) ? std::bit_width(std::max(createInfo.width, createInfo.height)) : createInfo.mipLevels;
		
		m_details->gl->TextureStorage2D(m_details->texture, mipLevels, glFormat.sizeFormat, createInfo.width, createInfo.height);

		if (createInfo.data)
			SetImage(createInfo.data, 0, 0, 0, createInfo.width, createInfo.height);

		return Result::Success;
	}

	Result HLTexture2D::Destroy()
	{
		auto result = m_details->Destroy();
		m_details = nullptr;
		return result;
	}

	Result HLTexture2D::SetImage(const void* data, uint32_t mipLevel, uint32_t xOffset, uint32_t yOffset, uint32_t width, uint32_t height)
	{
		auto glFormat = Impl::GetGLFormat2(m_details->format);
		m_details->gl->TextureSubImage2D(m_details->texture, mipLevel, xOffset, yOffset, width, height, glFormat.baseFormat, glFormat.sizeFormat, data);

		return Result::Success;
	}

	Result HLTexture2D::GetImage(void* data, size_t bufferSize, uint32_t mipLevel, uint32_t xOffset, uint32_t yOffset, uint32_t width, uint32_t height)
	{
		auto glFormat = Impl::GetGLFormat2(m_details->format);
		m_details->gl->GetTextureSubImage(m_details->texture, mipLevel, xOffset, yOffset, 0, width, height, 0, glFormat.baseFormat, glFormat.sizeFormat, static_cast<GLsizei>(bufferSize), data);

		return Result::Success;
	}

	Result HLTexture2D::GenerateMips()
	{
		m_details->gl->GenerateTextureMipmap(m_details->texture);

		return Result::Success;
	}

	HLTexture2D::NativeHandle HLTexture2D::GetNativeHandle()
	{
		return m_details->texture;
	}
	
	bool HLTexture2D::IsInitialized()
	{
		return m_details && m_details->gl;
	}
}
