#pragma once
#include <Pinewood/Core.h>
#include <Pinewood/Error.h>
#include <Pinewood/Renderer/HL/HLContext.h>
#include <Pinewood/Renderer/HL/HLImageFormat.h>

namespace Pinewood
{
	enum class HLTextureFilter
	{
		Null,
		Nearest,
		Linear
	};

	enum class HLTextureWrapMode
	{
		Null,
		Black,
		White,
		Repeat,
		MirroredRepeat,
		ClampToEdge
	};

	struct HLTexture2DArrayCreateInfo
	{
		HLContext context;
		uint32_t width, height;
		uint32_t count = 1;			// Number of Textures (defaults to 1)
		uint32_t mipLevels = 0;		// 0 means generate a full mip chain (defaults to 1)
		HLTextureFilter sampleFilter = HLTextureFilter::Linear;
		HLTextureWrapMode wrapMode = HLTextureWrapMode::Repeat;
		HLImageFormat format;
		const void* data;			// Can be nullptr
	};

	class HLTexture2DArray
	{
	public:
		using NativeHandle = uint32_t;

		HLTexture2DArray() = default;
		HLTexture2DArray(const HLTexture2DArray&) = default;
		HLTexture2DArray(HLTexture2DArray&& rhs) noexcept :m_details(std::move(rhs.m_details)) {}
		~HLTexture2DArray() = default;

		HLTexture2DArray& operator=(const HLTexture2DArray&) = default;
		HLTexture2DArray& operator=(HLTexture2DArray&& rhs) noexcept { m_details = std::move(rhs.m_details); return *this; }

		Result Create(const HLTexture2DArrayCreateInfo& createInfo);
		Result Destroy();

		// Sets the data in the buffer.
		// Params:
		//  - data = A pointer to the data to send to the buffer.
		//  - offset = The offset into the GPU's buffer.
		//  - size = The number of bytes to send to the GPU.
		Result SetImage(const void* data, uint32_t mipLevel, uint32_t xOffset, uint32_t yOffset, uint32_t width, uint32_t height, uint32_t startIndex, uint32_t numTextures);

		// Gets data from the buffer.
		// Params:
		//  - data = A pointer to where the data will be placed.
		//  - offset = The offset into the GPU's buffer.
		//  - size = The number of bytes to read from the GPU.
		Result GetImage(void* data, size_t bufferSize, uint32_t mipLevel, uint32_t xOffset, uint32_t yOffset, uint32_t width, uint32_t height, uint32_t startIndex, uint32_t numTextures);
		
		// Generates the mips in the mip chain
		Result GenerateMips();

		NativeHandle GetNativeHandle();

		bool IsInitialized();

	private:
		class Details;

		HLTexture2DArray(std::shared_ptr<Details> details) :m_details(details) {}

		std::shared_ptr<Details> m_details;
	};
}
