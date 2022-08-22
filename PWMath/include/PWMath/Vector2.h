#pragma once
#include <PWMath/Vector.h>

#if PWM_DEFINE_OSTREAM
#include <ostream>
#endif // PWM_DEFINE_OSTREAM
#include <cmath>

namespace PWMath
{
	// 2 component template specialization of Vector
	template<typename T, PackingMode P>
	struct Vector<T, 2, P>
	{
	public:
		using Type = T;
		static constexpr size_t size = 2;
		static constexpr PackingMode packingMode = P;

		union
		{
			struct { T x, y; };
			struct { T r, g; };
			T array[2];
		};

		// Default constuctors and destructors
		Vector() = default;
		Vector(const Vector&) = default;
		~Vector() = default;
		
		// Special constructors and destructors
		template<typename TVal>
		constexpr Vector(TVal values) noexcept :array{ static_cast<T>(values), static_cast<T>(values) } {}
		template<typename TX, typename TY>
		constexpr Vector(TX x, TY y) noexcept :array{ static_cast<T>(x), static_cast<T>(y) } {}
		template<typename TVec, PackingMode PVec>
		constexpr Vector(const Vector<TVec, 2, PVec>& rhs) noexcept :array{ static_cast<T>(rhs.x), static_cast<T>(rhs.y) } {}
		template<typename TArr>
		constexpr Vector(TArr (&values)[2]) noexcept :array{ static_cast<T>(values[0]), static_cast<T>(values[1]) } {}

		constexpr T& operator[](size_t index) { return array[index]; }
		constexpr const T& operator[](size_t index) const { return array[index]; }

		Vector& operator=(const Vector& rhs) = default;

		bool operator==(const Vector& rhs) const = default;

		constexpr T Length() const;
		constexpr T Length2() const;
		constexpr Vector Normalize() const;
		constexpr T Dot(const Vector<T, 2, P>& rhs) const;

		constexpr Vector<T, 2, P> Swizzle(size_t index0, size_t index1) const { return Vector<T, 2, P>{ array[index0], array[index1] }; }
		constexpr Vector<T, 3, P> Swizzle(size_t index0, size_t index1, size_t index2) const { return Vector<T, 3, P>{ array[index0], array[index1], array[index2] }; }
		constexpr Vector<T, 4, P> Swizzle(size_t index0, size_t index1, size_t index2, size_t index3) const { return Vector<T, 4, P>{ array[index0], array[index1], array[index2], array[index3] }; }
	};

	template<typename T, PackingMode P>
	constexpr Vector<T, 2, P> operator+(const Vector<T, 2, P>& vector) noexcept;
	template<typename T, PackingMode P>
	constexpr Vector<T, 2, P> operator-(const Vector<T, 2, P>& vector) noexcept;

	template<typename T, PackingMode P>
	constexpr Vector<T, 2, P> operator+(const Vector<T, 2, P>& lhs, T rhs) noexcept;
	template<typename T, PackingMode P>
	constexpr Vector<T, 2, P> operator-(const Vector<T, 2, P>& lhs, T rhs) noexcept;
	template<typename T, PackingMode P>
	constexpr Vector<T, 2, P> operator*(const Vector<T, 2, P>& lhs, T rhs) noexcept;
	template<typename T, PackingMode P>
	constexpr Vector<T, 2, P> operator/(const Vector<T, 2, P>& lhs, T rhs) noexcept;
	template<typename T, PackingMode P>	
	constexpr const Vector<T, 2, P>& operator+=(Vector<T, 2, P>& lhs, T rhs) noexcept;
	template<typename T, PackingMode P>	
	constexpr const Vector<T, 2, P>& operator-=(Vector<T, 2, P>& lhs, T rhs) noexcept;
	template<typename T, PackingMode P>	
	constexpr const Vector<T, 2, P>& operator*=(Vector<T, 2, P>& lhs, T rhs) noexcept;
	template<typename T, PackingMode P>	
	constexpr const Vector<T, 2, P>& operator/=(Vector<T, 2, P>& lhs, T rhs) noexcept;

	template<typename T, PackingMode P>	
	constexpr Vector<T, 2, P> operator+(const Vector<T, 2, P>& lhs, const Vector<T, 2, P>& rhs) noexcept;
	template<typename T, PackingMode P>	
	constexpr Vector<T, 2, P> operator-(const Vector<T, 2, P>& lhs, const Vector<T, 2, P>& rhs) noexcept;
	template<typename T, PackingMode P>	
	constexpr Vector<T, 2, P> operator*(const Vector<T, 2, P>& lhs, const Vector<T, 2, P>& rhs) noexcept;
	template<typename T, PackingMode P>	
	constexpr Vector<T, 2, P> operator/(const Vector<T, 2, P>& lhs, const Vector<T, 2, P>& rhs) noexcept;
	template<typename T, PackingMode P>	
	constexpr const Vector<T, 2, P>& operator+=(Vector<T, 2, P>& lhs, const Vector<T, 2, P>& rhs) noexcept;
	template<typename T, PackingMode P>	
	constexpr const Vector<T, 2, P>& operator-=(Vector<T, 2, P>& lhs, const Vector<T, 2, P>& rhs) noexcept;
	template<typename T, PackingMode P>	
	constexpr const Vector<T, 2, P>& operator*=(Vector<T, 2, P>& lhs, const Vector<T, 2, P>& rhs) noexcept;
	template<typename T, PackingMode P>	
	constexpr const Vector<T, 2, P>& operator/=(Vector<T, 2, P>& lhs, const Vector<T, 2, P>& rhs) noexcept;

	template<typename T, PackingMode P>
	constexpr Vector<T, 2, P> operator+(T lhs, const Vector<T, 2, P>& rhs) noexcept;
	template<typename T, PackingMode P>
	constexpr Vector<T, 2, P> operator-(T lhs, const Vector<T, 2, P>& rhs) noexcept;
	template<typename T, PackingMode P>
	constexpr Vector<T, 2, P> operator*(T lhs, const Vector<T, 2, P>& rhs) noexcept;
	template<typename T, PackingMode P>
	constexpr Vector<T, 2, P> operator/(T lhs, const Vector<T, 2, P>& rhs) noexcept;

	template<typename T, PackingMode P>
	constexpr T Length(const Vector<T, 2, P>& vector);

	template<typename T, PackingMode P>
	constexpr T Length2(const Vector<T, 2, P>& vector);

	template<typename T, PackingMode P>
	constexpr Vector<T, 2, P> Normalize(const Vector<T, 2, P>& vector);

	template<typename T, PackingMode P>
	constexpr T Dot(const Vector<T, 2, P>& lhs, const Vector<T, 2, P>& rhs);

#if PWM_DEFINE_OSTREAM
	template<typename T, PackingMode P>
	inline std::ostream& operator<<(std::ostream& stream, Vector<T, 2, P> vector)
	{
		stream << '[' << vector.x << ", " << vector.y << ']';
		return stream;
	}
#endif // PWM_DEFINE_OSTREAM

	template<typename T, PackingMode P = PackingMode::Default>
	using Vector2 = Vector<T, 2, P>;

	using Vector2F32	= Vector2<float>;
	using Vector2F64	= Vector2<double>;
	using Vector2I8		= Vector2<int8_t>;
	using Vector2I16	= Vector2<int16_t>;
	using Vector2I32	= Vector2<int32_t>;
	using Vector2I64	= Vector2<int64_t>;
	using Vector2U8		= Vector2<uint8_t>;
	using Vector2U16	= Vector2<uint16_t>;
	using Vector2U32	= Vector2<uint32_t>;
	using Vector2U64	= Vector2<uint64_t>;
}

#include <PWMath/Impl/Vector2.inl>
