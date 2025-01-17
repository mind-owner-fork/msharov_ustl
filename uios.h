// This file is part of the uSTL library, an STL implementation.
//
// Copyright (c) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#pragma once
#include "utypes.h"

namespace ustl {

class file_exception;

/// Defines types and constants used by all stream classes.
class ios_base {
public:
    /// Used to set parameters for stringstreams
    enum fmtflags_bits {
	boolalpha	= (1 << 0),	///< Boolean values printed as text.
	showbase	= (1 << 1),	///< Add 0x or 0 prefixes on hex and octal numbers.
	showpoint	= (1 << 2),	///< Print decimal point.
	showpos		= (1 << 3),
	skipws		= (1 << 4),	///< Skip whitespace when reading.
	unitbuf		= (1 << 5),
	uppercase	= (1 << 6),
	dec		= (1 << 7),	///< Decimal number output.
	oct		= (1 << 8),	///< Octal number output.
	hex		= (1 << 9),	///< Hexadecimal number output.
	fixed		= (1 << 10),	///< Fixed-point float output.
	scientific	= (1 << 11),	///< Scientific float format.
	left		= (1 << 12),	///< Left alignment.
	right		= (1 << 13),	///< Right alignment.
	internal	= (1 << 14),
	basefield	= dec| oct| hex,
	floatfield	= fixed| scientific,
	adjustfield	= left| right| internal
    };
    /// For file-based streams, specifies fd mode.
    enum openmode_bits {
	in	= (1 << 0),
	out	= (1 << 1),
	app	= (1 << 2),
	ate	= (1 << 3),
	binary	= (1 << 4),
	trunc	= (1 << 5),
	#ifndef DOXYGEN_SHOULD_SKIP_THIS
	nonblock= (1 << 6),
	nocreate= (1 << 7),
	noctty	= (1 << 8),
	nombits	= 9
	#endif
    };
    /// Seek directions, equivalent to SEEK_SET, SEEK_CUR, and SEEK_END.
    enum seekdir {
	beg,
	cur,
	end
    };
    /// I/O state bitmasks.
    enum iostate_bits {
	goodbit	= 0,
	badbit	= (1 << 0),
	eofbit	= (1 << 1),
	failbit	= (1 << 2),
	#ifndef DOXYGEN_SHOULD_SKIP_THIS
	nbadbits = 3,
	allbadbits = 0x7
	#endif
    };

    enum { default_stream_buffer_size = 4095 };

    using openmode	= uint32_t;	///< Holds openmode_bits.
    using fmtflags	= uint32_t;	///< Holds fmtflags_bits for a string stream.
    using iostate	= uint32_t;	///< Holds iostate_bits for a file stream.
    using failure	= file_exception;	///< Thrown by fstream on errors.

    static const char c_DefaultDelimiters [16];	///< Default word delimiters for stringstreams.
public:
    constexpr		ios_base (void)			: _state (goodbit), _exceptions (allbadbits) {}
    constexpr iostate	rdstate (void) const		{ return _state; }
    constexpr bool	bad (void) const		{ return rdstate() & badbit; }
    constexpr bool	good (void) const		{ return rdstate() == goodbit; }
    constexpr bool	fail (void) const		{ return rdstate() & (badbit | failbit); }
    constexpr bool	eof (void) const		{ return rdstate() & eofbit; }
    constexpr bool	operator! (void) const		{ return fail(); }
    constexpr		operator bool (void) const	{ return !fail(); }
    constexpr void	clear (iostate v = goodbit)	{ _state = v; }
    constexpr void	setstate (iostate v)		{ _state |= v; }
    constexpr iostate	exceptions (void) const		{ return _exceptions; }
    constexpr iostate	exceptions (iostate v)		{ return _exceptions = v; }
protected:
    constexpr bool	set_and_throw (iostate v)	{ setstate(v); return exceptions() & v; }
    void		overrun (const char* op, const char* type, uint32_t n, uint32_t p, uint32_t rem);
private:
    uint16_t		_state;		///< Open state, using ios::iostate_bits.
    uint16_t		_exceptions;	///< Exception flags, using ios::iostate_bits.
};

} // namespace ustl
