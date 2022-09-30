#pragma once
#include "pch.h"
#include <Pinewood/Renderer/HLTexture2D.h>

namespace Pinewood
{
	struct GLFormat { uint32_t baseFormat, sizeFormat; };
	static constexpr GLFormat GetGLFormat(HLImageFormat format)
	{
		constexpr GLFormat formatTable[]
		{
			{ 0, 0 }, // Unknown
			{ GL_RED,	GL_R8				}, // R8_UNorm
			{ GL_RG,	GL_RG8				}, // R8G8_UNorm
			{ GL_RGB,	GL_RGB8				}, // R8G8B8_UNorm
			{ GL_RGBA,	GL_RGBA8			}, // R8G8B8A8_UNorm
			{ GL_RED,	GL_R8_SNORM			}, // R8_SNorm
			{ GL_RG,	GL_RG8_SNORM		}, // R8G8_SNorm
			{ GL_RGB,	GL_RGB8_SNORM		}, // R8G8B8_SNorm
			{ GL_RGBA,	GL_RGBA8_SNORM		}, // R8G8B8A8_SNorm
			{ GL_RED,	GL_R8UI				}, // R8_UInt
			{ GL_RG,	GL_RG8UI			}, // R8G8_UInt
			{ GL_RGB,	GL_RGB8UI			}, // R8G8B8_UInt
			{ GL_RGBA,	GL_RGBA8UI			}, // R8G8B8A8_UInt
			{ GL_RED,	GL_R8I				}, // R8_SInt
			{ GL_RG,	GL_RG8I				}, // R8G8_SInt
			{ GL_RGB,	GL_RGB8I			}, // R8G8B8_SInt
			{ GL_RGBA,	GL_RGBA8I			}, // R8G8B8A8_SInt
			{ GL_RED,	GL_R16				}, // R16_UNorm
			{ GL_RG,	GL_RG16				}, // R16G16_UNorm
			{ GL_RGB,	GL_RGB16			}, // R16G16B16_UNorm
			{ GL_RGBA,	GL_RGBA16			}, // R16G16B16A16_UNorm
			{ GL_RED,	GL_R16_SNORM		}, // R16_SNorm
			{ GL_RG,	GL_RG16_SNORM		}, // R16G16_SNorm
			{ GL_RGB,	GL_RGB16_SNORM		}, // R16G16B16_SNorm
			{ GL_RGBA,	GL_RGBA16_SNORM		}, // R16G16B16A16_SNorm
			{ GL_RED,	GL_R16UI			}, // R16_UInt
			{ GL_RG,	GL_RG16UI			}, // R16G16_UInt
			{ GL_RGB,	GL_RGB16UI			}, // R16G16B16_UInt
			{ GL_RGBA,	GL_RGBA16UI			}, // R16G16B16A16_UInt
			{ GL_RED,	GL_R16I				}, // R16_SInt
			{ GL_RG,	GL_RG16I			}, // R16G16_SInt
			{ GL_RGB,	GL_RGB16I			}, // R16G16B16_SInt
			{ GL_RGBA,	GL_RGBA16I			}, // R16G16B16A16_SInt
			{ GL_RED,	GL_R16F				}, // R16_Float
			{ GL_RG,	GL_RG16F			}, // R16G16_Float
			{ GL_RGB,	GL_RGB16F			}, // R16G16B16_Float
			{ GL_RGBA,	GL_RGBA16F			}, // R16G16B16A16_Float
			{ GL_RED,	GL_R32UI			}, // R32_UInt
			{ GL_RG,	GL_RG32UI			}, // R32G32_UInt
			{ GL_RGB,	GL_RGB32UI			}, // R32G32B32_UInt
			{ GL_RGBA,	GL_RGBA32UI			}, // R32G32B32A32_UInt
			{ GL_RED,	GL_R32I				}, // R32_SInt
			{ GL_RG,	GL_RG32I			}, // R32G32_SInt
			{ GL_RGB,	GL_RGB32I			}, // R32G32B32_SInt
			{ GL_RGBA,	GL_RGBA32I			}, // R32G32B32A32_SInt
			{ GL_RED,	GL_R32F				}, // R32_Float
			{ GL_RG,	GL_RG32F			}, // R32G32_Float
			{ GL_RGB,	GL_RGB32F			}, // R32G32B32_Float
			{ GL_RGBA,	GL_RGBA32F			}, // R32G32B32A32_Float
			{ GL_RGB,	GL_RGB9_E5			}, // R9G9B9E5_SExp
			{ GL_RGBA,	GL_RGB10_A2			}, // R10G10B10A2_UNORM
			{ GL_RGBA,	GL_RGB10_A2UI		}, // R10G10B10A2_UInt
			{ GL_RGB,	GL_R11F_G11F_B10F	}, // R11G11B10_Float
		};

		// Use the look-up table, and don't forget to do bounds checking
		return formatTable[static_cast<uint32_t>(format) < (sizeof(formatTable) / sizeof(GLFormat)) ? static_cast<uint32_t>(format) : 0];
	}

	static constexpr GLFormat GetGLFormat2(HLImageFormat format)
	{
		constexpr GLFormat formatTable[]
		{
			{ 0, 0 }, // Unknown
			{ GL_RED,	GL_UNSIGNED_BYTE	}, // R8_UNorm
			{ GL_RG,	GL_UNSIGNED_BYTE	}, // R8G8_UNorm
			{ GL_RGB,	GL_UNSIGNED_BYTE	}, // R8G8B8_UNorm
			{ GL_RGBA,	GL_UNSIGNED_BYTE	}, // R8G8B8A8_UNorm
			{ GL_RED,	GL_BYTE				}, // R8_SNorm
			{ GL_RG,	GL_BYTE				}, // R8G8_SNorm
			{ GL_RGB,	GL_BYTE				}, // R8G8B8_SNorm
			{ GL_RGBA,	GL_BYTE				}, // R8G8B8A8_SNorm
			{ GL_RED,	GL_UNSIGNED_BYTE	}, // R8_UInt
			{ GL_RG,	GL_UNSIGNED_BYTE	}, // R8G8_UInt
			{ GL_RGB,	GL_UNSIGNED_BYTE	}, // R8G8B8_UInt
			{ GL_RGBA,	GL_UNSIGNED_BYTE	}, // R8G8B8A8_UInt
			{ GL_RED,	GL_BYTE				}, // R8_SInt
			{ GL_RG,	GL_BYTE				}, // R8G8_SInt
			{ GL_RGB,	GL_BYTE				}, // R8G8B8_SInt
			{ GL_RGBA,	GL_BYTE				}, // R8G8B8A8_SInt
			{ GL_RED,	GL_UNSIGNED_SHORT	}, // R16_UNorm
			{ GL_RG,	GL_UNSIGNED_SHORT	}, // R16G16_UNorm
			{ GL_RGB,	GL_UNSIGNED_SHORT	}, // R16G16B16_UNorm
			{ GL_RGBA,	GL_UNSIGNED_SHORT	}, // R16G16B16A16_UNorm
			{ GL_RED,	GL_SHORT			}, // R16_SNorm
			{ GL_RG,	GL_SHORT			}, // R16G16_SNorm
			{ GL_RGB,	GL_SHORT			}, // R16G16B16_SNorm
			{ GL_RGBA,	GL_SHORT			}, // R16G16B16A16_SNorm
			{ GL_RED,	GL_UNSIGNED_SHORT	}, // R16_UInt
			{ GL_RG,	GL_UNSIGNED_SHORT	}, // R16G16_UInt
			{ GL_RGB,	GL_UNSIGNED_SHORT	}, // R16G16B16_UInt
			{ GL_RGBA,	GL_UNSIGNED_SHORT	}, // R16G16B16A16_UInt
			{ GL_RED,	GL_SHORT			}, // R16_SInt
			{ GL_RG,	GL_SHORT			}, // R16G16_SInt
			{ GL_RGB,	GL_SHORT			}, // R16G16B16_SInt
			{ GL_RGBA,	GL_SHORT			}, // R16G16B16A16_SInt
			{ GL_RED,	GL_HALF_FLOAT		}, // R16_Float
			{ GL_RG,	GL_HALF_FLOAT		}, // R16G16_Float
			{ GL_RGB,	GL_HALF_FLOAT		}, // R16G16B16_Float
			{ GL_RGBA,	GL_HALF_FLOAT		}, // R16G16B16A16_Float
			{ GL_RED,	GL_UNSIGNED_INT		}, // R32_UInt
			{ GL_RG,	GL_UNSIGNED_INT		}, // R32G32_UInt
			{ GL_RGB,	GL_UNSIGNED_INT		}, // R32G32B32_UInt
			{ GL_RGBA,	GL_UNSIGNED_INT		}, // R32G32B32A32_UInt
			{ GL_RED,	GL_INT				}, // R32_SInt
			{ GL_RG,	GL_INT				}, // R32G32_SInt
			{ GL_RGB,	GL_INT				}, // R32G32B32_SInt
			{ GL_RGBA,	GL_INT				}, // R32G32B32A32_SInt
			{ GL_RED,	GL_FLOAT			}, // R32_Float
			{ GL_RG,	GL_FLOAT			}, // R32G32_Float
			{ GL_RGB,	GL_FLOAT			}, // R32G32B32_Float
			{ GL_RGBA,	GL_FLOAT			}, // R32G32B32A32_Float
			{ GL_RGB,	GL_FLOAT			}, // R9G9B9E5_SExp
			{ GL_RGBA,	GL_FLOAT			}, // R10G10B10A2_UNORM
			{ GL_RGBA,	GL_FLOAT			}, // R10G10B10A2_UInt
			{ GL_RGB,	GL_FLOAT			}, // R11G11B10_Float
		};

		// Use the look-up table, and don't forget to do bounds checking
		return formatTable[static_cast<uint32_t>(format) < (sizeof(formatTable) / sizeof(GLFormat)) ? static_cast<uint32_t>(format) : 0];
	}

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
		auto glFormat = GetGLFormat(createInfo.format);

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
		return m_details->Destroy();
	}

	Result HLTexture2D::SetImage(const void* data, uint32_t mipLevel, uint32_t xOffset, uint32_t yOffset, uint32_t width, uint32_t height)
	{
		auto glFormat = GetGLFormat2(m_details->format);
		m_details->gl->TextureSubImage2D(m_details->texture, mipLevel, xOffset, yOffset, width, height, glFormat.baseFormat, glFormat.sizeFormat, data);

		return Result::Success;
	}

	Result HLTexture2D::GetImage(void* data, size_t bufferSize, uint32_t mipLevel, uint32_t xOffset, uint32_t yOffset, uint32_t width, uint32_t height)
	{
		auto glFormat = GetGLFormat2(m_details->format);
		m_details->gl->GetTextureSubImage(m_details->texture, mipLevel, xOffset, yOffset, 0, width, height, 0, glFormat.baseFormat, glFormat.sizeFormat, bufferSize, data);

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
