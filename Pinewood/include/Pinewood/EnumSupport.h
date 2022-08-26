#pragma once
#include <Pinewood/Core.h>
#include <type_traits>

#define PW_DEFINE_ENUMCLASS_OPERATOR_OR(enum_class)		\
	constexpr enum_class operator|(enum_class lhs, enum_class rhs) { return static_cast<enum_class>(static_cast<std::underlying_type_t<enum_class>>(lhs) | static_cast<std::underlying_type_t<enum_class>>(rhs)); }

#define PW_DEFINE_ENUMCLASS_OPERATOR_AND(enum_class)		\
	constexpr enum_class operator&(enum_class lhs, enum_class rhs) { return static_cast<enum_class>(static_cast<std::underlying_type_t<enum_class>>(lhs) & static_cast<std::underlying_type_t<enum_class>>(rhs)); }

#define PW_DEFINE_ENUMCLASS_OPERATOR_NOT(enum_class)	\
	constexpr enum_class operator~(enum_class value) { return static_cast<enum_class>(~static_cast<std::underlying_type_t<enum_class>>(value)); }
