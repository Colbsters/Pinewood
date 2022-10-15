#pragma once
#include <Pinewood/Core.h>
#include <Pinewood/Error.h>
#include <Pinewood/Renderer/HL/HLContext.h>
#include <Pinewood/Renderer/HL/HLTexture2D.h>

namespace Pinewood
{
	enum class HLFramebufferAttachment
	{
		Null = 0,
		Color0,
		Color1,
		Color2,
		Color3,
		Color4,
		Color5,
		Color6,
		Color7,
		Color8,
		Color9,
		Color10,
		Color11,
		Color12,
		Color13,
		Color14,
		Color15,
		DepthStencil
	};

	struct HLFramebufferCreateInfo
	{
		HLContext context;
		std::span<HLTexture2D> textures;
		std::span<HLFramebufferAttachment> attachments;
	};

	class HLFramebuffer
	{
	public:
		using NativeHandle = uint32_t;

		HLFramebuffer() = default;
		HLFramebuffer(const HLFramebuffer&) = default;
		HLFramebuffer(HLFramebuffer&& rhs) noexcept :m_details(std::move(rhs.m_details)) {}
		~HLFramebuffer() = default;

		HLFramebuffer& operator=(const HLFramebuffer&) = default;
		HLFramebuffer& operator=(HLFramebuffer&& rhs) noexcept { m_details = std::move(rhs.m_details); return *this; }

		Result Create(const HLFramebufferCreateInfo& createInfo);
		Result Destroy();

		NativeHandle GetNativeHandle();

		bool IsInitialized();

	private:
		class Details;

		HLFramebuffer(std::shared_ptr<Details> details) :m_details(details) {}

		std::shared_ptr<Details> m_details;
	};
}
