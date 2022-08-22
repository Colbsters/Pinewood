#pragma once
#include <PWMath/Matrix3x3.h>

namespace PWMath
{
#pragma region Unary operators

	template<typename T, PackingMode P>
	Matrix<T, 3, 3, P> operator+(const Matrix<T, 3, 3, P>& rhs)
	{
		return rhs;
	}

	template<typename T, PackingMode P>
	Matrix<T, 3, 3, P> operator-(const Matrix<T, 3, 3, P>& rhs)
	{
		return rhs * -1;
	}

#pragma endregion

#pragma region Addition and substraction

	template<typename T, PackingMode P>
	Matrix<T, 3, 3, P> operator+(const Matrix<T, 3, 3, P>& lhs, const Matrix<T, 3, 3, P>& rhs)
	{
		return Matrix<T, 3, 3, P>{
			lhs.x + rhs.x,		// Using Vector3's operators to do the operation on rows at a time
			lhs.y + rhs.y,
			lhs.z + rhs.z
		};
	}

	template<typename T, PackingMode P>
	Matrix<T, 3, 3, P> operator-(const Matrix<T, 3, 3, P>& lhs, const Matrix<T, 3, 3, P>& rhs)
	{
		return Matrix<T, 3, 3, P>{
			lhs.x - rhs.x,		// Using Vector3's operators to do the operation on rows at a time
			lhs.y - rhs.y,
			lhs.z - rhs.z
		};
	}

	template<typename T, PackingMode P>
	const Matrix<T, 3, 3, P>& operator+=(Matrix<T, 3, 3, P>& lhs, const Matrix<T, 3, 3, P>& rhs)
	{
		return lhs = (lhs + rhs);
	}

	template<typename T, PackingMode P>
	const Matrix<T, 3, 3, P>& operator-=(Matrix<T, 3, 3, P>& lhs, const Matrix<T, 3, 3, P>& rhs)
	{
		return lhs = (lhs - rhs);
	}

#pragma endregion

#pragma region Matrix-scalar multiplication

	template<typename T, PackingMode P>
	Matrix<T, 3, 3, P> operator*(const Matrix<T, 3, 3, P>& lhs, T rhs)
	{
		// I could've constructed an identity matrix, but I feel this is simpler and easier to understand.
		// It may also make it easier for the compiler to optimize with less code.
		return Matrix<T, 3, 3, P>{
			lhs.x * rhs,		// Using Vector3's operators to do the operation on rows at a time
			lhs.y * rhs,
			lhs.z * rhs
		};
	}

	template<typename T, PackingMode P>
	const Matrix<T, 3, 3, P>& operator*=(Matrix<T, 3, 3, P>& lhs, T rhs)
	{
		return lhs = (lhs * rhs);
	}

	template<typename T, PackingMode P>
	Matrix<T, 3, 3, P> operator*(T lhs, const Matrix<T, 3, 3, P>& rhs)
	{
		// I could've constructed an identity matrix, but I feel this is simpler and easier to understand.
		// It may also make it easier for the compiler to optimize with less code.
		return rhs * lhs;
	}

#pragma endregion

#pragma region Matrix multiplication

	template<typename T, PackingMode P>
	Matrix<T, 3, 3, P> operator*(const Matrix<T, 3, 3, P>& lhs, const Matrix<T, 3, 3, P>& rhs)
	{
		return Matrix<T, 3, 3, P>{
			Dot(lhs.GetRow(0), rhs.GetColumn(0)), Dot(lhs.GetRow(0), rhs.GetColumn(1)), Dot(lhs.GetRow(0), rhs.GetColumn(2)),
			Dot(lhs.GetRow(1), rhs.GetColumn(0)), Dot(lhs.GetRow(1), rhs.GetColumn(1)), Dot(lhs.GetRow(1), rhs.GetColumn(2)),
			Dot(lhs.GetRow(2), rhs.GetColumn(0)), Dot(lhs.GetRow(2), rhs.GetColumn(1)), Dot(lhs.GetRow(2), rhs.GetColumn(2))
		};
	}

	template<typename T, PackingMode P>
	const Matrix<T, 3, 3, P>& operator*=(Matrix<T, 3, 3, P>& lhs, const Matrix<T, 3, 3, P>& rhs)
	{
		return *this = *this * rhs;
	}

#pragma endregion

#pragma region Matrix-vector multiplication

	template<typename T, PackingMode P>
	Vector<T, 3, P> operator*(const Matrix<T, 3, 3, P>& lhs, const Vector<T, 3, P>& rhs)
	{
		return Vector<T, 3, P>{
			Dot(lhs[0], rhs),
			Dot(lhs[1], rhs),
			Dot(lhs[2], rhs)
		};
	}

	template<typename T, PackingMode P>
	Vector<T, 3, P> operator*(const Vector<T, 3, P>& lhs, const Matrix<T, 3, 3, P>& rhs)
	{
		return (lhs[0] * rhs[0]) + (lhs[1] * rhs[1]) + (lhs[2] * rhs[2]);
	}

	template<typename T, PackingMode P>
	const Vector<T, 3, P>& operator*=(Vector<T, 3, P>& lhs, const Matrix<T, 3, 3, P>& rhs)
	{
		return lhs = (lhs * rhs);
	}

#pragma endregion


#pragma region Functions

	template<typename T, PackingMode P>
	Matrix<T, 3, 3, P>::TransposeType Transpose(const Matrix<T, 3, 3, P>& matrix)
	{
		return typename Matrix<T, 3, 3, P>::TransposeType{
			matrix[0][0], matrix[1][0], matrix[2][0],
			matrix[0][1], matrix[1][1], matrix[2][1],
			matrix[0][2], matrix[1][2], matrix[2][2]
		};
	}

	template<typename T, PackingMode P>
	T Determinant(const Matrix<T, 3, 3, P>& matrix)
	{
		// 2x2 determinants, numbers are the indexs of the columns
		const T determinant01 = (matrix[1][0] * matrix[2][1]) - (matrix[1][1] * matrix[2][0]);
		const T determinant02 = (matrix[1][0] * matrix[2][2]) - (matrix[1][2] * matrix[2][0]);
		const T determinant12 = (matrix[1][1] * matrix[2][2]) - (matrix[1][2] * matrix[2][1]);

		return (matrix[0][0] * determinant12) - (matrix[0][1] * determinant02) + (matrix[0][2] * determinant01);
	}

#pragma endregion

#pragma region Member version of functions

	template<typename T, PackingMode P>
	inline Matrix<T, 3, 3, P>::TransposeType Matrix<T, 3, 3, P>::Transpose() const { return PWMath::Transpose(*this); }

	template<typename T, PackingMode P>
	inline T Matrix<T, 3, 3, P>::Determinant() const { return PWMath::Determinant(*this); }

#pragma endregion

}
