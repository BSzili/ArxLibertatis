
#ifndef ARX_PLATFORM_PLATFORMCONFIG_H
#define ARX_PLATFORM_PLATFORMCONFIG_H

/*!
 * Low-level configuration.
 * Please only add config flags that change rarely (such as compiler features)
 * in order to not break incremental builds. Particularly, availability of
 * libraries and their features belongs in src/Configure.h.in
 */

// C++11 features

// alignas(n)
#define ARX_HAVE_CXX11_ALIGNAS 0
// alignof(T)
#define ARX_HAVE_CXX11_ALIGNOF 0
// auto
#define ARX_HAVE_CXX11_AUTO 0
// std::atomic
#define ARX_HAVE_CXX11_ATOMIC 0
// std::forward in <utility>
#define ARX_HAVE_CXX11_FORWARD 0
// std::itegral_constant in <type_traits>
#define ARX_HAVE_CXX11_INTEGRAL_CONSTANT 0
// long long
#define ARX_HAVE_CXX11_LONG_LONG 1
// std::max_align_t <cstddef>
#define ARX_HAVE_CXX11_MAX_ALIGN_T 0
// noexcept
#define ARX_HAVE_CXX11_NOEXCEPT 0
// static_assert(cond, msg)
#define ARX_HAVE_CXX11_STATIC_ASSERT 0
// variadic templates
#define ARX_HAVE_CXX11_VARIADIC_TEMPLATES 0
// thread_local
#define ARX_HAVE_CXX11_THREADLOCAL 0

// GCC extensions
// __attribute__((aligned(n)))
#define ARX_HAVE_ATTRIBUTE_ALIGNED 1
// __attribute__((format(printf, i, j)))
#define ARX_HAVE_ATTRIBUTE_FORMAT_PRINTF 1
// __builtin_trap()
#define ARX_HAVE_BUILTIN_TRAP 0
// __builtin_unreachable()
#define ARX_HAVE_BUILTIN_UNREACHABLE 0
// __alignof__(T)
#define ARX_HAVE_GCC_ALIGNOF 1
// __thread
#define ARX_HAVE_GCC_THREADLOCAL 1

// MSVC extensions
// __declspec(align(n))
#define ARX_HAVE_DECLSPEC_ALIGN 0
// __alignof(T)
#define ARX_HAVE_MSVC_ALIGNOF 0
// __declspec(thread)
#define ARX_HAVE_DECLSPEC_THREADLOCAL 0

// Code generation options
#define BUILD_PROFILER_INSTRUMENT 0

#endif // ARX_PLATFORM_PLATFORMCONFIG_H
