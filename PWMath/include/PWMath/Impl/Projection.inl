#pragma once
#include <PWMath/Projection.h>
#include <cmath>
#include <algorithm>

namespace PWMath
{
	template<typename T, PackingMode P>
	constexpr Matrix4x4<T, P> Orthographic(float screenSize, float aspectRatio, float near, float far)
	{
		float width = screenSize * std::max(aspectRatio, 1.0f), height = screenSize / std::min(aspectRatio, 1.0f);

		return Matrix4x4<T, P>{
			2 / width,	0,			0,						0,
			0,			2 / height,	0,						0,
			0,			0,			1 / (far - near),		0,
			0,			0,			-near / (far - near),	1
		};
	}

	template<typename T, PackingMode P>
	constexpr Matrix4x4<T, P> OrthographicGL(float screenSize, float aspectRatio, float near, float far)
	{
		float width = screenSize * std::max(aspectRatio, 1.0f), height = screenSize / std::min(aspectRatio, 1.0f);

		return Matrix4x4<T, P>{
			2 / width,	0,			0,								0,
			0,			2 / height,	0,								0,
			0,			0,			2 / (far - near),				0,
			0,			0,			-(2 * near) / (far - near) - 1,	1
		};
	}

	template<typename T, PackingMode P>
	constexpr Matrix4x4<T, P> Perpective(float fov, float aspectRatio, float near, float far)
	{
		float tX = std::tan(0.5f * fov * std::max(aspectRatio, 1.0f)), tY = std::tan(0.5f * fov / std::min(aspectRatio, 1.0f));

		return Matrix4x4<T, P>{
			1 / tX,	0,		0,								0,
			0,		1 / tY,	0,								0,
			0,		0,		far / (far-near),				1,
			0,		0,		-(far * near) / (far - near),	0
		};
	}

	template<typename T, PackingMode P>
	constexpr Matrix4x4<T, P> PerpectiveGL(float fov, float aspectRatio, float near, float far)
	{
		float tX = std::tan(0.5f * fov * std::max(aspectRatio, 1.0f)), tY = std::tan(0.5f * fov / std::min(aspectRatio, 1.0f));

		return Matrix4x4<T, P>{
			1 / tX,	0,		0,									0,
			0,		1 / tY,	0,									0,
			0,		0,		(far + near) / (far-near),			1,
			0,		0,		-(2 * far * near) / (far - near),	0
		};
	}
}
