#pragma once
#include <Pinewood/Core.h>
#include <Pinewood/Error.h>
#include <Pinewood/Renderer/HLContext.h>

namespace Pinewood
{
	enum class HLShaderModuleType
	{
		Unknown		= 0,
		Vertex		= 1,
		Pixel		= 2
	};

	struct HLShaderModuleCreateInfo
	{
		HLContext context;

		HLShaderModuleType type;

		// Subject to change
		std::string_view shaderSource;
	};

	class HLShaderModule
	{
	public:
		using NativeHandle = uint32_t;

		HLShaderModule() = default;
		HLShaderModule(const HLShaderModule&) = default;
		HLShaderModule(HLShaderModule&& rhs) noexcept :m_details(std::move(rhs.m_details)) {}
		~HLShaderModule() = default;

		HLShaderModule& operator=(const HLShaderModule&) = default;
		HLShaderModule& operator=(HLShaderModule&& rhs) noexcept { m_details = std::move(rhs.m_details); return *this; }

		Result Create(const HLShaderModuleCreateInfo& createInfo);
		Result Destroy();

		NativeHandle GetNativeHandle();

		bool IsInitialized();

	private:
		class Details;

		HLShaderModule(std::shared_ptr<Details> details) :m_details(details) {}

		std::shared_ptr<Details> m_details;
	};
}
