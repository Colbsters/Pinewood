#pragma once
#include <Pinewood/Core.h>
#include <Pinewood/Error.h>
#include <Pinewood/EnumSupport.h>
#include <Pinewood/Renderer/HL/HLContext.h>
#include <Pinewood/Renderer/HL/HLVertexBinding.h>
#include <Pinewood/Renderer/HL/HLShaderProgram.h>
#include <Pinewood/Renderer/HL/HLTexture2D.h>
#include <Pinewood/Renderer/HL/HLFramebuffer.h>

namespace Pinewood
{
	enum class ClearTargetFlags : uint32_t
	{
		Color	= 0x01,
		Depth	= 0x02,
		Stencil	= 0x04
	};

	namespace Operators
	{
		PW_DEFINE_ENUMCLASS_OPERATOR_OR(ClearTargetFlags);
		PW_DEFINE_ENUMCLASS_OPERATOR_AND(ClearTargetFlags);
		PW_DEFINE_ENUMCLASS_OPERATOR_NOT(ClearTargetFlags);
	}

	struct HLRenderInterfaceCreateInfo
	{
		HLContext context;
	};

	class HLRenderInterface
	{
	public:
		HLRenderInterface() = default;
		HLRenderInterface(const HLRenderInterface&) = default;
		HLRenderInterface(HLRenderInterface&& rhs) noexcept :m_details(std::move(rhs.m_details)) {}
		~HLRenderInterface() = default;

		HLRenderInterface& operator=(const HLRenderInterface&) = default;
		HLRenderInterface& operator=(HLRenderInterface&& rhs) noexcept { m_details = std::move(rhs.m_details); return *this; }

		Result Create(const HLRenderInterfaceCreateInfo& createInfo);

		Result Destroy();
		
		// Sets the clear color for clear target (the type of color may change when the math library get added)
		Result SetClearColor(const float (&color)[4]);

		// Sets the depth value for clear target
		Result SetClearDepth(float depth);

		// Sets the stencil value for clear target (behavior for values above the maximum are undefined)
		Result SetClearStencil(uint32_t stencil);
		
		// Clears the render target
		Result ClearTarget(ClearTargetFlags flags);

		// Binds a vertex binding for rending
		Result BindVertexBinding(const HLVertexBinding& vertexBinding);

		// Binds a vertex binding for rending
		Result BindShaderProgram(const HLShaderProgram& program);

		// Sets a constant buffer
		Result SetConstantBuffer(uint32_t index, const HLBuffer& buffer);

		// Sets a texture
		Result SetTexture2D(uint32_t location, uint32_t slot, const HLTexture2D& texture);

		// Draws using the vertex buffer
		Result Draw(uint32_t startIndex, uint32_t count);

		// Draws using the index buffer and the vertex buffer
		Result DrawIndexed(uint32_t count);

		// Sets the framebuffer
		Result SetFramebuffer(const HLFramebuffer& framebuffer);

		// Use default framebuffer
		Result ResetFramebuffer();


		HLContext GetContext();

		bool IsInitialized();

	private:
		class Details;
		
		HLRenderInterface(std::shared_ptr<Details>& details) :m_details(details) {}

		std::shared_ptr<Details> m_details;
	};
}
