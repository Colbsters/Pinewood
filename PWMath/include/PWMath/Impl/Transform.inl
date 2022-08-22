#pragma once
#include <PWMath/Transform.h>

#include <cmath>

namespace PWMath
{
	template<typename T, PackingMode P>
	constexpr Matrix3x3<T, P> Translate(const Matrix3x3<T, P>& matrix, const Vector2<T, P>& translation)
	{
		// Create an identity matrix and fill the bottom row with the translation
		Matrix3x3<T, P> transform{ 1 };
		transform[2] = Vector3<T, P>{ translation, static_cast<T>(1) };
		// Multiply with the previous matrix
		return matrix * transform;
	}

	template<typename T, PackingMode P>
	constexpr Matrix4x4<T, P> Translate(const Matrix4x4<T, P>& matrix, const Vector3<T, P>& translation)
	{
		// Create an identity matrix and fill the bottom row with the translation
		Matrix4x4<T, P> transform{ 1 };
		transform[3] = Vector4<T, P>{ translation, static_cast<T>(1) };
		// Multiply with the previous matrix
		return matrix * transform;
	}


	template<typename T, PackingMode P>
	constexpr Matrix2x2<T, P> Scale(const Matrix2x2<T, P>& matrix, const Vector2<T, P>& scale)
	{
		// Create a scale matrix
		Matrix2x2<T, P> transform{
			scale.x,	0,
			0,			scale.y
		};
		// Transform matrix
		return matrix * transform;
	}

	template<typename T, PackingMode P>
	constexpr Matrix3x3<T, P> Scale(const Matrix3x3<T, P>& matrix, const Vector2<T, P>& scale)
	{
		// Create a scale matrix
		Matrix3x3<T, P> transform{
			scale.x,	0,			0,
			0,			scale.y,	0,
			0,			0,			1
		};
		// Transform matrix
		return matrix * transform;
	}

	template<typename T, PackingMode P>
	constexpr Matrix3x3<T, P> Scale(const Matrix3x3<T, P>& matrix, const Vector3<T, P>& scale)
	{
		// Create a scale matrix
		Matrix3x3<T, P> transform{
			scale.x,	0,			0,
			0,			scale.y,	0,
			0,			0,			scale.z
		};
		// Transform matrix
		return matrix * transform;
	}

	template<typename T, PackingMode P>
	constexpr Matrix4x4<T, P> Scale(const Matrix4x4<T, P>& matrix, const Vector3<T, P>& scale)
	{
		// Create a scale matrix
		Matrix4x4<T, P> transform{
			scale.x,	0,			0,			0,
			0,			scale.y,	0,			0,
			0,			0,			scale.z,	0,
			0,			0,			0,			1
		};
		// Transform matrix
		return matrix * transform;
	}


	template<typename T, PackingMode P>
	constexpr Matrix2x2<T, P> Rotate(const Matrix2x2<T, P>& matrix, float rotation)
	{
		const auto s = std::sin(rotation), c = std::cos(rotation);

		// Create a rotation matrix
		Matrix2x2<T, P> transform{
			c,-s,
			s, c
		};
		// Transform matrix
		return matrix * transform;
	}

	// Rotates a matrix (2D in 3x3 matrix)
	template<typename T, PackingMode P>
	constexpr Matrix3x3<T, P> Rotate(const Matrix3x3<T, P>& matrix, float rotation)
	{
		const auto s = std::sin(rotation), c = std::cos(rotation);

		// Create a rotation matrix
		Matrix3x3<T, P> transform{
			c,-s, 0,
			s, c, 0,
			0, 0, 1
		};
		// Transform matrix
		return matrix * transform;
	}

	template<typename T, PackingMode P>
	constexpr Matrix3x3<T, P> Rotate(const Matrix3x3<T, P>& matrix, float rotation, const Vector3<T, P>& axis)
	{
		const auto u = axis.Normalize();
		const auto s = std::sin(rotation), c = std::cos(rotation);
		const auto u_1subc = u * (1 - c);

		// Create a rotation matrix
		// For more info, see https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
		Matrix3x3<T, P> transform{
			c + u.x * u_1subc.x,		u.x * u_1subc.y - u.z * s,	u.x * u_1subc.z + u.y * s,
			u.y * u_1subc.x + u.z * s,	c + u.y * u_1subc.y,		u.y * u_1subc.z - u.x * s,
			u.z * u_1subc.x - u.y * s,	u.z * u_1subc.y + u.x * s,	c + u.z * u_1subc.z
		};
		// Transform matrix
		return matrix * transform;
	}

	// Rotates a matrix (3D in 4x4 matrix)
	template<typename T, PackingMode P>
	constexpr Matrix4x4<T, P> Rotate(const Matrix4x4<T, P>& matrix, float rotation, const Vector3<T, P>& axis)
	{
		const auto u = axis.Normalize();
		const auto s = std::sin(rotation), c = std::cos(rotation);
		const auto u_1subc = u * (1 - c);

		// Create a rotation matrix
		// For more info, see https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
		Matrix4x4<T, P> transform{
			c + u.x * u_1subc.x,		u.x * u_1subc.y - u.z * s,	u.x * u_1subc.z + u.y * s,	0,
			u.y * u_1subc.x + u.z * s,	c + u.y * u_1subc.y,		u.y * u_1subc.z - u.x * s,	0,
			u.z * u_1subc.x - u.y * s,	u.z * u_1subc.y + u.x * s,	c + u.z * u_1subc.z,		0,
			0,							0,							0,							1
		};
		// Transform matrix
		return matrix * transform;
	}


	template<typename T, PackingMode P>
	constexpr Matrix2x2<T, P> Shear(const Matrix2x2<T, P>& matrix, float xShear, float yShear)
	{
		// Create a shear matrix
		Matrix2x2<T, P> transform{
			1,		xShear,
			yShear,	1
		};
		// Transform matrix
		return matrix * transform;
	}

	template<typename T, PackingMode P>
	constexpr Matrix3x3<T, P> Shear(const Matrix3x3<T, P>& matrix, float xShear, float yShear)
	{
		// Create a shear matrix
		Matrix3x3<T, P> transform{
			1,		xShear,	0,
			yShear,	1,		0,
			0,		0,		1
		};
		// Transform matrix
		return matrix * transform;
	}

	template<typename T, PackingMode P>
	constexpr Matrix3x3<T, P> Shear(const Matrix3x3<T, P>& matrix, const Vector2<T, P>& xShear, const Vector2<T, P>& yShear, const Vector2<T, P>& zShear)
	{
		// Create a shear matrix
		Matrix3x3<T, P> transform{
			1,			xShear.y,	xShear.x,
			yShear.x,	1,			yShear.y,
			zShear.x,	zShear.y,	1
		};
		// Transform matrix
		return matrix * transform;
	}

	template<typename T, PackingMode P>
	constexpr Matrix4x4<T, P> Shear(const Matrix4x4<T, P>& matrix, const Vector2<T, P>& xShear, const Vector2<T, P>& yShear, const Vector2<T, P>& zShear)
	{
		// Create a shear matrix
		Matrix4x4<T, P> transform{
			1,			xShear.y,	xShear.x,	0,
			yShear.x,	1,			yShear.y,	0,
			zShear.x,	zShear.y,	1,			0,
			0,			0,			0,			1
		};
		// Transform matrix
		return matrix * transform;
	}
}
