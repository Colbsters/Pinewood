#pragma once
#include "pch.h"
#include <Pinewood/Renderer/HLShaderModule.h>

namespace Pinewood
{
	static uint32_t GetGLShaderType(HLShaderModuleType type)
	{
		switch (type)
		{
		case Pinewood::HLShaderModuleType::Vertex:
			return GL_VERTEX_SHADER;
		case Pinewood::HLShaderModuleType::Pixel:
			return GL_FRAGMENT_SHADER;
		default:
			return 0;
		}
	}

	class HLShaderModule::Details
	{
	public:
		HLContext context;
		const GladGLContext* gl; // So I don't need to get it from the context all the time

		uint32_t shader;

		~Details();

		Result Destroy();
	};

	HLShaderModule::Details::~Details()
	{
		Destroy();
	}

	Result HLShaderModule::Details::Destroy()
	{
		gl->DeleteShader(shader);

		gl = nullptr;
		context = HLContext{};

		return Result::Success;
	}

	Result HLShaderModule::Create(const HLShaderModuleCreateInfo& createInfo)
	{
		m_details = std::make_shared<Details>();
		m_details->context = createInfo.context;
		m_details->gl = static_cast<const GladGLContext*>(m_details->context.GetNativeHandle().gl);

		m_details->shader = m_details->gl->CreateShader(GetGLShaderType(createInfo.type));

		{
			// Create some temporary variables so I can pass pointers to glShaderSource
			std::string shaderSource{ createInfo.shaderSource };
			const char* shaderSourceCStr = shaderSource.c_str();
			int shaderSourceLength = static_cast<int>(createInfo.shaderSource.size());

			m_details->gl->ShaderSource(m_details->shader, 1, &shaderSourceCStr, &shaderSourceLength);
		}

		m_details->gl->CompileShader(m_details->shader);

		// Check for compile errors
		int compileStatus;
		m_details->gl->GetShaderiv(m_details->shader, GL_COMPILE_STATUS, &compileStatus);
		if (compileStatus == GL_FALSE)
			return Result::SystemError; // Error message should be printed out by the debug callback

		return Result::Success;
	}

	Result HLShaderModule::Destroy()
	{
		return m_details->Destroy();
	}


	HLShaderModule::NativeHandle HLShaderModule::GetNativeHandle()
	{
		return m_details->shader;
	}
	
	bool HLShaderModule::IsInitialized()
	{
		return m_details && m_details->gl;
	}
}
