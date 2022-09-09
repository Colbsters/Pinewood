#pragma once
#include <Pinewood/Core.h>
#include <Pinewood/Error.h>
#include <Pinewood/Renderer/HLContext.h>

namespace Pinewood
{
	enum class HLBufferUsage : uint32_t
	{
		Unknown		= 0,
		Mutable		= 1,
		Immutable	= 2,
	};

	enum class HLBufferAccess : uint32_t
	{
		Read		= 1,
		Write		= 2
	};

	namespace Operators
	{
		PW_DEFINE_ENUMCLASS_OPERATOR_OR(HLBufferAccess);
		PW_DEFINE_ENUMCLASS_OPERATOR_AND(HLBufferAccess);
		PW_DEFINE_ENUMCLASS_OPERATOR_NOT(HLBufferAccess);
	}

	struct HLBufferCreateInfo
	{
		HLContext context;
		HLBufferUsage usage;
		size_t size;
		void* data; // Can be nullptr
	};

	class HLBuffer
	{
	public:
		using NativeHandle = uint32_t;

		HLBuffer() = default;
		HLBuffer(const HLBuffer&) = default;
		HLBuffer(HLBuffer&& rhs) noexcept :m_details(std::move(rhs.m_details)) {}
		~HLBuffer() = default;

		HLBuffer& operator=(const HLBuffer&) = default;
		HLBuffer& operator=(HLBuffer&& rhs) noexcept { m_details = std::move(rhs.m_details); return *this; }

		Result Create(const HLBufferCreateInfo& createInfo);
		Result Destroy();

		// Sets the data in the buffer.
		// Params:
		//  - data = A pointer to the data to send to the buffer.
		//  - offset = The offset into the GPU's buffer.
		//  - size = The number of bytes to send to the GPU.
		Result SetData(void* data, size_t offset, size_t size);

		// Gets data from the buffer.
		// Params:
		//  - data = A pointer to where the data will be placed.
		//  - offset = The offset into the GPU's buffer.
		//  - size = The number of bytes to read from the GPU.
		Result GetData(void* dataOut, size_t offset, size_t size);

		// Maps the buffer into memory.
		// Params:
		//  - ptrOut = The returned pointer to the.
		//  - access = The access to the buffer (Read/Write, etc.).
		Result Map(void*& ptrOut, HLBufferAccess access);

		// Unmaps the buffer
		Result Unmap();

		NativeHandle GetNativeHandle();

		bool IsInitialized();

	private:
		class Details;

		HLBuffer(std::shared_ptr<Details> details) :m_details(details) {}

		std::shared_ptr<Details> m_details;
	};
}
