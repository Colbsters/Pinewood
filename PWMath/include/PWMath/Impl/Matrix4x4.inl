#pragma once
#include <PWMath/Matrix4x4.h>

namespace PWMath
{
#pragma region Unary Operators

	template<typename T, PackingMode P>
	Matrix<T, 4, 4, P> operator+(const Matrix<T, 4, 4, P>& rhs)
	{
		return rhs;
	}

	template<typename T, PackingMode P>
	Matrix<T, 4, 4, P> operator-(const Matrix<T, 4, 4, P>& rhs)
	{
		return rhs * -1;
	}

#pragma endregion

#pragma region Addition and substraction

	template<typename T, PackingMode P>
	Matrix<T, 4, 4, P> operator+(const Matrix<T, 4, 4, P>& lhs, const Matrix<T, 4, 4, P>& rhs)
	{
		return Matrix<T, 4, 4, P>{
			lhs.x + rhs.x,		// Using Vector4's operators to do the operation on rows at a time
			lhs.y + rhs.y,
			lhs.z + rhs.z,
			lhs.w + rhs.w
		};
	}

	template<typename T, PackingMode P>
	Matrix<T, 4, 4, P> operator-(const Matrix<T, 4, 4, P>& lhs, const Matrix<T, 4, 4, P>& rhs)
	{
		return Matrix<T, 4, 4, P>{
			lhs.x - rhs.x,		// Using Vector4's operators to do the operation on rows at a time
			lhs.y - rhs.y,
			lhs.z - rhs.z,
			lhs.w - rhs.w
		};
	}

	template<typename T, PackingMode P>
	const Matrix<T, 4, 4, P>& operator+=(Matrix<T, 4, 4, P>& lhs, const Matrix<T, 4, 4, P>& rhs)
	{
		return lhs = (lhs + rhs);
	}

	template<typename T, PackingMode P>
	const Matrix<T, 4, 4, P>& operator-=(Matrix<T, 4, 4, P>& lhs, const Matrix<T, 4, 4, P>& rhs)
	{
		return lhs = (lhs - rhs);
	}

#pragma endregion

#pragma region Matrix-scalar multiplication

	template<typename T, PackingMode P>
	Matrix<T, 4, 4, P> operator*(const Matrix<T, 4, 4, P>& lhs, T rhs)
	{
		// I could've constructed an identity matrix, but I feel this is simpler and easier to understand.
		// It may also make it easier for the compiler to optimize with less code.
		return Matrix<T, 4, 4, P>{
			lhs.x * rhs,		// Using Vector4's operators to do the operation on rows at a time
			lhs.y * rhs,
			lhs.z * rhs,
			lhs.w * rhs
		};
	}

	template<typename T, PackingMode P>
	const Matrix<T, 4, 4, P>& operator*=(Matrix<T, 4, 4, P>& lhs, T rhs)
	{
		return lhs = (lhs * rhs);
	}

	template<typename T, PackingMode P>
	Matrix<T, 4, 4, P> operator*(T lhs, const Matrix<T, 4, 4, P>& rhs)
	{
		// I could've constructed an identity matrix, but I feel this is simpler and easier to understand.
		// It may also make it easier for the compiler to optimize with less code.
		return rhs * lhs;
	}

#pragma endregion

#pragma region Matrix multiplication

	template<typename T, PackingMode P>
	Matrix<T, 4, 4, P> operator*(const Matrix<T, 4, 4, P>& lhs, const Matrix<T, 4, 4, P>& rhs)
	{
		return Matrix<T, 4, 4, P>{
			Dot(lhs.GetRow(0), rhs.GetColumn(0)), Dot(lhs.GetRow(0), rhs.GetColumn(1)), Dot(lhs.GetRow(0), rhs.GetColumn(2)), Dot(lhs.GetRow(0), rhs.GetColumn(3)),
			Dot(lhs.GetRow(1), rhs.GetColumn(0)), Dot(lhs.GetRow(1), rhs.GetColumn(1)), Dot(lhs.GetRow(1), rhs.GetColumn(2)), Dot(lhs.GetRow(1), rhs.GetColumn(3)),
			Dot(lhs.GetRow(2), rhs.GetColumn(0)), Dot(lhs.GetRow(2), rhs.GetColumn(1)), Dot(lhs.GetRow(2), rhs.GetColumn(2)), Dot(lhs.GetRow(2), rhs.GetColumn(3)),
			Dot(lhs.GetRow(3), rhs.GetColumn(0)), Dot(lhs.GetRow(3), rhs.GetColumn(1)), Dot(lhs.GetRow(3), rhs.GetColumn(2)), Dot(lhs.GetRow(3), rhs.GetColumn(3))
		};
	}

	template<typename T, PackingMode P>
	const Matrix<T, 4, 4, P>& operator*=(Matrix<T, 4, 4, P>& lhs, const Matrix<T, 4, 4, P>& rhs)
	{
		return *this = *this * rhs;
	}

#pragma endregion

#pragma region Matrix-vector multiplication

	template<typename T, PackingMode P>
	Vector<T, 4, P> operator*(const Matrix<T, 4, 4, P>& lhs, const Vector<T, 4, P>& rhs)
	{
		return Vector<T, 4, P>{
			Dot(lhs[0], rhs),
			Dot(lhs[1], rhs),
			Dot(lhs[2], rhs),
			Dot(lhs[3], rhs)
		};
	}

	template<typename T, PackingMode P>
	Vector<T, 4, P> operator*(const Vector<T, 4, P>& lhs, const Matrix<T, 4, 4, P>& rhs)
	{
		return (lhs[0] * rhs[0]) + (lhs[1] * rhs[1]) + (lhs[2] * rhs[2]) + (lhs[3] * rhs[3]);
	}

	template<typename T, PackingMode P>
	const Vector<T, 4, P>& operator*=(Vector<T, 4, P>& lhs, const Matrix<T, 4, 4, P>& rhs)
	{
		return lhs = (lhs * rhs);
	}

#pragma endregion


#pragma region Functions

	template<typename T, PackingMode P>
	Matrix<T, 4, 4, P>::TransposeType Transpose(const Matrix<T, 4, 4, P>& matrix)
	{
		return typename Matrix<T, 4, 4, P>::TransposeType{
			matrix[0][0], matrix[1][0], matrix[2][0], matrix[3][0],
			matrix[0][1], matrix[1][1], matrix[2][1], matrix[3][1],
			matrix[0][2], matrix[1][2], matrix[2][2], matrix[3][2],
			matrix[0][3], matrix[1][3], matrix[2][3], matrix[3][3]
		};
	}

	template<typename T, PackingMode P>
	T Determinant(const Matrix<T, 4, 4, P>& matrix)
	{
		// 2x2 determinants, numbers are the indexs of the columns
		const T determinant01 = (matrix[2][0] * matrix[3][1]) - (matrix[2][1] * matrix[3][0]);
		const T determinant02 = (matrix[2][0] * matrix[3][2]) - (matrix[2][2] * matrix[3][0]);
		const T determinant03 = (matrix[2][0] * matrix[3][3]) - (matrix[2][3] * matrix[3][0]);
		const T determinant12 = (matrix[2][1] * matrix[3][2]) - (matrix[2][2] * matrix[3][1]);
		const T determinant13 = (matrix[2][1] * matrix[3][3]) - (matrix[2][3] * matrix[3][1]);
		const T determinant23 = (matrix[2][2] * matrix[3][3]) - (matrix[2][3] * matrix[3][2]);

		// 3x3 determinants, numbers are the indexs of the columns
		const T determinant012 = (matrix[1][0] * determinant12) - (matrix[1][1] * determinant02) + (matrix[1][2] * determinant01);
		const T determinant013 = (matrix[1][0] * determinant13) - (matrix[1][1] * determinant03) + (matrix[1][3] * determinant01);
		const T determinant023 = (matrix[1][0] * determinant23) - (matrix[1][2] * determinant03) + (matrix[1][3] * determinant02);
		const T determinant123 = (matrix[1][1] * determinant23) - (matrix[1][2] * determinant13) + (matrix[1][3] * determinant12);

		return (matrix[0][0] * determinant123)
			- (matrix[0][1] * determinant023)
			+ (matrix[0][2] * determinant013)
			- (matrix[0][3] * determinant012);
	}

#pragma endregion

#pragma region Member version of functions

	template<typename T, PackingMode P>
	inline Matrix<T, 4, 4, P>::TransposeType Matrix<T, 4, 4, P>::Transpose() const { return PWMath::Transpose(*this); }

	template<typename T, PackingMode P>
	inline T Matrix<T, 4, 4, P>::Determinant() const { return PWMath::Determinant(*this); }

#pragma endregion

}
