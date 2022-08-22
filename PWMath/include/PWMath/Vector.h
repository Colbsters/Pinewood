#pragma once
#include <PWMath/Packing.h>

namespace PWMath
{
	// Template base type for vector
	template<typename T, size_t L, PackingMode P = PackingMode::Default>
	struct Vector {};
}
