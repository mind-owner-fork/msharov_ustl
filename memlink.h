// This file is part of the uSTL library, an STL implementation.
//
// Copyright (c) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#pragma once
#include "cmemlink.h"
#include "ualgo.h"

namespace ustl {

/// \class memlink memlink.h ustl.h
/// \ingroup MemoryManagement
///
/// \brief Wrapper for pointer to block with size.
///
/// Use this class the way you would a pointer to an allocated unstructured block.
/// The pointer and block size are available through member functions and cast operator.
///
/// Example usage:
/// \code
///     void* p = malloc (46721);
///     memlink a, b;
///     a.link (p, 46721);
///     assert (a.size() == 46721));
///     b = a;
///     assert (b.size() == 46721));
///     assert (b.begin() + 34 == a.begin + 34);
///     assert (0 == memcmp (a, b, 12));
///     a.fill (673, b, 42, 67);
///     b.erase (87, 12);
/// \endcode
///
class memlink : public cmemlink {
public:
    using pointer	= value_type*;
    using const_pointer	= cmemlink::pointer;
    using const_iterator= cmemlink::const_iterator;
    using iterator	= pointer;
    using rcself_t	= const memlink&;
public:
    constexpr		memlink (void)				: cmemlink() {}
    constexpr		memlink (void* p, size_type n)		: cmemlink (p, n) {}
    constexpr		memlink (const void* p, size_type n)	: cmemlink (p, n) {}
    constexpr		memlink (rcself_t l)			: cmemlink (l) {}
    constexpr explicit	memlink (const cmemlink& l)		: cmemlink (l) {}
    constexpr pointer	data (void)				{ return const_cast<pointer>(cmemlink::data()); }
    constexpr const_pointer data (void) const			{ return cmemlink::data(); }
    constexpr iterator	begin (void)				{ return iterator (data()); }
    constexpr iterator	iat (size_type i)			{ assert (i <= size()); return begin() + i; }
    constexpr iterator	end (void)				{ return iat (size()); }
    constexpr const_iterator	begin (void) const		{ return cmemlink::begin(); }
    constexpr const_iterator	end (void) const		{ return cmemlink::end(); }
    constexpr const_iterator	iat (size_type i) const		{ return cmemlink::iat (i); }
    constexpr size_type	writable_size (void) const		{ return size(); }
    inline rcself_t	operator= (const cmemlink& l)		{ cmemlink::operator= (l); return *this; }
    inline rcself_t	operator= (rcself_t l)			{ cmemlink::operator= (l); return *this; }
    inline void		link (const void* p, size_type n)	{ cmemlink::link (p, n); }
    inline void		link (void* p, size_type n)		{ cmemlink::link (p, n); }
    inline void		link (const cmemlink& l)		{ cmemlink::link (l); }
    inline void		link (memlink& l)			{ cmemlink::link (l); }
    inline void		link (const void* first, const void* last)	{ link (first, distance (first, last)); }
    inline void		link (void* first, void* last)		{ link (first, distance (first, last)); }
    constexpr void	relink (const void* p, size_type n)	{ cmemlink::relink (p, n); }
    constexpr void	relink (void* p, size_type n)		{ cmemlink::relink (p, n); }
    constexpr void	swap (memlink& l)			{ cmemlink::swap (l); }
    void		fill (const_iterator start, const void* p, size_type elsize, size_type elCount = 1) noexcept;
    inline void		insert (const_iterator start, size_type size);
    inline void		erase (const_iterator start, size_type size);
    void		read (istream& is);
};

/// Shifts the data in the linked block from \p start to \p start + \p n.
/// The contents of the uncovered bytes is undefined.
inline void memlink::insert (const_iterator cstart, size_type n)
{
    assert (data() || !n);
    assert (cmemlink::begin() || !n);
    assert (cstart >= begin() && cstart + n <= end());
    iterator start = const_cast<iterator>(cstart);
    rotate (start, end() - n, end());
}

/// Shifts the data in the linked block from \p start + \p n to \p start.
/// The contents of the uncovered bytes is undefined.
inline void memlink::erase (const_iterator cstart, size_type n)
{
    assert (data() || !n);
    assert (cmemlink::begin() || !n);
    assert (cstart >= begin() && cstart + n <= end());
    iterator start = const_cast<iterator>(cstart);
    rotate (start, start + n, end());
}

/// Use with memlink-derived classes to allocate and link to stack space.
#define alloca_link(m,n)	(m).link (alloca (n), (n))

} // namespace ustl
