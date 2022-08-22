#pragma once
#include <PWMath/Vector.h>

#if PWM_DEFINE_OSTREAM
#include <ostream>
#endif // PWM_DEFINE_OSTREAM
#include <cmath>

namespace PWMath
{
	// 4 component template specialization of Vector
	template<typename T, PackingMode P>
	struct Vector<T, 4, P>
	{
	public:
		using Type = T;
		static constexpr size_t size = 4;
		static constexpr PackingMode packingMode = P;

		union
		{
			struct { T x, y, z, w; };
			struct { T r, g, b, a; };
			T array[4];
		};

		// Default constuctors and destructors
		constexpr Vector() = default;
		constexpr Vector(const Vector&) = default;
		constexpr ~Vector() = default;
		
		// Special constructors and destructors
		template<typename TVal>
		constexpr Vector(TVal values) noexcept :array{ static_cast<T>(values), static_cast<T>(values), static_cast<T>(values), static_cast<T>(values) } {}
		template<typename TX, typename TY, typename TZ, typename TW>
		constexpr Vector(TX x, TY y, TZ z, TW w) noexcept :array{ static_cast<T>(x), static_cast<T>(y), static_cast<T>(z), static_cast<T>(w) } {}
		
		template<typename TVec, PackingMode PVec>
		constexpr Vector(const Vector<TVec, 2, PVec>& xy, TVec z, TVec w) noexcept :array{ static_cast<T>(xy.x), static_cast<T>(xy.y), static_cast<T>(z), static_cast<T>(w) } {}
		template<typename TVec, PackingMode PVec>
		constexpr Vector(TVec x, const Vector<TVec, 2, PVec>& yz, TVec w) noexcept :array{ static_cast<T>(x), static_cast<T>(yz.x), static_cast<T>(yz.y), static_cast<T>(w) } {}
		template<typename TVec, PackingMode PVec>
		constexpr Vector(TVec x, TVec y, const Vector<TVec, 2, PVec>& zw) noexcept :array{ static_cast<T>(x), static_cast<T>(y), static_cast<T>(zw.x), static_cast<T>(zw.y) } {}
		template<typename TVec, PackingMode PVec>
		constexpr Vector(const Vector<TVec, 2, PVec>& xy, const Vector<TVec, 2, PVec>& zw) noexcept :array{ static_cast<T>(xy.x), static_cast<T>(xy.y), static_cast<T>(zw.x), static_cast<T>(zw.y) } {}

		template<typename TVec, PackingMode PVec>
		constexpr Vector(const Vector<TVec, 3, PVec>& xyz, TVec w) noexcept :array{ static_cast<T>(xyz.x), static_cast<T>(xyz.y), static_cast<T>(xyz.z), static_cast<T>(w) } {}
		template<typename TVec, PackingMode PVec>
		constexpr Vector(TVec x, const Vector<TVec, 3, PVec>& yzw) noexcept :array{ static_cast<T>(x), static_cast<T>(yzw.x), static_cast<T>(yzw.y), static_cast<T>(yzw.z) } {}

		template<typename TVec, PackingMode PVec>
		constexpr Vector(const Vector<TVec, 4, PVec>& rhs) noexcept :array{ static_cast<T>(rhs.x), static_cast<T>(rhs.y), static_cast<T>(rhs.z), static_cast<T>(rhs.w) } {}
		template<typename TArr>
		constexpr Vector(TArr (&values)[4]) noexcept :array{ static_cast<T>(values[0]), static_cast<T>(values[1]), static_cast<T>(values[2]), static_cast<T>(values[3])} {}

		constexpr T& operator[](size_t index) { return array[index]; }
		constexpr const T& operator[](size_t index) const { return array[index]; }

		Vector& operator=(const Vector& rhs) = default;

		bool operator==(const Vector& rhs) const = default;

		constexpr T Length() const;
		constexpr T Length2() const;
		constexpr Vector Normalize() const;
		constexpr T Dot(const Vector<T, 4, P>& rhs) const;

		constexpr Vector<T, 2, P> Swizzle(size_t index0, size_t index1) const { return Vector<T, 2, P>{ array[index0], array[index1] }; }
		constexpr Vector<T, 3, P> Swizzle(size_t index0, size_t index1, size_t index2) const { return Vector<T, 3, P>{ array[index0], array[index1], array[index2] }; }
		constexpr Vector<T, 4, P> Swizzle(size_t index0, size_t index1, size_t index2, size_t index3) const { return Vector<T, 4, P>{ array[index0], array[index1], array[index2], array[index3] }; }
	};

	template<typename T, PackingMode P>
	constexpr Vector<T, 4, P> operator+(const Vector<T, 4, P>& vector) noexcept;
	template<typename T, PackingMode P>
	constexpr Vector<T, 4, P> operator-(const Vector<T, 4, P>& vector) noexcept;

	template<typename T, PackingMode P>
	constexpr Vector<T, 4, P> operator+(const Vector<T, 4, P>& lhs, T rhs) noexcept;
	template<typename T, PackingMode P>
	constexpr Vector<T, 4, P> operator-(const Vector<T, 4, P>& lhs, T rhs) noexcept;
	template<typename T, PackingMode P>
	constexpr Vector<T, 4, P> operator*(const Vector<T, 4, P>& lhs, T rhs) noexcept;
	template<typename T, PackingMode P>
	constexpr Vector<T, 4, P> operator/(const Vector<T, 4, P>& lhs, T rhs) noexcept;
	template<typename T, PackingMode P>	
	constexpr const Vector<T, 4, P>& operator+=(Vector<T, 4, P>& lhs, T rhs) noexcept;
	template<typename T, PackingMode P>	
	constexpr const Vector<T, 4, P>& operator-=(Vector<T, 4, P>& lhs, T rhs) noexcept;
	template<typename T, PackingMode P>	
	constexpr const Vector<T, 4, P>& operator*=(Vector<T, 4, P>& lhs, T rhs) noexcept;
	template<typename T, PackingMode P>	
	constexpr const Vector<T, 4, P>& operator/=(Vector<T, 4, P>& lhs, T rhs) noexcept;

	template<typename T, PackingMode P>	
	constexpr Vector<T, 4, P> operator+(const Vector<T, 4, P>& lhs, const Vector<T, 4, P>& rhs) noexcept;
	template<typename T, PackingMode P>	
	constexpr Vector<T, 4, P> operator-(const Vector<T, 4, P>& lhs, const Vector<T, 4, P>& rhs) noexcept;
	template<typename T, PackingMode P>	
	constexpr Vector<T, 4, P> operator*(const Vector<T, 4, P>& lhs, const Vector<T, 4, P>& rhs) noexcept;
	template<typename T, PackingMode P>	
	constexpr Vector<T, 4, P> operator/(const Vector<T, 4, P>& lhs, const Vector<T, 4, P>& rhs) noexcept;
	template<typename T, PackingMode P>	
	constexpr const Vector<T, 4, P>& operator+=(Vector<T, 4, P>& lhs, const Vector<T, 4, P>& rhs) noexcept;
	template<typename T, PackingMode P>	
	constexpr const Vector<T, 4, P>& operator-=(Vector<T, 4, P>& lhs, const Vector<T, 4, P>& rhs) noexcept;
	template<typename T, PackingMode P>	
	constexpr const Vector<T, 4, P>& operator*=(Vector<T, 4, P>& lhs, const Vector<T, 4, P>& rhs) noexcept;
	template<typename T, PackingMode P>	
	constexpr const Vector<T, 4, P>& operator/=(Vector<T, 4, P>& lhs, const Vector<T, 4, P>& rhs) noexcept;

	template<typename T, PackingMode P>
	constexpr Vector<T, 4, P> operator+(T lhs, const Vector<T, 4, P>& rhs) noexcept;
	template<typename T, PackingMode P>
	constexpr Vector<T, 4, P> operator-(T lhs, const Vector<T, 4, P>& rhs) noexcept;
	template<typename T, PackingMode P>
	constexpr Vector<T, 4, P> operator*(T lhs, const Vector<T, 4, P>& rhs) noexcept;
	template<typename T, PackingMode P>
	constexpr Vector<T, 4, P> operator/(T lhs, const Vector<T, 4, P>& rhs) noexcept;

	template<typename T, PackingMode P>
	constexpr T Length(const Vector<T, 4, P>& vector);

	template<typename T, PackingMode P>
	constexpr T Length2(const Vector<T, 4, P>& vector);

	template<typename T, PackingMode P>
	constexpr Vector<T, 4, P> Normalize(const Vector<T, 4, P>& vector);

	template<typename T, PackingMode P>
	constexpr T Dot(const Vector<T, 4, P>& lhs, const Vector<T, 4, P>& rhs);

#if PWM_DEFINE_OSTREAM
	template<typename T, PackingMode P>
	inline std::ostream& operator<<(std::ostream& stream, Vector<T, 4, P> vector)
	{
		stream << '[' << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << ']';
		return stream;
	}
#endif // PWM_DEFINE_OSTREAM

	template<typename T, PackingMode P = PackingMode::Default>
	using Vector4 = Vector<T, 4, P>;

	using Vector4F32	= Vector4<float>;
	using Vector4F64	= Vector4<double>;
	using Vector4I8		= Vector4<int8_t>;
	using Vector4I16	= Vector4<int16_t>;
	using Vector4I32	= Vector4<int32_t>;
	using Vector4I64	= Vector4<int64_t>;
	using Vector4U8		= Vector4<uint8_t>;
	using Vector4U16	= Vector4<uint16_t>;
	using Vector4U32	= Vector4<uint32_t>;
	using Vector4U64	= Vector4<uint64_t>;

	template<typename T>
	using Vector4Fast = Vector<T, 4, PackingMode::Fast>;

	using Vector4F32Fast	= Vector4Fast<float>;
	using Vector4F64Fast	= Vector4Fast<double>;
	using Vector4I8Fast		= Vector4Fast<int8_t>;
	using Vector4I16Fast	= Vector4Fast<int16_t>;
	using Vector4I32Fast	= Vector4Fast<int32_t>;
	using Vector4I64Fast	= Vector4Fast<int64_t>;
	using Vector4U8Fast		= Vector4Fast<uint8_t>;
	using Vector4U16Fast	= Vector4Fast<uint16_t>;
	using Vector4U32Fast	= Vector4Fast<uint32_t>;
	using Vector4U64Fast	= Vector4Fast<uint64_t>;
}

#include <PWMath/Impl/Vector4.inl>
