#pragma once
#include <Pinewood/Core.h>
#include <exception>

namespace Pinewood
{
	enum class Result : uint32_t
	{
		// Success Values
		Success				= 0,

		// Error values (General)
		UnknownError		= 0x80000000,
		SystemError			= 0x80000001,
		InvalidParameter	= 0x80000002,
		OutOfMemory			= 0x80000003,
		NotInitialized		= 0x80000004,
	};

	class Exception
		:public std::exception
	{
	public:
		Exception(Result result)
			:m_result(result)
		{}

		virtual const char* what() const noexcept override
		{
			return "Pinewood Exception";
		}

		inline Result GetErrorCode() const noexcept
		{
			return m_result;
		}

	private:
		Result m_result;
	};

	constexpr bool IsError(Result result)
	{
		return static_cast<uint32_t>(result) & 0x80000000;
	}

	inline void ThrowException(Result result)
	{
#ifdef PW_USE_EXCEPTIONS
		throw Exception(result);
#endif
	}
}