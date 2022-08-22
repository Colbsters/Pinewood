#pragma once
#include <PWMath/Vector2.h>

namespace PWMath
{
#pragma region Unary operators

	template<typename T, PackingMode P>
	constexpr Vector<T, 2, P> operator+(const Vector<T, 2, P>& vector) noexcept
	{
		return vector;
	}

	template<typename T, PackingMode P>
	constexpr Vector<T, 2, P> operator-(const Vector<T, 2, P>& vector) noexcept
	{
		return Vector<T, 2, P>{ -vector.x, -vector.y };
	}

#pragma endregion

#pragma region Vector and scalar

	template<typename T, PackingMode P>
	constexpr Vector<T, 2, P> operator+(const Vector<T, 2, P>& lhs, T rhs) noexcept
	{
		return lhs + Vector<T, 2, P>{ rhs };
	}

	template<typename T, PackingMode P>
	constexpr Vector<T, 2, P> operator-(const Vector<T, 2, P>& lhs, T rhs) noexcept
	{
		return lhs - Vector<T, 2, P>{ rhs };
	}

	template<typename T, PackingMode P>
	constexpr Vector<T, 2, P> operator*(const Vector<T, 2, P>& lhs, T rhs) noexcept
	{
		return lhs * Vector<T, 2, P>{ rhs };
	}

	template<typename T, PackingMode P>
	constexpr Vector<T, 2, P> operator/(const Vector<T, 2, P>& lhs, T rhs) noexcept
	{
		return lhs / Vector<T, 2, P>{ rhs };
	}

	template<typename T, PackingMode P>
	constexpr const Vector<T, 2, P>& operator+=(Vector<T, 2, P>& lhs, T rhs) noexcept
	{
		return lhs += Vector<T, 2, P>{ rhs };
	}

	template<typename T, PackingMode P>
	constexpr const Vector<T, 2, P>& operator-=(Vector<T, 2, P>& lhs, T rhs) noexcept
	{
		return lhs -= Vector<T, 2, P>{ rhs };
	}

	template<typename T, PackingMode P>
	constexpr const Vector<T, 2, P>& operator*=(Vector<T, 2, P>& lhs, T rhs) noexcept
	{
		return lhs *= Vector<T, 2, P>{ rhs };
	}

	template<typename T, PackingMode P>
	constexpr const Vector<T, 2, P>& operator/=(Vector<T, 2, P>& lhs, T rhs) noexcept
	{
		return lhs /= Vector<T, 2, P>{ rhs };
	}

#pragma endregion

#pragma region Vector and vector

	template<typename T, PackingMode P>
	constexpr Vector<T, 2, P> operator+(const Vector<T, 2, P>& lhs, const Vector<T, 2, P>& rhs) noexcept
	{
		return Vector<T, 2, P>{ lhs.x + rhs.x, lhs.y + rhs.y };
	}

	template<typename T, PackingMode P>
	constexpr Vector<T, 2, P> operator-(const Vector<T, 2, P>& lhs, const Vector<T, 2, P>& rhs) noexcept
	{
		return Vector<T, 2, P>{ lhs.x - rhs.x, lhs.y - rhs.y };
	}

	template<typename T, PackingMode P>
	constexpr Vector<T, 2, P> operator*(const Vector<T, 2, P>& lhs, const Vector<T, 2, P>& rhs) noexcept
	{
		return Vector<T, 2, P>{ lhs.x * rhs.x, lhs.y * rhs.y };
	}

	template<typename T, PackingMode P>
	constexpr Vector<T, 2, P> operator/(const Vector<T, 2, P>& lhs, const Vector<T, 2, P>& rhs) noexcept
	{
		return Vector<T, 2, P>{ lhs.x / rhs.x, lhs.y / rhs.y };
	}

	template<typename T, PackingMode P>
	constexpr const Vector<T, 2, P>& operator+=(Vector<T, 2, P>& lhs, const Vector<T, 2, P>& rhs) noexcept
	{
		return lhs = (lhs + rhs);
	}

	template<typename T, PackingMode P>
	constexpr const Vector<T, 2, P>& operator-=(Vector<T, 2, P>& lhs, const Vector<T, 2, P>& rhs) noexcept
	{
		return lhs = (lhs - rhs);
	}

	template<typename T, PackingMode P>
	constexpr const Vector<T, 2, P>& operator*=(Vector<T, 2, P>& lhs, const Vector<T, 2, P>& rhs) noexcept
	{
		return lhs = (lhs * rhs);
	}

	template<typename T, PackingMode P>
	constexpr const Vector<T, 2, P>& operator/=(Vector<T, 2, P>& lhs, const Vector<T, 2, P>& rhs) noexcept
	{
		return lhs = (lhs / rhs);
	}

#pragma endregion
	
#pragma region Scalar and vector

	template<typename T, PackingMode P>
	constexpr Vector<T, 2, P> operator+(T lhs, const Vector<T, 2, P>& rhs) noexcept
	{
		return Vector<T, 2, P>{ lhs } + rhs;
	}

	template<typename T, PackingMode P>
	constexpr Vector<T, 2, P> operator-(T lhs, const Vector<T, 2, P>& rhs) noexcept
	{
		return Vector<T, 2, P>{ lhs } - rhs;
	}

	template<typename T, PackingMode P>
	constexpr Vector<T, 2, P> operator*(T lhs, const Vector<T, 2, P>& rhs) noexcept
	{
		return Vector<T, 2, P>{ lhs } * rhs;
	}

	template<typename T, PackingMode P>
	constexpr Vector<T, 2, P> operator/(T lhs, const Vector<T, 2, P>& rhs) noexcept
	{
		return Vector<T, 2, P>{ lhs } / rhs;
	}

#pragma endregion

#pragma region Other functions

	template<typename T, PackingMode P>
	constexpr T Length(const Vector<T, 2, P>& vector)
	{
		return std::sqrt(Length2(vector));
	}

	template<typename T, PackingMode P>
	constexpr T Length2(const Vector<T, 2, P>& vector)
	{
		return (vector.x * vector.x) + (vector.y * vector.y);
	}

	template<typename T, PackingMode P>
	constexpr Vector<T, 2, P> Normalize(const Vector<T, 2, P>& vector)
	{
		return vector / Length(vector);
	}

	template<typename T, PackingMode P>
	constexpr T Dot(const Vector<T, 2, P>& lhs, const Vector<T, 2, P>& rhs)
	{
		return (lhs.x * rhs.x) + (lhs.y * rhs.y);
	}

#pragma endregion

#pragma region Member versions of functions

	template<typename T, PackingMode P>
	constexpr T Vector<T, 2, P>::Length() const { return PWMath::Length(*this); }

	template<typename T, PackingMode P>
	constexpr T Vector<T, 2, P>::Length2() const { return PWMath::Length2(*this); }

	template<typename T, PackingMode P>
	constexpr Vector<T, 2, P> Vector<T, 2, P>::Normalize() const { return PWMath::Normalize(*this); }

	template<typename T, PackingMode P>
	constexpr T Vector<T, 2, P>::Dot(const Vector<T, 2, P>& rhs) const { return PWMath::Dot(*this, rhs); }

#pragma endregion
}
