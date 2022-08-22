#pragma once
#include "PWMath/Matrix.h"
#include "PWMath/Vector4.h"

#if PWM_DEFINE_OSTREAM
#include <ostream>
#endif // PWM_DEFINE_OSTREAM
#include <cmath>

namespace PWMath
{
	// 4x4 template specialization of Matrix
	template<typename T, PackingMode P>
	struct Matrix<T, 4, 4, P>
	{
		using Type = T;
		using ColumnType = Vector4<T, P>;
		using RowType = Vector4<T, P>;
		using TransposeType = Matrix<T, 4, 4, P>;
		static constexpr size_t colomns = 4;
		static constexpr size_t rows = 4;
		static constexpr PackingMode packingMode = P;

		union
		{
			struct { Vector4<T, P> x, y, z, w; };
			struct { Vector4<T, P> r, b, g, a; };
			Vector4<T, P> array[4];
		};

		// Default constructors
		Matrix() = default;
		Matrix(const Matrix&) = default;
		~Matrix() = default;

		// Special constructors
		template<typename TVal>
		Matrix(TVal identityVal)
			:array{
				{ static_cast<T>(identityVal), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0) },
				{ static_cast<T>(0), static_cast<T>(identityVal), static_cast<T>(0), static_cast<T>(0) },
				{ static_cast<T>(0), static_cast<T>(0), static_cast<T>(identityVal), static_cast<T>(0) },
				{ static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(identityVal) } }
		{}

		// NOTE: Each parameter is a row
		template<typename TX, typename TY, typename TZ, typename TW, PackingMode PX, PackingMode PY, PackingMode PZ, PackingMode PW>
		Matrix(Vector4<TX, PX> x, Vector4<TY, PY> y, Vector4<TZ, PZ> z, Vector4<TW, PW> w)
			:array{ Vector4<T, P>{ x }, Vector4<T, P>{ y }, Vector4<T, P>{ z }, Vector4<T, P>{ w } }
		{}

		template<typename TMat, PackingMode PMat>
		Matrix(Matrix<TMat, 4, 4, PMat> matrix)
			: array{ Vector4<T, P>{ matrix[0] }, Vector4<T, P>{ matrix[1] }, Vector4<T, P>{ matrix[2] }, Vector4<T, P>{ matrix[3] } }
		{}

		// NOTE: Row major ordering
		template<
			typename T00, typename T01, typename T02, typename T03,
			typename T10, typename T11, typename T12, typename T13,
			typename T20, typename T21, typename T22, typename T23,
			typename T30, typename T31, typename T32, typename T33>
		Matrix(
			T00 _00, T01 _01, T02 _02, T03 _03,
			T10 _10, T11 _11, T12 _12, T13 _13,
			T20 _20, T21 _21, T22 _22, T23 _23,
			T30 _30, T31 _31, T32 _32, T33 _33)
			:array{
				{ static_cast<T>(_00), static_cast<T>(_01), static_cast<T>(_02), static_cast<T>(_03) },
				{ static_cast<T>(_10), static_cast<T>(_11), static_cast<T>(_12), static_cast<T>(_13) },
				{ static_cast<T>(_20), static_cast<T>(_21), static_cast<T>(_22), static_cast<T>(_23) },
				{ static_cast<T>(_30), static_cast<T>(_31), static_cast<T>(_32), static_cast<T>(_33) } }
		{}

		// NOTE: Row major ordering
		template<typename TArr>
		Matrix(TArr(&vals)[16])
			:array{
				{ static_cast<T>(vals[0]), static_cast<T>(vals[1]), static_cast<T>(vals[2]), static_cast<T>(vals[3]) },
				{ static_cast<T>(vals[4]), static_cast<T>(vals[5]), static_cast<T>(vals[6]), static_cast<T>(vals[7]) },
				{ static_cast<T>(vals[8]), static_cast<T>(vals[9]), static_cast<T>(vals[10]), static_cast<T>(vals[11]) },
				{ static_cast<T>(vals[12]), static_cast<T>(vals[13]), static_cast<T>(vals[14]), static_cast<T>(vals[15]) } }
		{}

		RowType GetRow(size_t index) const { return array[index]; }
		ColumnType GetColumn(size_t index) const { return RowType{ x[index], y[index], z[index], w[index] }; }

		ColumnType& operator[](size_t index) { return array[index]; }
		const ColumnType& operator[](size_t index) const { return array[index]; }

		Matrix& operator=(const Matrix&) = default;

		bool operator==(const Matrix&) const = default;

		TransposeType Transpose() const;
		T Determinant() const;
	};

	// Unary operators
	template<typename T, PackingMode P>
	Matrix<T, 4, 4, P> operator+(const Matrix<T, 4, 4, P>& rhs);
	template<typename T, PackingMode P>
	Matrix<T, 4, 4, P> operator-(const Matrix<T, 4, 4, P>& rhs);

	// Addition and substraction
	template<typename T, PackingMode P>
	Matrix<T, 4, 4, P> operator+(const Matrix<T, 4, 4, P>& lhs, const Matrix<T, 4, 4, P>& rhs);
	template<typename T, PackingMode P>
	Matrix<T, 4, 4, P> operator-(const Matrix<T, 4, 4, P>& lhs, const Matrix<T, 4, 4, P>& rhs);
	template<typename T, PackingMode P>
	const Matrix<T, 4, 4, P>& operator+=(Matrix<T, 4, 4, P>& lhs, const Matrix<T, 4, 4, P>& rhs);
	template<typename T, PackingMode P>
	const Matrix<T, 4, 4, P>& operator-=(Matrix<T, 4, 4, P>& lhs, const Matrix<T, 4, 4, P>& rhs);

	// Matrix-scalar multiplication
	template<typename T, PackingMode P>
	Matrix<T, 4, 4, P> operator*(const Matrix<T, 4, 4, P>& lhs, T rhs);
	template<typename T, PackingMode P>
	const Matrix<T, 4, 4, P>& operator*=(Matrix<T, 4, 4, P>& lhs, T rhs);

	template<typename T, PackingMode P>
	Matrix<T, 4, 4, P> operator*(T lhs, const Matrix<T, 4, 4, P>& rhs);

	// Matrix multiplication
	template<typename T, PackingMode P>
	Matrix<T, 4, 4, P> operator*(const Matrix<T, 4, 4, P>& lhs, const Matrix<T, 4, 4, P>& rhs);
	template<typename T, PackingMode P>
	const Matrix<T, 4, 4, P>& operator*=(Matrix<T, 4, 4, P>& lhs, const Matrix<T, 4, 4, P>& rhs);

	// Matrix-vector multiplication
	template<typename T, PackingMode P>
	Vector<T, 4, P> operator*(const Matrix<T, 4, 4, P>& lhs, const Vector<T, 4, P>& rhs);
	template<typename T, PackingMode P>
	Vector<T, 4, P> operator*(const Vector<T, 4, P>& lhs, const Matrix<T, 4, 4, P>& rhs);
	template<typename T, PackingMode P>
	const Vector<T, 4, P>& operator*=(Vector<T, 4, P>& lhs, const Matrix<T, 4, 4, P>& rhs);

	template<typename T, PackingMode P>
	Matrix<T, 4, 4, P>::TransposeType Transpose(const Matrix<T, 4, 4, P>& matrix);

	template<typename T, PackingMode P>
	T Determinant(const Matrix<T, 4, 4, P>& matrix);

#if PWM_DEFINE_OSTREAM
	// Prints as row major
	template<typename T, PackingMode P>
	inline std::ostream& operator<<(std::ostream& stream, Matrix<T, 4, 4, P> matrix)
	{
		stream << "[[" << matrix[0][0] << ", " << matrix[0][1] << ", " << matrix[0][2] << ", " << matrix[0][3] << "], "
			<< '[' << matrix[1][0] << ", " << matrix[1][1] << ", " << matrix[1][2] << ", " << matrix[1][3] << "], "
			<< '[' << matrix[2][0] << ", " << matrix[2][1] << ", " << matrix[2][2] << ", " << matrix[2][3] << "], "
			<< '[' << matrix[3][0] << ", " << matrix[3][1] << ", " << matrix[3][2] << ", " << matrix[3][3] << "]]";
		return stream;
	}

#endif // PWM_DEFINE_OSTREAM

	template<typename T, PackingMode P = PackingMode::Default>
	using Matrix4x4 = Matrix<T, 4, 4, P>;

	using Matrix4x4F32 = Matrix4x4<float>;
	using Matrix4x4F64 = Matrix4x4<double>;
	using Matrix4x4I8 = Matrix4x4<int8_t>;
	using Matrix4x4I16 = Matrix4x4<int16_t>;
	using Matrix4x4I32 = Matrix4x4<int32_t>;
	using Matrix4x4I64 = Matrix4x4<int64_t>;
	using Matrix4x4U8 = Matrix4x4<uint8_t>;
	using Matrix4x4U16 = Matrix4x4<uint16_t>;
	using Matrix4x4U32 = Matrix4x4<uint32_t>;
	using Matrix4x4U64 = Matrix4x4<uint64_t>;
}

#include <PWMath/Impl/Matrix4x4.inl>
