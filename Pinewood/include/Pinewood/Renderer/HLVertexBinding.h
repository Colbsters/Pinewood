#pragma once
#include <Pinewood/Core.h>
#include <Pinewood/Error.h>
#include <Pinewood/Renderer/HLContext.h>
#include <Pinewood/Renderer/HLBuffer.h>
#include <Pinewood/Renderer/HLLayout.h>

namespace Pinewood
{
	struct HLVertexBindingCreateInfo
	{
		HLContext context;
		std::span<const HLBuffer> vertexBuffers;
		HLBuffer indexBuffer;	// Optional
		HLLayout vertexLayout;
	};

	class HLVertexBinding
	{
	public:
		using NativeHandle = uint32_t;

		HLVertexBinding() = default;
		HLVertexBinding(const HLVertexBinding&) = default;
		HLVertexBinding(HLVertexBinding&& rhs) noexcept :m_details(std::move(rhs.m_details)) {}
		~HLVertexBinding() = default;

		HLVertexBinding& operator=(const HLVertexBinding&) = default;
		HLVertexBinding& operator=(HLVertexBinding&& rhs) noexcept { m_details = std::move(rhs.m_details); return *this; }

		Result Create(const HLVertexBindingCreateInfo& createInfo);
		Result Destroy();

		NativeHandle GetNativeHandle();

		bool IsInitialized();

	private:
		class Details;

		HLVertexBinding(std::shared_ptr<Details> details) :m_details(details) {}

		std::shared_ptr<Details> m_details;
	};
}
