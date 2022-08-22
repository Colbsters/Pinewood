#pragma once
#include <PWMath/Packing.h>

namespace PWMath
{
	// Template base type for vector
	template<typename T, size_t C, size_t R, PackingMode P = PackingMode::Default>
	struct Matrix {};
}
