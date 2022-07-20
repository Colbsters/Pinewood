#pragma once
#include <Pinewood/Core.h>
#include <type_traits>

#define PW_DEFINE_ENUMCLASS_OPERATOR_OR(enum_class)		\
	constexpr std::underlying_type_t<enum_class> operator|(uint32_t lhs, WindowCreateFlags rhs) { return lhs | static_cast<std::underlying_type_t<enum_class>>(rhs); }			\
	constexpr std::underlying_type_t<enum_class> operator|(WindowCreateFlags lhs, uint32_t rhs) { return rhs | lhs; }															\
	constexpr std::underlying_type_t<enum_class> operator|(WindowCreateFlags lhs, WindowCreateFlags rhs) { return static_cast<std::underlying_type_t<enum_class>>(lhs) | rhs; }	\
	constexpr std::underlying_type_t<enum_class> operator|=(uint32_t& lhs, WindowCreateFlags rhs) { return lhs |= static_cast<std::underlying_type_t<enum_class>>(rhs); }

#define PW_DEFINE_ENUMCLASS_OPERATOR_AND(enum_class)	\
	constexpr std::underlying_type_t<enum_class> operator&(uint32_t lhs, WindowCreateFlags rhs) { return lhs & static_cast<std::underlying_type_t<enum_class>>(rhs); }			\
	constexpr std::underlying_type_t<enum_class> operator&(WindowCreateFlags lhs, uint32_t rhs) { return rhs & lhs; }															\
	constexpr std::underlying_type_t<enum_class> operator&=(uint32_t& lhs, WindowCreateFlags rhs) { return lhs &= static_cast<std::underlying_type_t<enum_class>>(rhs); }
