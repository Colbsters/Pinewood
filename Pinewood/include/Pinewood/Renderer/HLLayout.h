#pragma once

#include <Pinewood/Core.h>
#include <Pinewood/Error.h>
#include <Pinewood/Renderer/HLContext.h>

namespace Pinewood
{
	enum class HLLayoutElementType : uint32_t
	{
		UInt8		= 0x00,
		Vector2U8	= 0x01,
		Vector3U8	= 0x02,
		Vector4U8	= 0x03,

		UInt16		= 0x10,
		Vector2U16	= 0x11,
		Vector3U16	= 0x12,
		Vector4U16	= 0x13,

		UInt32		= 0x20,
		Vector2U32	= 0x21,
		Vector3U32	= 0x22,
		Vector4U32	= 0x23,

		Int8		= 0x40,
		Vector2I8	= 0x41,
		Vector3I8	= 0x42,
		Vector4I8	= 0x43,

		Int16		= 0x50,
		Vector2I16	= 0x51,
		Vector3I16	= 0x52,
		Vector4I16	= 0x53,

		Int32		= 0x60,
		Vector2I32	= 0x61,
		Vector3I32	= 0x62,
		Vector4I32	= 0x63,

		Float32		= 0x80,
		Vector2F32	= 0x81,
		Vector3F32	= 0x82,
		Vector4F32	= 0x83,

		Float64		= 0x90,
		Vector2F64	= 0x91,
		Vector3F64	= 0x92,
		Vector4F64	= 0x93,

		UNorm8		= 0xa0,
		Vector2UN8	= 0xa1,
		Vector3UN8	= 0xa2,
		Vector4UN8	= 0xa3,

		UNorm16		= 0xb0,
		Vector2UN16	= 0xb1,
		Vector3UN16	= 0xb2,
		Vector4UN16	= 0xb3,

		Norm8		= 0xc0,
		Vector2N8	= 0xc1,
		Vector3N8	= 0xc2,
		Vector4N8	= 0xc3,

		Norm16		= 0xd0,
		Vector2N16	= 0xd1,
		Vector3N16	= 0xd2,
		Vector4N16	= 0xd3,
	};

	struct HLLayoutElement
	{
		uint32_t offset;
		HLLayoutElementType type;
		uint32_t index;
		uint32_t binding;
		uint32_t instanceDivisor; // 0 means per-vertex data
	};

	struct HLLayoutBinding
	{
		uint32_t offset;
		uint32_t stride;
	};

	struct HLLayoutCreateInfo
	{
		HLContext& context;
		std::span<const HLLayoutElement> elements;
		std::span<const HLLayoutBinding> bindings;
	};

	class HLLayout
	{
	public:
		// Note: This format is subject to change, please do not rely on this format as it is for internal use
		using NativeHandle = struct { std::span<const HLLayoutElement> elements; std::span<const HLLayoutBinding> bindings; };

		HLLayout() = default;
		HLLayout(const HLLayout&) = default;
		HLLayout(HLLayout&& rhs) noexcept :m_details(std::move(rhs.m_details)) {}
		~HLLayout() = default;

		HLLayout& operator=(const HLLayout&) = default;
		HLLayout& operator=(HLLayout&& rhs) noexcept { m_details = std::move(rhs.m_details); return *this; }

		Result Create(const HLLayoutCreateInfo& createInfo);
		Result Destroy();

		NativeHandle GetNativeHandle();

		bool IsInitialized();

	private:
		class Details;

		HLLayout(std::shared_ptr<Details> details) :m_details(details) {}

		std::shared_ptr<Details> m_details;
	};
}
