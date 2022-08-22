#pragma once
#include "PWMath/Matrix.h"
#include "PWMath/Vector3.h"

#if PWM_DEFINE_OSTREAM
#include <ostream>
#endif // PWM_DEFINE_OSTREAM
#include <cmath>

namespace PWMath
{
	// 3x3 template specialization of Matrix
	template<typename T, PackingMode P>
	struct Matrix<T, 3, 3, P>
	{
		using Type = T;
		using ColumnType = Vector3<T, P>;
		using RowType = Vector3<T, P>;
		using TransposeType = Matrix<T, 3, 3, P>;
		static constexpr size_t colomns = 3;
		static constexpr size_t rows = 3;
		static constexpr PackingMode packingMode = P;

		union
		{
			struct { Vector3<T, P> x, y, z; };
			struct { Vector3<T, P> r, b, g; };
			Vector3<T, P> array[3];
		};

		// Default constructors
		Matrix() = default;
		Matrix(const Matrix&) = default;
		~Matrix() = default;

		// Special constructors
		template<typename TVal>
		Matrix(TVal identityVal)
			:array{
				{ static_cast<T>(identityVal), static_cast<T>(0), static_cast<T>(0) },
				{ static_cast<T>(0), static_cast<T>(identityVal), static_cast<T>(0) },
				{ static_cast<T>(0), static_cast<T>(0), static_cast<T>(identityVal) } }
		{}

		// NOTE: Each parameter is a row
		template<typename TX, typename TY, typename TZ, PackingMode PX, PackingMode PY, PackingMode PZ>
		Matrix(Vector3<TX, PX> x, Vector3<TY, PY> y, Vector3<TZ, PZ> z)
			:array{ Vector3<T, P>{ x }, Vector3<T, P>{ y }, Vector3<T, P>{ z } }
		{}

		template<typename TMat, PackingMode PMat>
		Matrix(Matrix<TMat, 3, 3, PMat> matrix)
			: array{ Vector3<T, P>{ matrix[0] }, Vector3<T, P>{ matrix[1] }, Vector3<T, P>{ matrix[2] } }
		{}

		// NOTE: Row major ordering
		template<
			typename T00, typename T01, typename T02,
			typename T10, typename T11, typename T12,
			typename T20, typename T21, typename T22>
		Matrix(
			T00 _00, T01 _01, T02 _02,
			T10 _10, T11 _11, T12 _12,
			T20 _20, T21 _21, T22 _22)
			:array{
				{ static_cast<T>(_00), static_cast<T>(_01), static_cast<T>(_02) },
				{ static_cast<T>(_10), static_cast<T>(_11), static_cast<T>(_12) },
				{ static_cast<T>(_20), static_cast<T>(_21), static_cast<T>(_22) } }
		{}

		// NOTE: Row major ordering
		template<typename TArr>
		Matrix(TArr(&vals)[9])
			:array{
				{ static_cast<T>(vals[0]), static_cast<T>(vals[1]), static_cast<T>(vals[2]) },
				{ static_cast<T>(vals[3]), static_cast<T>(vals[4]), static_cast<T>(vals[5]) },
				{ static_cast<T>(vals[6]), static_cast<T>(vals[7]), static_cast<T>(vals[9]) } }
		{}

		RowType GetRow(size_t index) const { return array[index]; }
		ColumnType GetColumn(size_t index) const { return RowType{ x[index], y[index], z[index] }; }

		ColumnType& operator[](size_t index) { return array[index]; }
		const ColumnType& operator[](size_t index) const { return array[index]; }

		Matrix& operator=(const Matrix&) = default;

		bool operator==(const Matrix&) const = default;

		TransposeType Transpose() const;
		T Determinant() const;
	};

	// Unary plus and minus
	template<typename T, PackingMode P>
	Matrix<T, 3, 3, P> operator+(const Matrix<T, 3, 3, P>& rhs);
	template<typename T, PackingMode P>
	Matrix<T, 3, 3, P> operator-(const Matrix<T, 3, 3, P>& rhs);

	// Addition and substraction
	template<typename T, PackingMode P>
	Matrix<T, 3, 3, P> operator+(const Matrix<T, 3, 3, P>& lhs, const Matrix<T, 3, 3, P>& rhs);
	template<typename T, PackingMode P>
	Matrix<T, 3, 3, P> operator-(const Matrix<T, 3, 3, P>& lhs, const Matrix<T, 3, 3, P>& rhs);
	template<typename T, PackingMode P>
	const Matrix<T, 3, 3, P>& operator+=(Matrix<T, 3, 3, P>& lhs, const Matrix<T, 3, 3, P>& rhs);
	template<typename T, PackingMode P>
	const Matrix<T, 3, 3, P>& operator-=(Matrix<T, 3, 3, P>& lhs, const Matrix<T, 3, 3, P>& rhs);

	// Matrix-scalar multiplication
	template<typename T, PackingMode P>
	Matrix<T, 3, 3, P> operator*(const Matrix<T, 3, 3, P>& lhs, T rhs);
	template<typename T, PackingMode P>
	const Matrix<T, 3, 3, P>& operator*=(Matrix<T, 3, 3, P>& lhs, T rhs);

	template<typename T, PackingMode P>
	Matrix<T, 3, 3, P> operator*(T lhs, const Matrix<T, 3, 3, P>& rhs);

	// Matrix multiplication
	template<typename T, PackingMode P>
	Matrix<T, 3, 3, P> operator*(const Matrix<T, 3, 3, P>& lhs, const Matrix<T, 3, 3, P>& rhs);
	template<typename T, PackingMode P>
	const Matrix<T, 3, 3, P>& operator*=(Matrix<T, 3, 3, P>& lhs, const Matrix<T, 3, 3, P>& rhs);

	// Matrix-vector multiplication
	template<typename T, PackingMode P>
	Vector<T, 3, P> operator*(const Matrix<T, 3, 3, P>& lhs, const Vector<T, 3, P>& rhs);
	template<typename T, PackingMode P>
	Vector<T, 3, P> operator*(const Vector<T, 3, P>& lhs, const Matrix<T, 3, 3, P>& rhs);
	template<typename T, PackingMode P>
	const Vector<T, 3, P>& operator*=(Vector<T, 3, P>& lhs, const Matrix<T, 3, 3, P>& rhs);

	template<typename T, PackingMode P>
	Matrix<T, 3, 3, P>::TransposeType Transpose(const Matrix<T, 3, 3, P>& matrix);

	template<typename T, PackingMode P>
	T Determinant(const Matrix<T, 3, 3, P>& matrix);

#if PWM_DEFINE_OSTREAM
	// Prints as row major
	template<typename T, PackingMode P>
	inline std::ostream& operator<<(std::ostream& stream, Matrix<T, 3, 3, P> matrix)
	{
		stream << "[[" << matrix[0][0] << ", " << matrix[0][1] << ", " << matrix[0][2] << "], "
			<< '[' << matrix[1][0] << ", " << matrix[1][1] << ", " << matrix[1][2] << "], "
			<< '[' << matrix[2][0] << ", " << matrix[2][1] << ", " << matrix[2][2] << "]]";
		return stream;
	}

#endif // PWM_DEFINE_OSTREAM

	template<typename T, PackingMode P = PackingMode::Default>
	using Matrix3x3 = Matrix<T, 3, 3, P>;

	using Matrix3x3F32 = Matrix3x3<float>;
	using Matrix3x3F64 = Matrix3x3<double>;
	using Matrix3x3I8 = Matrix3x3<int8_t>;
	using Matrix3x3I16 = Matrix3x3<int16_t>;
	using Matrix3x3I32 = Matrix3x3<int32_t>;
	using Matrix3x3I64 = Matrix3x3<int64_t>;
	using Matrix3x3U8 = Matrix3x3<uint8_t>;
	using Matrix3x3U16 = Matrix3x3<uint16_t>;
	using Matrix3x3U32 = Matrix3x3<uint32_t>;
	using Matrix3x3U64 = Matrix3x3<uint64_t>;
}

#include <PWMath/Impl/Matrix3x3.inl>
