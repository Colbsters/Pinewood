#pragma once
#include <Pinewood/Core.h>
#include <Pinewood/Error.h>
#include <Pinewood/Renderer/HL/HLContext.h>
#include <Pinewood/Renderer/HL/HLShaderModule.h>

namespace Pinewood
{
	struct HLShaderProgramCreateInfo
	{
		HLContext context;

		// Subject to change
		std::span<HLShaderModule> shaderModules;
	};

	class HLShaderProgram
	{
	public:
		using NativeHandle = uint32_t;

		HLShaderProgram() = default;
		HLShaderProgram(const HLShaderProgram&) = default;
		HLShaderProgram(HLShaderProgram&& rhs) noexcept :m_details(std::move(rhs.m_details)) {}
		~HLShaderProgram() = default;

		HLShaderProgram& operator=(const HLShaderProgram&) = default;
		HLShaderProgram& operator=(HLShaderProgram&& rhs) noexcept { m_details = std::move(rhs.m_details); return *this; }

		Result Create(const HLShaderProgramCreateInfo& createInfo);
		Result Destroy();

		NativeHandle GetNativeHandle();

		bool IsInitialized();

	private:
		class Details;

		HLShaderProgram(std::shared_ptr<Details> details) :m_details(details) {}

		std::shared_ptr<Details> m_details;
	};
}
