// This file is part of the uSTL library, an STL implementation.
//
// Copyright (c) 2007 by Mike Sharov <msharov@users.sourceforge.net>
//
// This implementation is adapted from the Loki library, distributed under
// the MIT license with Copyright (c) 2001 by Andrei Alexandrescu.

#pragma once
#include "typelist.h"

namespace ustl {
namespace tm {

//----------------------------------------------------------------------
// Type classes and type modifiers
//----------------------------------------------------------------------

using StdUnsignedInts = tl::Seq<unsigned char, unsigned short, unsigned, unsigned long>::Type ;
using StdSignedInts = tl::Seq<signed char, short, int, long>::Type	;
using StdOtherInts = tl::Seq<bool, char, wchar_t>::Type		;
using StdFloats = tl::Seq<float, double>::Type			;

template <typename U> struct Identity			{ using Result = U; };
template <typename U> struct AddPointer			{ using Result = U*; };
template <typename U> struct AddPointer<U&>		{ using Result = U*; };
template <typename U> struct AddReference		{ using Result = U&; };
template <typename U> struct AddReference<U&>		{ using Result = U&; };
template <>           struct AddReference<void>		{ using Result = NullType; };
template <typename U> struct AddParameterType		{ using Result = const U&; };
template <typename U> struct AddParameterType<U&>	{ using Result = U&; };
template <>           struct AddParameterType<void>	{ using Result = NullType; };
template <typename U> struct RemoveReference		{ using Result = U; };
template <typename U> struct RemoveReference<U&>	{ using Result = U; };
template <typename U> struct RemoveReference<U&&>	{ using Result = U; };
template <bool, typename T> struct EnableIf		{ using Result = void; };
template <typename T> struct EnableIf<true, T>		{ using Result = T; };


//----------------------------------------------------------------------
// Function pointer testers
//----------------------------------------------------------------------
// Macros expand to numerous parameters

template <typename T>
struct IsFunctionPointerRaw { enum { result = false}; };
template <typename T>
struct IsMemberFunctionPointerRaw { enum { result = false}; };

#define TM_FPR_MAXN		9
#define TM_FPR_TYPE(n)		PASTE(T,n)
#define TM_FPR_TYPENAME(n)	typename TM_FPR_TYPE(n)

// First specialize for regular functions
template <typename T>
struct IsFunctionPointerRaw<T(*)(void)> 
{enum {result = true};};

#define TM_FPR_SPEC(n)		\
template <typename T, COMMA_LIST(n, TM_FPR_TYPENAME)>		\
struct IsFunctionPointerRaw<T(*)(COMMA_LIST(n, TM_FPR_TYPE))>	\
{ enum { result = true }; }

LIST (TM_FPR_MAXN, TM_FPR_SPEC, ;);

// Then for those with an ellipsis argument
template <typename T>
struct IsFunctionPointerRaw<T(*)(...)> 
{enum {result = true};};

#define TM_FPR_SPEC_ELLIPSIS(n)	\
template <typename T, COMMA_LIST(n, TM_FPR_TYPENAME)>			\
struct IsFunctionPointerRaw<T(*)(COMMA_LIST(n, TM_FPR_TYPE), ...)>	\
{ enum { result = true }; }

LIST (TM_FPR_MAXN, TM_FPR_SPEC_ELLIPSIS, ;);

// Then for member function pointers
template <typename T, typename S>
struct IsMemberFunctionPointerRaw<T (S::*)(void)> 
{ enum { result = true }; };

#define TM_MFPR_SPEC(n)		\
template <typename T, typename S, COMMA_LIST(n, TM_FPR_TYPENAME)>	\
struct IsMemberFunctionPointerRaw<T (S::*)(COMMA_LIST(n, TM_FPR_TYPE))>	\
{ enum { result = true };};

LIST (TM_FPR_MAXN, TM_MFPR_SPEC, ;);

// Then for member function pointers with an ellipsis argument
template <typename T, typename S>
struct IsMemberFunctionPointerRaw<T (S::*)(...)> 
{ enum { result = true }; };

#define TM_MFPR_SPEC_ELLIPSIS(n)		\
template <typename T, typename S, COMMA_LIST(n, TM_FPR_TYPENAME)>	\
struct IsMemberFunctionPointerRaw<T (S::*)(COMMA_LIST(n, TM_FPR_TYPE), ...)> \
{ enum { result = true }; };

LIST (TM_FPR_MAXN, TM_MFPR_SPEC_ELLIPSIS, ;);

// Then for const member function pointers (getting tired yet?)
template <typename T, typename S>
struct IsMemberFunctionPointerRaw<T (S::*)(void) const> 
{ enum { result = true }; };

#define TM_CMFPR_SPEC(n)	\
template <typename T, typename S, COMMA_LIST(n, TM_FPR_TYPENAME)>	\
struct IsMemberFunctionPointerRaw<T (S::*)(COMMA_LIST(n, TM_FPR_TYPE)) const>	\
{ enum { result = true };};

LIST (TM_FPR_MAXN, TM_CMFPR_SPEC, ;);

// Finally for const member function pointers with an ellipsis argument (whew!)
template <typename T, typename S>
struct IsMemberFunctionPointerRaw<T (S::*)(...) const> 
{ enum { result = true }; };

#define TM_CMFPR_SPEC_ELLIPSIS(n)		\
template <typename T, typename S, COMMA_LIST(n, TM_FPR_TYPENAME)>	\
struct IsMemberFunctionPointerRaw<T (S::*)(COMMA_LIST(n, TM_FPR_TYPE), ...) const> \
{ enum { result = true }; };

LIST (TM_FPR_MAXN, TM_CMFPR_SPEC_ELLIPSIS, ;);

#undef TM_FPR_SPEC
#undef TM_FPR_SPEC_ELLIPSIS
#undef TM_MFPR_SPEC
#undef TM_MFPR_SPEC_ELLIPSIS
#undef TM_CMFPR_SPEC
#undef TM_CMFPR_SPEC_ELLIPSIS
#undef TM_FPR_TYPENAME
#undef TM_FPR_TYPE
#undef TM_FPR_MAXN

//----------------------------------------------------------------------
// Type traits template
//----------------------------------------------------------------------

/// Figures out at compile time various properties of any given type
/// Invocations (T is a type, TypeTraits<T>::Propertie):
///
/// - isPointer       : returns true if T is a pointer type
/// - PointeeType     : returns the type to which T points if T is a pointer 
///                     type, NullType otherwise
/// - isReference     : returns true if T is a reference type
/// - isLValue        : returns true if T is an lvalue
/// - isRValue        : returns true if T is an rvalue
/// - ReferredType    : returns the type to which T refers if T is a reference 
///                     type, NullType otherwise
/// - isMemberPointer : returns true if T is a pointer to member type
/// - isStdUnsignedInt: returns true if T is a standard unsigned integral type
/// - isStdSignedInt  : returns true if T is a standard signed integral type
/// - isStdIntegral   : returns true if T is a standard integral type
/// - isStdFloat      : returns true if T is a standard floating-point type
/// - isStdArith      : returns true if T is a standard arithmetic type
/// - isStdFundamental: returns true if T is a standard fundamental type
/// - isUnsignedInt   : returns true if T is a unsigned integral type
/// - isSignedInt     : returns true if T is a signed integral type
/// - isIntegral      : returns true if T is a integral type
/// - isFloat         : returns true if T is a floating-point type
/// - isArith         : returns true if T is a arithmetic type
/// - isFundamental   : returns true if T is a fundamental type
/// - ParameterType   : returns the optimal type to be used as a parameter for 
///                     functions that take Ts
/// - isConst         : returns true if T is a const-qualified type
/// - NonConstType    : Type with removed 'const' qualifier from T, if any
/// - isVolatile      : returns true if T is a volatile-qualified type
/// - NonVolatileType : Type with removed 'volatile' qualifier from T, if any
/// - UnqualifiedType : Type with removed 'const' and 'volatile' qualifiers from 
///                     T, if any
/// - ConstParameterType: returns the optimal type to be used as a parameter 
///                       for functions that take 'const T's
///
template <typename T>
class TypeTraits {
private:
    #define TMTT1	template <typename U> struct
    #define TMTT2	template <typename U, typename V> struct
    TMTT1 ReferenceTraits	{ enum { result = false, lvalue = true, rvalue = false }; using ReferredType = U; };
    TMTT1 ReferenceTraits<U&>	{ enum { result = true,  lvalue = true, rvalue = false }; using ReferredType = U; };
    TMTT1 PointerTraits		{ enum { result = false }; using PointeeType = NullType; };
    TMTT1 PointerTraits<U*>	{ enum { result = true  }; using PointeeType = U; };
    TMTT1 PointerTraits<U*&>	{ enum { result = true  }; using PointeeType = U; };
    TMTT1 PToMTraits		{ enum { result = false }; };
    TMTT2 PToMTraits<U V::*>	{ enum { result = true  }; };
    TMTT2 PToMTraits<U V::*&>	{ enum { result = true  }; };
    TMTT1 FunctionPointerTraits	{ enum { result = IsFunctionPointerRaw<U>::result }; };
    TMTT1 PToMFunctionTraits	{ enum { result = IsMemberFunctionPointerRaw<U>::result }; };
    TMTT1 UnConst		{ using Result = U;  enum { isConst = false }; };
    TMTT1 UnConst<const U>	{ using Result = U;  enum { isConst = true  }; };
    TMTT1 UnConst<const U&>	{ using Result = U&; enum { isConst = true  }; };
    TMTT1 UnVolatile		{ using Result = U;  enum { isVolatile = false }; };
    TMTT1 UnVolatile<volatile U>{ using Result = U;  enum { isVolatile = true  }; };
    TMTT1 UnVolatile<volatile U&> {using Result = U&;enum { isVolatile = true  }; };
    TMTT1 ReferenceTraits<U&&>	{ enum { result = true,  lvalue = false, rvalue = true }; using ReferredType = U; };
    TMTT1 PointerTraits<U*&&>	{ enum { result = true  }; using PointeeType = U; };
    TMTT2 PToMTraits<U V::*&&>	{ enum { result = true  }; };
    TMTT1 UnConst<const U&&>	{ using Result = U&&; enum { isConst = true  }; };
    TMTT1 UnVolatile<volatile U&&> {using Result = U&&;enum { isVolatile = true  }; };
    #undef TMTT2
    #undef TMTT1
public:
    using NonConstType		= typename UnConst<T>::Result;
    using NonVolatileType	= typename UnVolatile<T>::Result;
    using UnqualifiedType	= typename UnVolatile<typename UnConst<T>::Result>::Result;
    using PointeeType		= typename PointerTraits<UnqualifiedType>::PointeeType;
    using ReferredType		= typename ReferenceTraits<T>::ReferredType;

    enum { isConst		= UnConst<T>::isConst };
    enum { isVolatile		= UnVolatile<T>::isVolatile };
    enum { isReference		= ReferenceTraits<UnqualifiedType>::result };
    enum { isLValue		= ReferenceTraits<UnqualifiedType>::lvalue };
    enum { isRValue		= ReferenceTraits<UnqualifiedType>::rvalue };
    enum { isFunction		= FunctionPointerTraits<typename AddPointer<T>::Result >::result };
    enum { isFunctionPointer	= FunctionPointerTraits<
				    typename ReferenceTraits<UnqualifiedType>::ReferredType >::result };
    enum { isMemberFunctionPointer= PToMFunctionTraits<
				    typename ReferenceTraits<UnqualifiedType>::ReferredType >::result };
    enum { isMemberPointer	= PToMTraits<
				    typename ReferenceTraits<UnqualifiedType>::ReferredType >::result ||
				    isMemberFunctionPointer };
    enum { isPointer		= PointerTraits<
				    typename ReferenceTraits<UnqualifiedType>::ReferredType >::result ||
				    isFunctionPointer };
    enum { isStdUnsignedInt	= tl::IndexOf<StdUnsignedInts, UnqualifiedType>::value >= 0 ||
				    tl::IndexOf<StdUnsignedInts,
					typename ReferenceTraits<UnqualifiedType>::ReferredType>::value >= 0};
    enum { isStdSignedInt	= tl::IndexOf<StdSignedInts, UnqualifiedType>::value >= 0 ||
				    tl::IndexOf<StdSignedInts,
					typename ReferenceTraits<UnqualifiedType>::ReferredType>::value >= 0};
    enum { isStdIntegral	= isStdUnsignedInt || isStdSignedInt ||
				    tl::IndexOf<StdOtherInts, UnqualifiedType>::value >= 0 ||
				    tl::IndexOf<StdOtherInts,
					typename ReferenceTraits<UnqualifiedType>::ReferredType>::value >= 0};
    enum { isStdFloat		= tl::IndexOf<StdFloats, UnqualifiedType>::value >= 0 ||
				    tl::IndexOf<StdFloats,
					typename ReferenceTraits<UnqualifiedType>::ReferredType>::value >= 0};
    enum { isStdArith		= isStdIntegral || isStdFloat };
    enum { isStdFundamental	= isStdArith || isStdFloat || Conversion<T, void>::sameType };
	
    enum { isUnsignedInt	= isStdUnsignedInt };
    enum { isUnsigned		= isUnsignedInt || isPointer };
    enum { isSignedInt		= isStdSignedInt };
    enum { isIntegral		= isStdIntegral || isUnsignedInt || isSignedInt };
    enum { isFloat		= isStdFloat };
    enum { isSigned		= isSignedInt || isFloat };
    enum { isArith		= isIntegral || isFloat };
    enum { isFundamental	= isStdFundamental || isArith };
    
    using ParameterType = typename Select<isStdArith || isPointer || isMemberPointer, T, typename AddParameterType<T>::Result>::Result ;
};

} // namespace tm
} // namespace ustl
