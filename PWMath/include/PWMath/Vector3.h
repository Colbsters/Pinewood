#pragma once
#include <PWMath/Vector.h>

#if PWM_DEFINE_OSTREAM
#include <ostream>
#endif // PWM_DEFINE_OSTREAM
#include <cmath>

namespace PWMath
{
	// 3 component template specialization of Vector
	template<typename T, PackingMode P>
	struct Vector<T, 3, P>
	{
	public:
		using Type = T;
		static constexpr size_t size = 3;
		static constexpr PackingMode packingMode = P;

		union
		{
			struct { T x, y, z; };
			struct { T r, g, b; };
			T array[3];
		};

		// Default constuctors and destructors
		Vector() = default;
		Vector(const Vector&) = default;
		~Vector() = default;
		
		// Special constructors and destructors
		template<typename TVal>
		constexpr Vector(TVal values) noexcept :array{ static_cast<T>(values), static_cast<T>(values), static_cast<T>(values) } {}
		template<typename TX, typename TY, typename TZ>
		constexpr Vector(TX x, TY y, TZ z) noexcept :array{ static_cast<T>(x), static_cast<T>(y), static_cast<T>(z) } {}
		template<typename TVec, PackingMode PVec>
		constexpr Vector(const Vector<TVec, 2, PVec>& xy, TVec z) noexcept :array{ static_cast<T>(xy.x), static_cast<T>(xy.y), static_cast<T>(z) } {}
		template<typename TVec, PackingMode PVec>
		constexpr Vector(TVec x, const Vector<TVec, 2, PVec>& yz) noexcept :array{ static_cast<T>(x), static_cast<T>(yz.x), static_cast<T>(yz.y) } {}
		template<typename TVec, PackingMode PVec>
		constexpr Vector(const Vector<TVec, 3, PVec>& rhs) noexcept :array{ static_cast<T>(rhs.x), static_cast<T>(rhs.y),  static_cast<T>(rhs.z) } {}
		template<typename TArr>
		constexpr Vector(TArr (&values)[3]) noexcept :array{ static_cast<T>(values[0]), static_cast<T>(values[1]), static_cast<T>(values[2]) } {}

		constexpr T& operator[](size_t index) { return array[index]; }
		constexpr const T& operator[](size_t index) const { return array[index]; }

		Vector& operator=(const Vector& rhs) = default;

		bool operator==(const Vector& rhs) const = default;

		constexpr T Length() const;
		constexpr T Length2() const;
		constexpr Vector Normalize() const;
		constexpr T Dot(const Vector<T, 3, P>& rhs) const;
		constexpr Vector Cross(const Vector<T, 3, P>& rhs) const;

		constexpr Vector<T, 2, P> Swizzle(size_t index0, size_t index1) const { return Vector<T, 2, P>{ array[index0], array[index1] }; }
		constexpr Vector<T, 3, P> Swizzle(size_t index0, size_t index1, size_t index2) const { return Vector<T, 3, P>{ array[index0], array[index1], array[index2] }; }
		constexpr Vector<T, 4, P> Swizzle(size_t index0, size_t index1, size_t index2, size_t index3) const { return Vector<T, 4, P>{ array[index0], array[index1], array[index2], array[index3] }; }
	};

	template<typename T, PackingMode P>
	constexpr Vector<T, 3, P> operator+(const Vector<T, 3, P>& vector) noexcept;
	template<typename T, PackingMode P>
	constexpr Vector<T, 3, P> operator-(const Vector<T, 3, P>& vector) noexcept;

	template<typename T, PackingMode P>
	constexpr Vector<T, 3, P> operator+(const Vector<T, 3, P>& lhs, T rhs) noexcept;
	template<typename T, PackingMode P>
	constexpr Vector<T, 3, P> operator-(const Vector<T, 3, P>& lhs, T rhs) noexcept;
	template<typename T, PackingMode P>
	constexpr Vector<T, 3, P> operator*(const Vector<T, 3, P>& lhs, T rhs) noexcept;
	template<typename T, PackingMode P>
	constexpr Vector<T, 3, P> operator/(const Vector<T, 3, P>& lhs, T rhs) noexcept;
	template<typename T, PackingMode P>	
	constexpr const Vector<T, 3, P>& operator+=(Vector<T, 3, P>& lhs, T rhs) noexcept;
	template<typename T, PackingMode P>	
	constexpr const Vector<T, 3, P>& operator-=(Vector<T, 3, P>& lhs, T rhs) noexcept;
	template<typename T, PackingMode P>	
	constexpr const Vector<T, 3, P>& operator*=(Vector<T, 3, P>& lhs, T rhs) noexcept;
	template<typename T, PackingMode P>	
	constexpr const Vector<T, 3, P>& operator/=(Vector<T, 3, P>& lhs, T rhs) noexcept;

	template<typename T, PackingMode P>	
	constexpr Vector<T, 3, P> operator+(const Vector<T, 3, P>& lhs, const Vector<T, 3, P>& rhs) noexcept;
	template<typename T, PackingMode P>	
	constexpr Vector<T, 3, P> operator-(const Vector<T, 3, P>& lhs, const Vector<T, 3, P>& rhs) noexcept;
	template<typename T, PackingMode P>	
	constexpr Vector<T, 3, P> operator*(const Vector<T, 3, P>& lhs, const Vector<T, 3, P>& rhs) noexcept;
	template<typename T, PackingMode P>	
	constexpr Vector<T, 3, P> operator/(const Vector<T, 3, P>& lhs, const Vector<T, 3, P>& rhs) noexcept;
	template<typename T, PackingMode P>	
	constexpr const Vector<T, 3, P>& operator+=(Vector<T, 3, P>& lhs, const Vector<T, 3, P>& rhs) noexcept;
	template<typename T, PackingMode P>	
	constexpr const Vector<T, 3, P>& operator-=(Vector<T, 3, P>& lhs, const Vector<T, 3, P>& rhs) noexcept;
	template<typename T, PackingMode P>	
	constexpr const Vector<T, 3, P>& operator*=(Vector<T, 3, P>& lhs, const Vector<T, 3, P>& rhs) noexcept;
	template<typename T, PackingMode P>	
	constexpr const Vector<T, 3, P>& operator/=(Vector<T, 3, P>& lhs, const Vector<T, 3, P>& rhs) noexcept;

	template<typename T, PackingMode P>
	constexpr Vector<T, 3, P> operator+(T lhs, const Vector<T, 3, P>& rhs) noexcept;
	template<typename T, PackingMode P>
	constexpr Vector<T, 3, P> operator-(T lhs, const Vector<T, 3, P>& rhs) noexcept;
	template<typename T, PackingMode P>
	constexpr Vector<T, 3, P> operator*(T lhs, const Vector<T, 3, P>& rhs) noexcept;
	template<typename T, PackingMode P>
	constexpr Vector<T, 3, P> operator/(T lhs, const Vector<T, 3, P>& rhs) noexcept;

	template<typename T, PackingMode P>
	constexpr T Length(const Vector<T, 3, P>& vector);

	template<typename T, PackingMode P>
	constexpr T Length2(const Vector<T, 3, P>& vector);

	template<typename T, PackingMode P>
	constexpr Vector<T, 3, P> Normalize(const Vector<T, 3, P>& vector);

	template<typename T, PackingMode P>
	constexpr T Dot(const Vector<T, 3, P>& lhs, const Vector<T, 3, P>& rhs);

	template<typename T, PackingMode P>
	constexpr Vector<T, 3, P> Cross(const Vector<T, 3, P>& lhs, const Vector<T, 3, P>& rhs);

#if PWM_DEFINE_OSTREAM
	template<typename T, PackingMode P>
	inline std::ostream& operator<<(std::ostream& stream, Vector<T, 3, P> vector)
	{
		stream << '[' << vector.x << ", " << vector.y << ", " << vector.z << ']';
		return stream;
	}
#endif // PWM_DEFINE_OSTREAM

	template<typename T, PackingMode P = PackingMode::Default>
	using Vector3 = Vector<T, 3, P>;

	using Vector3F32	= Vector3<float>;
	using Vector3F64	= Vector3<double>;
	using Vector3I8		= Vector3<int8_t>;
	using Vector3I16	= Vector3<int16_t>;
	using Vector3I32	= Vector3<int32_t>;
	using Vector3I64	= Vector3<int64_t>;
	using Vector3U8		= Vector3<uint8_t>;
	using Vector3U16	= Vector3<uint16_t>;
	using Vector3U32	= Vector3<uint32_t>;
	using Vector3U64	= Vector3<uint64_t>;
}

#include <PWMath/Impl/Vector3.inl>
