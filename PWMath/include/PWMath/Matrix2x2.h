#pragma once
#include "PWMath/Matrix.h"
#include "PWMath/Vector2.h"

#if PWM_DEFINE_OSTREAM
#include <ostream>
#endif // PWM_DEFINE_OSTREAM
#include <cmath>

namespace PWMath
{
	// 2x2 template specialization of Matrix
	template<typename T, PackingMode P>
	struct Matrix<T, 2, 2, P>
	{
		using Type = T;
		using ColumnType = Vector2<T, P>;
		using RowType = Vector2<T, P>;
		using TransposeType = Matrix<T, 2, 2, P>;
		static constexpr size_t colomns = 2;
		static constexpr size_t rows = 2;
		static constexpr PackingMode packingMode = P;

		union
		{
			struct { Vector2<T, P> x, y; };
			struct { Vector2<T, P> r, b; };
			Vector2<T, P> array[2];
		};

		// Default constructors
		Matrix() = default;
		Matrix(const Matrix&) = default;
		~Matrix() = default;

		// Special constructors
		template<typename TVal>
		Matrix(TVal identityVal)
			:array{
				{ static_cast<T>(identityVal), static_cast<T>(0) },
				{ static_cast<T>(0), static_cast<T>(identityVal) } }
		{}

		// NOTE: Each parameter is a row
		template<typename TX, typename TY, PackingMode PX, PackingMode PY>
		Matrix(Vector2<TX, PX> x, Vector2<TY, PY> y)
			:array{ Vector2<T, P>{ x }, Vector2<T, P>{ y } }
		{}

		template<typename TMat, PackingMode PMat>
		Matrix(Matrix<TMat, 2, 2, PMat> matrix)
			: array{ Vector2<T, P>{ matrix[0] }, Vector2<T, P>{ matrix[1] } }
		{}

		// NOTE: Row major ordering
		template<
			typename T00, typename T01,
			typename T10, typename T11>
		Matrix(
			T00 _00, T01 _01,
			T10 _10, T11 _11)
			:array{
				{ static_cast<T>(_00), static_cast<T>(_01) },
				{ static_cast<T>(_10), static_cast<T>(_11) } }
		{}

		// NOTE: Row major ordering
		template<typename TArr>
		Matrix(TArr(&vals)[4])
			:array{
				{ static_cast<T>(vals[0]), static_cast<T>(vals[1]) },
				{ static_cast<T>(vals[2]), static_cast<T>(vals[3]) } }
		{}

		RowType GetRow(size_t index) const { return array[index]; }
		ColumnType GetColumn(size_t index) const { return RowType{ x[index], y[index] }; }

		ColumnType& operator[](size_t index) { return array[index]; }
		const ColumnType& operator[](size_t index) const { return array[index]; }

		Matrix& operator=(const Matrix&) = default;

		bool operator==(const Matrix&) const = default;

		TransposeType Transpose() const;
		T Determinant() const;
	};

	// Unary plus and minus
	template<typename T, PackingMode P>
	Matrix<T, 2, 2, P> operator+(const Matrix<T, 2, 2, P>& rhs);
	template<typename T, PackingMode P>
	Matrix<T, 2, 2, P> operator-(const Matrix<T, 2, 2, P>& rhs);

	// Addition and substraction
	template<typename T, PackingMode P>
	Matrix<T, 2, 2, P> operator+(const Matrix<T, 2, 2, P>& lhs, const Matrix<T, 2, 2, P>& rhs);
	template<typename T, PackingMode P>
	Matrix<T, 2, 2, P> operator-(const Matrix<T, 2, 2, P>& lhs, const Matrix<T, 2, 2, P>& rhs);
	template<typename T, PackingMode P>
	const Matrix<T, 2, 2, P>& operator+=(Matrix<T, 2, 2, P>& lhs, const Matrix<T, 2, 2, P>& rhs);
	template<typename T, PackingMode P>
	const Matrix<T, 2, 2, P>& operator-=(Matrix<T, 2, 2, P>& lhs, const Matrix<T, 2, 2, P>& rhs);

	// Matrix-scalar multiplication
	template<typename T, PackingMode P>
	Matrix<T, 2, 2, P> operator*(const Matrix<T, 2, 2, P>& lhs, T rhs);
	template<typename T, PackingMode P>
	const Matrix<T, 2, 2, P>& operator*=(Matrix<T, 2, 2, P>& lhs, T rhs);

	template<typename T, PackingMode P>
	Matrix<T, 2, 2, P> operator*(T lhs, const Matrix<T, 2, 2, P>& rhs);

	// Matrix multiplication
	template<typename T, PackingMode P>
	Matrix<T, 2, 2, P> operator*(const Matrix<T, 2, 2, P>& lhs, const Matrix<T, 2, 2, P>& rhs);
	template<typename T, PackingMode P>
	const Matrix<T, 2, 2, P>& operator*=(Matrix<T, 2, 2, P>& lhs, const Matrix<T, 2, 2, P>& rhs);

	// Matrix-vector multiplication
	template<typename T, PackingMode P>
	Vector<T, 2, P> operator*(const Matrix<T, 2, 2, P>& lhs, const Vector<T, 2, P>& rhs);
	template<typename T, PackingMode P>
	Vector<T, 2, P> operator*(const Vector<T, 2, P>& lhs, const Matrix<T, 2, 2, P>& rhs);
	template<typename T, PackingMode P>
	const Vector<T, 2, P>& operator*=(Vector<T, 2, P>& lhs, const Matrix<T, 2, 2, P>& rhs);

	template<typename T, PackingMode P>
	Matrix<T, 2, 2, P>::TransposeType Transpose(const Matrix<T, 2, 2, P>& matrix);

	template<typename T, PackingMode P>
	T Determinant(const Matrix<T, 2, 2, P>& matrix);

#if PWM_DEFINE_OSTREAM
	// Prints as row major
	template<typename T, PackingMode P>
	inline std::ostream& operator<<(std::ostream& stream, Matrix<T, 2, 2, P> matrix)
	{
		stream << "[[" << matrix[0][0] << ", " << matrix[0][1] << "], "
			<< '[' << matrix[1][0] << ", " << matrix[1][1] << "]]";
		return stream;
	}

#endif // PWM_DEFINE_OSTREAM

	template<typename T, PackingMode P = PackingMode::Default>
	using Matrix2x2 = Matrix<T, 2, 2, P>;

	using Matrix2x2F32 = Matrix2x2<float>;
	using Matrix2x2F64 = Matrix2x2<double>;
	using Matrix2x2I8 = Matrix2x2<int8_t>;
	using Matrix2x2I16 = Matrix2x2<int16_t>;
	using Matrix2x2I32 = Matrix2x2<int32_t>;
	using Matrix2x2I64 = Matrix2x2<int64_t>;
	using Matrix2x2U8 = Matrix2x2<uint8_t>;
	using Matrix2x2U16 = Matrix2x2<uint16_t>;
	using Matrix2x2U32 = Matrix2x2<uint32_t>;
	using Matrix2x2U64 = Matrix2x2<uint64_t>;
}

#include <PWMath/Impl/Matrix2x2.inl>
