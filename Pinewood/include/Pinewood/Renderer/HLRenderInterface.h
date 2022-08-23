#pragma once
#include <Pinewood/Core.h>
#include <Pinewood/Error.h>
#include <Pinewood/EnumSupport.h>
#include <Pinewood/Renderer/HLContext.h>

namespace Pinewood
{
	enum class ClearTargetFlags
	{
		Color	= 0x01,
		Depth	= 0x02,
		Stencil	= 0x04
	};

	namespace Operators
	{
		PW_DEFINE_ENUMCLASS_OPERATOR_OR(ClearTargetFlags);
		PW_DEFINE_ENUMCLASS_OPERATOR_AND(ClearTargetFlags);
		PW_DEFINE_ENUMCLASS_OPERATOR_EQUALS(ClearTargetFlags);
		PW_DEFINE_ENUMCLASS_OPERATOR_NOT(ClearTargetFlags);
	}

	struct HLRenderInterfaceCreateInfo
	{
		HLContext context;
	};

	class HLRenderInterface
	{
	public:

		~HLRenderInterface();

		Result Create(const HLRenderInterfaceCreateInfo& createInfo);

		Result Destroy();
		
		// Sets the clear color for clear target (the type of color may change when the math library get added)
		Result SetClearColor(const float (&color)[4]);

		// Sets the depth value for clear target
		Result SetClearDepth(float depth);

		// Sets the stencil value for clear target (behavior for values above the maximum are undefined)
		Result SetClearStencil(uint32_t stencil);
		
		// Clears the render target
		Result ClearTarget(uint32_t flags);

		HLContext GetContext();

	private:
#if PW_RENDERER_OPENGL4
		HLContext m_context;
		void* m_gl;
#endif
	};
}
