#pragma once
#include <PWMath/Vector2.h>
#include <PWMath/Vector3.h>
#include <PWMath/Matrix3x3.h>
#include <PWMath/Matrix4x4.h>

namespace PWMath
{
	// Creates a 3D orthographic projection matrix (4x4 matrix)
	// Notes:
	//  - Z axis gets squished into a 0-1 range
	template<typename T = float, PackingMode P = PackingMode::Default>
	constexpr Matrix4x4<T, P> Orthographic(float screenSize, float aspectRatio, float near, float far);

	// Creates a 3D orthographic projection matrix (4x4 matrix)
	// Notes:
	//  - Z axis gets squished into a (-1)-1 range (For OpenGL)
	template<typename T = float, PackingMode P = PackingMode::Default>
	constexpr Matrix4x4<T, P> OrthographicGL(float screenSize, float aspectRatio, float near, float far);

	// Creates a perspective projection matrix (4x4 matrix)
	// Notes:
	//  - Z axis gets squished into a 0-1 range
	template<typename T = float, PackingMode P = PackingMode::Default>
	constexpr Matrix4x4<T, P> Perpective(float fov, float aspectRatio, float near, float far);

	// Creates a 3D orthographic projection matrix (4x4 matrix)
	// Notes:
	//  - Z axis gets squished into a (-1)-1 range (For OpenGL)
	template<typename T = float, PackingMode P = PackingMode::Default>
	constexpr Matrix4x4<T, P> PerpectiveGL(float screenSize, float aspectRatio, float near, float far);
}

#include <PWMath/Impl/Projection.inl>
