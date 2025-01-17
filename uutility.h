// This file is part of the uSTL library, an STL implementation.
//
// Copyright (c) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.
//
/// \file uutility.h
/// \brief Utility templates.

#pragma once
#include "utypes.h"
#include "ulimits.h"

namespace ustl {

using nullptr_t	= decltype(nullptr);
using max_align_t = uintptr_t;

/// Returns the number of bits in the given type
#define BitsInType(t)	(sizeof(t) * CHAR_BIT)

/// Returns the mask of type \p t with the lowest \p n bits set.
#define BitMask(t,n)	(t(~t(0)) >> (BitsInType(t) - (n)))

/// Argument that is used only in debug builds (as in an assert)
#ifndef NDEBUG
    #define DebugArg(x)	x
#else
    #define DebugArg(x)
#endif

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Macro for passing template types as macro arguments.
// These are deprecated. Use metamac macros instead. Will remove by next release.
#define TEMPLATE_FULL_DECL1(d1,t1)		template <d1 t1>
#define TEMPLATE_FULL_DECL2(d1,t1,d2,t2)	template <d1 t1, d2 t2>
#define TEMPLATE_FULL_DECL3(d1,t1,d2,t2,d3,t3)	template <d1 t1, d2 t2, d3 t3>
#define TEMPLATE_DECL1(t1)		TEMPLATE_FULL_DECL1(typename,t1)
#define TEMPLATE_DECL2(t1,t2)		TEMPLATE_FULL_DECL2(typename,t1,typename,t2)
#define TEMPLATE_DECL3(t1,t2,t3)	TEMPLATE_FULL_DECL3(typename,t1,typename,t2,typename,t3)
#define TEMPLATE_TYPE1(type,a1)		type<a1>
#define TEMPLATE_TYPE2(type,a1,a2)	type<a1,a2>
#define TEMPLATE_TYPE3(type,a1,a2,a3)	type<a1,a2,a3>
#endif

/// Returns the minimum of \p a and \p b
template <typename T1, typename T2>
inline constexpr T1 min (const T1& a, const T2& b)
    { return a < b ? a : b; }

/// Returns the maximum of \p a and \p b
template <typename T1, typename T2>
inline constexpr T1 max (const T1& a, const T2& b)
    { return b < a ? a : b; }

/// Indexes into a static array with bounds limit
template <typename T, size_t N>
inline constexpr T& VectorElement (T(&v)[N], size_t i) { return v[min(i,N-1)]; }

/// The alignment performed by default.
const size_t c_DefaultAlignment = __alignof__(void*);

/// \brief Rounds \p n up to be divisible by \p grain
template <typename T>
inline constexpr T AlignDown (T n, size_t grain = c_DefaultAlignment)
    { return n - n % grain; }

/// \brief Rounds \p n up to be divisible by \p grain
template <typename T>
inline constexpr T Align (T n, size_t grain = c_DefaultAlignment)
    { return AlignDown (n + grain - 1, grain); }

/// Returns a nullptr pointer cast to T.
template <typename T>
inline constexpr T* NullPointer (void)
    { return nullptr; }

/// \brief Returns a non-dereferentiable value reference.
/// This is useful for passing to stream_align_of or the like which need a value but
/// don't need to actually use it.
template <typename T>
inline constexpr T& NullValue (void)
    { return *NullPointer<T>(); }

template <typename T>
T* addressof (T& v)
    { return reinterpret_cast<T*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(v))); }

// constexpr cast between pointers without reinterpret_cast
template <typename T, typename F> constexpr T* pointer_cast (F* p)
    { return static_cast<T*>(static_cast<void*>(p)); }
template <typename T, typename F> constexpr const T* pointer_cast (const F* p)
    { return static_cast<const T*>(static_cast<const void*>(p)); }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// The compiler issues a warning if an unsigned type is compared to 0.
template <typename T, bool IsSigned> struct __is_negative { inline constexpr bool operator()(const T& v) const { return v < 0; } };
template <typename T> struct __is_negative<T,false> { inline constexpr bool operator()(const T&) const { return false; } };
/// Warning-free way to check if \p v is negative, even if for unsigned types.
template <typename T> inline constexpr bool is_negative (const T& v) { return __is_negative<T,numeric_limits<T>::is_signed>()(v); }
#endif

/// \brief Returns the absolute value of \p v
/// Unlike the stdlib functions, this is inline and works with all types.
template <typename T>
inline constexpr T absv (T v)
{
    return is_negative(v) ? -v : v;
}

/// \brief Returns -1 for negative values, 1 for positive, and 0 for 0
template <typename T>
inline constexpr T sign (T v)
{
    return (0 < v) - is_negative(v);
}

/// Returns the greatest common divisor
template <typename T>
constexpr T gcd (T a, T b)
{
    return b ? gcd(b,a%b) : absv(a);
}

/// Returns the least common multiple
template <typename T>
constexpr T lcm (T a, T b)
{
    return a/gcd(a,b)*b;
}

// Defined in byteswap.h, which is usually unusable.
#undef bswap_16
#undef bswap_32
#undef bswap_64

inline constexpr uint8_t bswap_8 (uint8_t v)	{ return v; }
inline constexpr uint16_t bswap_16 (uint16_t v)	{ return __builtin_bswap16 (v); }
inline constexpr uint32_t bswap_32 (uint32_t v)	{ return __builtin_bswap32 (v); }
inline constexpr uint64_t bswap_64 (uint64_t v)	{ return __builtin_bswap64 (v); }

inline constexpr uint8_t bswap (uint8_t v)	{ return v; }
inline constexpr uint16_t bswap (uint16_t v)	{ return __builtin_bswap16 (v); }
inline constexpr uint32_t bswap (uint32_t v)	{ return __builtin_bswap32 (v); }
inline constexpr uint64_t bswap (uint64_t v)	{ return __builtin_bswap64 (v); }
inline constexpr int8_t bswap (int8_t v)	{ return v; }
inline constexpr int16_t bswap (int16_t v)	{ return __builtin_bswap16 (v); }
inline constexpr int32_t bswap (int32_t v)	{ return __builtin_bswap32 (v); }
inline constexpr int64_t bswap (int64_t v)	{ return __builtin_bswap64 (v); }

enum class endian {
    little	= __ORDER_LITTLE_ENDIAN__,
    big		= __ORDER_BIG_ENDIAN__,
    native	= __BYTE_ORDER__
};

#if BYTE_ORDER == BIG_ENDIAN
template <typename T> inline constexpr T le_to_native (const T& v) { return bswap (v); }
template <typename T> inline constexpr T be_to_native (const T& v) { return v; }
template <typename T> inline constexpr T native_to_le (const T& v) { return bswap (v); }
template <typename T> inline constexpr T native_to_be (const T& v) { return v; }
#elif BYTE_ORDER == LITTLE_ENDIAN
template <typename T> inline constexpr T le_to_native (const T& v) { return v; }
template <typename T> inline constexpr T be_to_native (const T& v) { return bswap (v); }
template <typename T> inline constexpr T native_to_le (const T& v) { return v; }
template <typename T> inline constexpr T native_to_be (const T& v) { return bswap (v); }
#endif // BYTE_ORDER

/// Deletes \p p and sets it to nullptr
template <typename T>
inline void Delete (T*& p)
{
    delete p;
    p = nullptr;
}

/// Deletes \p p as an array and sets it to nullptr
template <typename T>
inline void DeleteVector (T*& p)
{
    delete [] p;
    p = nullptr;
}

/// Template of making != from ! and ==
template <typename T>
inline constexpr bool operator!= (const T& x, const T& y)
    { return !(x == y); }

/// Template of making > from <
template <typename T>
inline constexpr bool operator> (const T& x, const T& y)
    { return y < x; }

/// Template of making <= from < and ==
template <typename T>
inline constexpr bool operator<= (const T& x, const T& y)
    { return !(y < x); }

/// Template of making >= from < and ==
template <typename T>
inline constexpr bool operator>= (const T& x, const T& y)
    { return !(x < y); }

/// Packs \p s multiple times into \p b. Useful for loop unrolling.
template <typename TSmall, typename TBig>
inline constexpr void pack_type (TSmall s, TBig& b)
{
    b = s;
    for (unsigned h = BitsInType(TSmall); h < BitsInType(TBig); h *= 2)
	b = (b << h)|b;
}

/// \brief Divides \p n1 by \p n2 and rounds the result up.
/// This is in contrast to regular division, which rounds down.
template <typename T1, typename T2>
inline constexpr T1 DivRU (T1 n1, T2 n2)
{
    T2 adj = n2 - 1;
    if (is_negative (n1))
	adj = -adj;
    return (n1 + adj) / n2;
}

/// Sets the contents of \p pm to 1 and returns true if the previous value was 0.
inline bool TestAndSet (int* pm)
{
#if __x86__
    int oldVal (1);
    asm volatile ("xchgl %0, %1" : "=r"(oldVal), "=m"(*pm) : "0"(oldVal), "m"(*pm) : "memory");
    return !oldVal;
#elif __sparc32__	// This has not been tested
    int rv;
    asm volatile ("ldstub %1, %0" : "=r"(rv), "=m"(*pm) : "m"(pm));
    return !rv;
#else
    const int oldVal (*pm);
    *pm = 1;
    return !oldVal;
#endif
}

/// Returns the index of the first set bit in \p v or \p nbv if none.
inline uoff_t FirstBit (uint32_t v, uoff_t nbv)
{
    uoff_t n = nbv;
#if __x86__
    if (!__builtin_constant_p(v)) asm ("bsr\t%1, %k0":"+r,r"(n):"r,m"(v)); else
#endif
    if (v) n = 31 - __builtin_clz(v);
    return n;
}
/// Returns the index of the first set bit in \p v or \p nbv if none.
inline uoff_t FirstBit (uint64_t v, uoff_t nbv)
{
    uoff_t n = nbv;
#if __x86_64__
    if (!__builtin_constant_p(v)) asm ("bsr\t%1, %0":"+r,r"(n):"r,m"(v)); else
#endif
#if SIZE_OF_LONG >= 8
    if (v) n = 63 - __builtin_clzl(v);
#elif SIZE_OF_LONG_LONG >= 8
    if (v) n = 63 - __builtin_clzll(v);
#else
    if (v) for (uint64_t m = uint64_t(1)<<(n=63); !(v & m); m >>= 1) --n;
#endif
    return n;
}

/// Returns the next power of 2 >= \p v.
/// Values larger than UINT32_MAX/2 will return 2^0
inline uint32_t NextPow2 (uint32_t v)
{
    uint32_t r = v-1;
#if __x86__
    if (!__builtin_constant_p(r)) asm("bsr\t%0, %0":"+r"(r)); else
#endif
    { r = FirstBit(r,r); if (r >= BitsInType(r)-1) r = uint32_t(-1); }
    return 1<<(1+r);
}

/// Bitwise rotate value left
template <typename T>
inline constexpr T Rol (T v, size_t n)
{
    return (v << n) | (v >> (BitsInType(T)-n));
}

/// Bitwise rotate value right
template <typename T>
inline constexpr T Ror (T v, size_t n)
{
    return (v >> n) | (v << (BitsInType(T)-n));
}

/// \brief This template is to be used for dereferencing a type-punned pointer without a warning.
///
/// When casting a local variable to an unrelated type through a pointer (for
/// example, casting a float to a uint32_t without conversion), the resulting
/// memory location can be accessed through either pointer, which violates the
/// strict aliasing rule. While -fno-strict-aliasing option can be given to
/// the compiler, eliminating this warning, inefficient code may result in
/// some instances, because aliasing inhibits some optimizations. By using
/// this template, and by ensuring the memory is accessed in one way only,
/// efficient code can be produced without the warning. For gcc 4.1.0+.
///
template <typename DEST, typename SRC>
inline DEST noalias (const DEST&, SRC* s)
{
    asm("":"+g"(s)::"memory");
    union UPun { SRC s; DEST d; };
    return reinterpret_cast<UPun*>(s)->d;
}

template <typename DEST, typename SRC>
inline DEST noalias_cast (SRC s)
{
    asm("":"+g"(s)::"memory");
    union { SRC s; DEST d; } u = {s};
    return u.d;
}

namespace simd {
    /// Call after you are done using SIMD algorithms for 64 bit tuples.
    #define ALL_MMX_REGS_CHANGELIST "mm0","mm1","mm2","mm3","mm4","mm5","mm6","mm7","st","st(1)","st(2)","st(3)","st(4)","st(5)","st(6)","st(7)"
#if __3DNOW__
    inline void reset_mmx (void) { asm ("femms":::ALL_MMX_REGS_CHANGELIST); }
#elif __MMX__
    inline void reset_mmx (void) { asm ("emms":::ALL_MMX_REGS_CHANGELIST); }
#else
    inline void reset_mmx (void) {}
#endif
} // namespace simd
} // namespace ustl
