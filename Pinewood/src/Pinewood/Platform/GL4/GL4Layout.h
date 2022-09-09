#pragma once
#include "pch.h"
#include <Pinewood/Renderer/HLLayout.h>

namespace Pinewood
{
	class HLLayout::Details
	{
	public:
		std::vector<HLLayoutElement> layoutElements;
		std::vector<HLLayoutBinding> bindings;

		~Details();

		Result Destroy();
	};

	HLLayout::Details::~Details()
	{
		Destroy();
	}
	
	Result HLLayout::Details::Destroy()
	{
		layoutElements.clear();
		return Result::Success;
	}

	Result HLLayout::Create(const HLLayoutCreateInfo& createInfo)
	{
		m_details = std::make_shared<Details>();
		std::copy(createInfo.elements.begin(), createInfo.elements.end(), std::back_inserter(m_details->layoutElements));
		std::copy(createInfo.bindings.begin(), createInfo.bindings.end(), std::back_inserter(m_details->bindings));

		return Result::Success;
	}

	Result HLLayout::Destroy()
	{
		return m_details->Destroy();
	}

	HLLayout::NativeHandle HLLayout::GetNativeHandle()
	{
		return HLLayout::NativeHandle{ { m_details->layoutElements.data(), m_details->layoutElements.size() }, { m_details->bindings.data(), m_details->bindings.size() } };
	}
}
