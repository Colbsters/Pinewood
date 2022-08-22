#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#include "Matrix2x2.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"

namespace PWMath
{
	// Translates a matrix (2D in 3x3 matrix)
	template<typename T, PackingMode P>
	constexpr Matrix3x3<T, P> Translate(const Matrix3x3<T, P>& matrix, const Vector2<T, P>& translation);

	// Translates a matrix (3D in 4x4 matrix)
	template<typename T, PackingMode P>
	constexpr Matrix4x4<T, P> Translate(const Matrix4x4<T, P>& matrix, const Vector3<T, P>& translation);


	// Scales a matrix (2D in 2x2 matrix)
	template<typename T, PackingMode P>
	constexpr Matrix2x2<T, P> Scale(const Matrix2x2<T, P>& matrix, const Vector2<T, P>& scale);

	// Scales a matrix (2D in 3x3 matrix)
	template<typename T, PackingMode P>
	constexpr Matrix3x3<T, P> Scale(const Matrix3x3<T, P>& matrix, const Vector2<T, P>& scale);

	// Scales a matrix (3D in 3x3 matrix)
	template<typename T, PackingMode P>
	constexpr Matrix3x3<T, P> Scale(const Matrix3x3<T, P>& matrix, const Vector3<T, P>& scale);

	// Scales a matrix (3D in 4x4 matrix)
	template<typename T, PackingMode P>
	constexpr Matrix4x4<T, P> Scale(const Matrix4x4<T, P>& matrix, const Vector3<T, P>& scale);


	// Rotates a matrix (2D in 2x2 matrix)
	template<typename T, PackingMode P>
	constexpr Matrix2x2<T, P> Rotate(const Matrix2x2<T, P>& matrix, float rotation);

	// Rotates a matrix (2D in 3x3 matrix)
	template<typename T, PackingMode P>
	constexpr Matrix3x3<T, P> Rotate(const Matrix3x3<T, P>& matrix, float rotation);

	// Rotates a matrix (3D in 3x3 matrix)
	template<typename T, PackingMode P>
	constexpr Matrix3x3<T, P> Rotate(const Matrix3x3<T, P>& matrix, float rotation, const Vector3<T, P>& axis);

	// Rotates a matrix (3D in 4x4 matrix)
	template<typename T, PackingMode P>
	constexpr Matrix4x4<T, P> Rotate(const Matrix4x4<T, P>& matrix, float rotation, const Vector3<T, P>& axis);


	// Shears a matrix (2D in 2x2 matrix)
	// Notes:
	//  - xShear describes how the X axis affects the other axes (the Y axis), same applies to yShear
	template<typename T, PackingMode P>
	constexpr Matrix2x2<T, P> Shear(const Matrix2x2<T, P>& matrix, float xShear, float yShear);

	// Shears a matrix (2D in 3x3 matrix)
	// Notes:
	//  - xShear describes how the X axis affects the other axes (the Y axis), same applies to yShear
	template<typename T, PackingMode P>
	constexpr Matrix3x3<T, P> Shear(const Matrix3x3<T, P>& matrix, float xShear, float yShear);

	// Shears a matrix (3D in 3x3 matrix)
	// Notes:
	//  - xShear describes how the X axis affects the other axes (the Y and Z axes), same applies to yShear and zShear
	//  - xShear.x indecates how the the X axis affects the Z axis, yShear.y indecates the effect the Y axis
	//  - yShear.x indecates how the the Y axis affects the X axis, yShear.y indecates the effect the Z axis
	//  - zShear.x indecates how the the Z axis affects the X axis, yShear.y indecates the effect the Y axis
	template<typename T, PackingMode P>
	constexpr Matrix3x3<T, P> Shear(const Matrix3x3<T, P>& matrix, const Vector2<T, P>& xShear, const Vector2<T, P>& yShear, const Vector2<T, P>& zShear);

	// Shears a matrix (3D in 4x4 matrix)
	// Notes:
	//  - xShear describes how the X axis affects the other axes (the Y and Z axes), same applies to yShear and zShear
	//  - xShear.x indecates how the the X axis affects the Z axis, yShear.y indecates the effect the Y axis
	//  - yShear.x indecates how the the Y axis affects the X axis, yShear.y indecates the effect the Z axis
	//  - zShear.x indecates how the the Z axis affects the X axis, yShear.y indecates the effect the Y axis
	template<typename T, PackingMode P>
	constexpr Matrix4x4<T, P> Shear(const Matrix4x4<T, P>& matrix, const Vector2<T, P>& xShear, const Vector2<T, P>& yShear, const Vector2<T, P>& zShear);
}

#include <PWMath/Impl/Transform.inl>
