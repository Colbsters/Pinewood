#pragma once
#include "pch.h"
#include <Pinewood/Renderer/HLVertexBinding.h>

namespace Pinewood
{
	struct GLType { GLenum type; int size; bool normalized; };
	constexpr static GLType GetGLType(HLLayoutElementType type)
	{
		GLType glType = { 0, 0, false };
		switch (static_cast<uint32_t>(type) >> 8)
		{
		case 0x0:
			glType.type = GL_UNSIGNED_BYTE;
			break;
		case 0x1:
			glType.type = GL_UNSIGNED_SHORT;
			break;
		case 0x2:
			glType.type = GL_UNSIGNED_INT;
			break;
		case 0x4:
			glType.type = GL_BYTE;
			break;
		case 0x5:
			glType.type = GL_SHORT;
			break;
		case 0x6:
			glType.type = GL_INT;
			break;
		case 0x8:
			glType.type = GL_FLOAT;
			break;
		case 0x9:
			glType.type = GL_DOUBLE;
			break;
		case 0xa:
			glType.type = GL_UNSIGNED_BYTE;
			glType.normalized = true;
			break;
		case 0xb:
			glType.type = GL_UNSIGNED_SHORT;
			glType.normalized = true;
			break;
		case 0xc:
			glType.type = GL_BYTE;
			glType.normalized = true;
			break;
		case 0xd:
			glType.type = GL_SHORT;
			glType.normalized = true;
			break;
		default:
			return {};
		}

		glType.size = static_cast<uint32_t>(type) & 0x3;

		return glType;
	}

	class HLVertexBinding::Details
	{
	public:
		HLContext context;
		const GladGLContext* gl; // So I don't need to get it from the context all the time

		std::vector<HLBuffer> vertexBuffers;
		HLBuffer indexBuffer;
		HLLayout vertexLayout;

		uint32_t vao;

		~Details();

		Result Destroy();
	};

	HLVertexBinding::Details::~Details()
	{
		Destroy();
	}

	Result HLVertexBinding::Details::Destroy()
	{
		gl->DeleteBuffers(1, &vao);

		gl = nullptr;
		context = HLContext{};

		return Result::Success;
	}

	Result HLVertexBinding::Create(const HLVertexBindingCreateInfo& createInfo)
	{

		m_details = std::make_shared<Details>();
		m_details->context = createInfo.context;
		m_details->gl = static_cast<const GladGLContext*>(m_details->context.GetNativeHandle().gl);

		std::copy(createInfo.vertexBuffers.begin(), createInfo.vertexBuffers.end(), std::back_inserter(m_details->vertexBuffers));
		m_details->indexBuffer = createInfo.indexBuffer;
		m_details->vertexLayout = createInfo.vertexLayout;

		const auto vertexLayoutData = m_details->vertexLayout.GetNativeHandle();

		m_details->gl->CreateVertexArrays(1, &m_details->vao);

		for (auto& element : vertexLayoutData.elements)
		{
			// Enable the attribute index
			m_details->gl->EnableVertexArrayAttrib(m_details->vao, element.index);

			// Set the format of the attribute
			auto glType = GetGLType(element.type);
			if (glType.type == GL_DOUBLE)
				m_details->gl->VertexArrayAttribLFormat(m_details->vao, element.index, glType.size,
					glType.type, element.offset);
			else
				m_details->gl->VertexArrayAttribFormat(m_details->vao, element.index, glType.size,
					glType.type, glType.normalized, element.offset);

			// Bind the attribute to a vertex buffer binding (see the next loop where I loop over all the vertex buffers)
			m_details->gl->VertexArrayAttribBinding(m_details->vao, element.index, element.binding);
		}

		// Make sure the sizes match
		if (createInfo.vertexBuffers.size() != vertexLayoutData.bindings.size())
			return Result::InvalidParameter;

		// Bind the vertex buffers
		for (uint32_t i = 0; i < createInfo.vertexBuffers.size(); i++)
			m_details->gl->VertexArrayVertexBuffer(m_details->vao, i, m_details->vertexBuffers[i].GetNativeHandle(),
				vertexLayoutData.bindings[i].offset, vertexLayoutData.bindings[i].stride);

		// Optionally bind the index buffer
		if (m_details->indexBuffer.IsInitialized())
			m_details->gl->VertexArrayElementBuffer(m_details->vao, m_details->indexBuffer.GetNativeHandle());

		return Result::Success;
	}

	Result HLVertexBinding::Destroy()
	{
		return m_details->Destroy();
	}


	HLVertexBinding::NativeHandle HLVertexBinding::GetNativeHandle()
	{
		return m_details->vao;
	}
	
	bool HLVertexBinding::IsInitialized()
	{
		return m_details && m_details->gl;
	}
}
