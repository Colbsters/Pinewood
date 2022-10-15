#pragma once
#include "pch.h"
#include <Pinewood/Renderer/HL/HLShaderProgram.h>

namespace Pinewood
{
	class HLShaderProgram::Details
	{
	public:
		HLContext context;
		const GladGLContext* gl; // So I don't need to get it from the context all the time

		std::vector<HLShaderModule> modules;

		uint32_t program;

		~Details();

		Result Destroy();
	};

	HLShaderProgram::Details::~Details()
	{
		Destroy();
	}

	Result HLShaderProgram::Details::Destroy()
	{
		gl->DeleteProgram(program);

		gl = nullptr;
		context = HLContext{};

		return Result::Success;
	}

	Result HLShaderProgram::Create(const HLShaderProgramCreateInfo& createInfo)
	{
		m_details = std::make_shared<Details>();
		m_details->context = createInfo.context;
		m_details->gl = static_cast<const GladGLContext*>(m_details->context.GetNativeHandle().gl);

		std::copy(createInfo.shaderModules.begin(), createInfo.shaderModules.end(), std::back_inserter(m_details->modules));

		m_details->program = m_details->gl->CreateProgram();

		for (auto& module : m_details->modules)
			m_details->gl->AttachShader(m_details->program, module.GetNativeHandle());

		m_details->gl->LinkProgram(m_details->program);

		// Check for link errors
		int compileStatus;
		m_details->gl->GetProgramiv(m_details->program, GL_LINK_STATUS, &compileStatus);
		if (compileStatus == GL_FALSE)
			return Result::SystemError; // Error message should be printed out by the debug callback

		m_details->gl->ValidateProgram(m_details->program);

		// Check for validation errors
		m_details->gl->GetProgramiv(m_details->program, GL_VALIDATE_STATUS, &compileStatus);
		if (compileStatus == GL_FALSE)
			return Result::SystemError; // Error message should be printed out by the debug callback

		return Result::Success;
	}

	Result HLShaderProgram::Destroy()
	{
		return m_details->Destroy();
	}


	HLShaderProgram::NativeHandle HLShaderProgram::GetNativeHandle()
	{
		return m_details->program;
	}
	
	bool HLShaderProgram::IsInitialized()
	{
		return m_details && m_details->gl;
	}
}
