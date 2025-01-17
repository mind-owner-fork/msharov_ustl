// This file is part of the uSTL library, an STL implementation.
//
// Copyright (c) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#pragma once
#include "utuple.h"

namespace ustl {

/// \class matrix umatrix.h ustl.h
/// \ingroup Sequences
///
/// \brief A two-dimensional array of NX*NY elements of type T.
///
template <size_t NX, size_t NY, typename T>
class matrix : public tuple<NX*NY,T> {
public:
    using row_type		= tuple<NX,T>;
    using column_type		= tuple<NY,T>;
    using tuple_type		= tuple<NX*NY,T>;
    using value_type		= typename tuple_type::value_type;
    using size_type		= typename tuple_type::size_type;
    using pointer		= typename tuple_type::pointer;
    using const_pointer		= typename tuple_type::const_pointer;
    using reference		= typename tuple_type::reference;
    using const_reference	= typename tuple_type::const_reference;
    using iterator		= typename tuple_type::iterator;
    using const_iterator	= typename tuple_type::const_iterator;
    using range_t		= typename tuple_type::range_t;
    using const_range_t		= typename tuple_type::const_range_t;
    using reverse_iterator	= typename tuple_type::reverse_iterator;
    using const_reverse_iterator = typename tuple_type::const_reverse_iterator;
public:
    inline			matrix (void)			: tuple<NX*NY,T>() { }
    template <typename T2>
    inline			matrix (const matrix<NX,NY,T2>& src) : tuple<NX*NY,T2>(src) {}
    inline			matrix (const matrix<NX,NY,T>& src) : tuple<NX*NY,T>(src) {}
    inline size_type		columns (void) const		{ return NX; }
    inline size_type		rows (void) const		{ return NY; }
    inline const_iterator	at (size_type i) const		{ return matrix::begin() + i * NX; }
    inline iterator		at (size_type i)		{ return matrix::begin() + i * NX; }
    inline const_iterator	operator[] (size_type i) const	{ return at (i); }
    inline iterator		operator[] (size_type i)	{ return at (i); }
    inline row_type		row (size_type r) const		{ return row_type (at (r)); }
    inline column_type		column (size_type c) const;
    template <typename T2>
    inline const matrix&	operator= (const matrix<NX,NY,T2>& src)	{ tuple_type::operator= (src); return *this; }
    inline const matrix&	operator= (const matrix<NX,NY,T>& src)	{ tuple_type::operator= (src); return *this; }
    inline const matrix&	operator+= (const_reference v)		{ tuple_type::operator+= (v); return *this; }
    inline const matrix&	operator-= (const_reference v)		{ tuple_type::operator-= (v); return *this; }
    inline const matrix&	operator*= (const_reference v)		{ tuple_type::operator*= (v); return *this; }
    inline const matrix&	operator/= (const_reference v)		{ tuple_type::operator/= (v); return *this; }
    inline const matrix		operator+ (const_reference v) const
				    { matrix result (*this); result += v; return result; }
    inline const matrix		operator- (const_reference v) const
				    { matrix result (*this); result -= v; return result; }
    inline const matrix		operator* (const_reference v) const
				    { matrix result (*this); result *= v; return result; }
    inline const matrix		operator/ (const_reference v) const
				    { matrix result (*this); result /= v; return result; }
    void			text_write (ostringstream& os) const;
};

template <size_t NX, size_t NY, typename T>
inline typename matrix<NX,NY,T>::column_type matrix<NX,NY,T>::column (size_type c) const
{
    column_type result;
    const_iterator src (matrix::begin() + c);
    iterator dest (result.begin());
    for (uoff_t i = 0; i < NY; ++ i, ++ dest, src += NX)
	*dest = *src;
    return result;
}

//----------------------------------------------------------------------
// Define SIMD specializations for member functions.

#if __SSE__
#define SSE_TUPLE_SPECS(n,type)			\
template <> inline tuple<n,type>::tuple (void)	\
{   asm volatile ("xorps %%xmm0, %%xmm0\n\t"	\
	"movups %%xmm0, %0\n\t"			\
	"movups %%xmm0, %1\n\t"			\
	"movups %%xmm0, %2\n\t"			\
	"movups %%xmm0, %3"			\
	: "=m"(_v[0]),"=m"(_v[4]),"=m"(_v[8]),"=m"(_v[12])\
	::"xmm0","memory");			\
}						\
namespace simd {				\
SIMD_PASSIGN_SPEC(n,type)			\
{  						\
    asm volatile ("movups %2, %%xmm0\n\t"	\
	"movups %3, %%xmm1\n\t"			\
	"movups %%xmm0, %0\n\t"			\
	"movups %%xmm1, %1"			\
	: "=m"(oout[0]),"=m"(oout[4])		\
	: "m"(oin[0]),"m"(oin[4])		\
	: "xmm0", "xmm1", "memory");		\
    asm volatile ("movups %2, %%xmm0\n\t"	\
	"movups %3, %%xmm1\n\t"			\
	"movups %%xmm0, %0\n\t"			\
	"movups %%xmm1, %1"			\
	: "=m"(oout[8]),"=m"(oout[12])		\
	: "m"(oin[8]),"m"(oin[12])		\
	: "xmm0", "xmm1", "memory");		\
}						\
}
SSE_TUPLE_SPECS(16,float)
SSE_TUPLE_SPECS(16,int32_t)
SSE_TUPLE_SPECS(16,uint32_t)
#undef SSE_TUPLE_SPECS
#endif

} // namespace ustl
