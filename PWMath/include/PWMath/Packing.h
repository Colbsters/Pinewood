#pragma once

namespace PWMath
{
	enum class PackingMode
	{
		Packed,			// Size of members (ex: Vector3Float is 12 bytes)
		Fast,			// Size of closest simd type (ex: __m128, __m256)

		Default = Packed	// Packed by default
	};
}
