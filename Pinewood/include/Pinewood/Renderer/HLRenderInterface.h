#pragma once
#include <Pinewood/Core.h>
#include <Pinewood/Error.h>

#include <Pinewood/Renderer/HLContext.h>

namespace Pinewood
{
	enum ClearTargetFlags
	{
		Color	= 0x01,
		Depth	= 0x02,
		Stencil	= 0x04
	};

	struct HLRenderInterfaceCreateInfo
	{
		std::shared_ptr<HLContext> context;
	};

	class HLRenderInterface
	{
	public:

		~HLRenderInterface();

		Result Create(const HLRenderInterfaceCreateInfo& createInfo);

		Result Destroy();
		
		// Sets the clear color for clear target (the type of color may change when the math library get added)
		Result SetClearColor(float (&color)[4]);

		// Sets the depth value for clear target
		Result SetClearDepth(float depth);

		// Sets the stencil value for clear target (behavior for values above the maximum are undefined)
		Result SetClearStencil(uint32_t stencil);
		
		// Clears the render target
		Result ClearTarget(ClearTargetFlags flags);

		std::shared_ptr<HLContext> GetContext();
	};
}
