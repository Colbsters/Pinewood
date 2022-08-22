#pragma once

////////////////////////////////////
// -=- Feature Support Macros -=- //
////////////////////////////////////

#if PW_ARCH_X64
// All x64 processors support SSE2
#define PWM_USE_SSE2 1
#endif // PW_ARCH_X64

// AVX2 implies AVX support
#if PWM_USE_AVX2 & !defined(PWM_USE_AVX)
#define PWM_USE_AVX 1
#endif // PWM_USE_AVX2

// AVX implies SSE4 (4.1 and 4.2) support
#if PWM_USE_AVX & !defined(PWM_USE_SSE4)
#define PWM_USE_SSE4 1
#endif // PWM_USE_AVX2

// SSE4 implies SSSE3 support
#if PWM_USE_SSE4 & !defined(PWM_USE_SSSE3)
#define PWM_USE_SSSE3 1
#endif // PWM_USE_AVX2

// SSSE3 implies SSE3 support
#if PWM_USE_SSSE3 & !defined(PWM_USE_SSE3)
#define PWM_USE_SSE3 1
#endif // PWM_USE_AVX2

// SSE3 implies SSE2 support
#if PWM_USE_SSE3 & !defined(PWM_USE_SSE2)
#define PWM_USE_SSE2 1
#endif // PWM_USE_AVX2

// SSE2 implies SSE support
#if PWM_USE_SSE2 & !defined(PWM_USE_SSE)
#define PWM_USE_SSE 1
#endif // PWM_USE_AVX2

