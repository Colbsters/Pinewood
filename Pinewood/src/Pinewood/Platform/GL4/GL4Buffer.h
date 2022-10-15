#pragma once
#include "pch.h"
#include <Pinewood/Renderer/HL/HLBuffer.h>

namespace Pinewood
{
	class HLBuffer::Details
	{
	public:
		HLContext context;
		const GladGLContext* gl; // So I don't need to get it from the context all the time
		
		uint32_t buffer;

		~Details();

		Result Destroy();
	};

	HLBuffer::Details::~Details()
	{
		Destroy();
	}
	
	Result HLBuffer::Details::Destroy()
	{
		gl->DeleteBuffers(1, &buffer);

		gl = nullptr;
		context = HLContext{};

		return Result::Success;
	}

	Result HLBuffer::Create(const HLBufferCreateInfo& createInfo)
	{
		m_details = std::make_shared<Details>();
		m_details->context = createInfo.context;
		m_details->gl = static_cast<const GladGLContext*>(m_details->context.GetNativeHandle().gl);

		m_details->gl->CreateBuffers(1, &m_details->buffer);

		m_details->gl->NamedBufferStorage(m_details->buffer, createInfo.size, createInfo.data, 
			(createInfo.usage == HLBufferUsage::Mutable) ? (GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT) : 0);

		return Result::Success;
	}

	Result HLBuffer::Destroy()
	{
		return m_details->Destroy();
	}

	Result HLBuffer::SetData(void* data, size_t offset, size_t size)
	{
		m_details->gl->NamedBufferSubData(m_details->buffer, offset, size, data);

		return Result();
	}

	Result HLBuffer::GetData(void* dataOut, size_t offset, size_t size)
	{
		m_details->gl->GetNamedBufferSubData(m_details->buffer, offset, size, dataOut);

		return Result();
	}

	Result HLBuffer::Map(void*& ptrOut, HLBufferAccess access)
	{
		ptrOut = m_details->gl->MapNamedBuffer(m_details->buffer,
			(access == HLBufferAccess::Read ? GL_READ_ONLY :
			(access == HLBufferAccess::Write ? GL_WRITE_ONLY :
			(access == (HLBufferAccess::Read | HLBufferAccess::Write) ? GL_READ_WRITE : 0))));

		return Result::Success;
	}

	Result HLBuffer::Unmap()
	{
		bool ok = m_details->gl->UnmapNamedBuffer(m_details->buffer);

		return ok ? Result::Success : Result::BufferMapCorrupt;
	}

	HLBuffer::NativeHandle HLBuffer::GetNativeHandle()
	{
		return m_details->buffer;
	}
	
	bool HLBuffer::IsInitialized()
	{
		return m_details && m_details->gl;
	}
}
