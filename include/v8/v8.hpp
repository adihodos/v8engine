//
// Copyright (c) 2011, 2012, Adrian Hodos
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the author nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR THE CONTRIBUTORS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

/*!
 * \file v8.hpp
 * \brief Root header for the V8 library.
 * Identifies the compiler and the operating system when
 * building the library. Also contains typedefs for the integer types.
 */

#include <cstddef>

#define V8_STRINGIZE_a(x)                           #x
#define V8_STRINGIZE_w(x)                           L## #x
#define V8_PASTE_X_Y(X, Y)                          X ## Y
#define V8_PASTE_X_Y_Z(X, Y, Z)                     X ## Y ## Z

#define V8_STRING_WIDEN(x)                          V8_STRINGIZE_w(x)

#ifndef __WFILE__
#define __WFILE__                                   V8_STRING_WIDEN(__FILE__)
#endif

#if defined(V8_COMPILER_IS_GCC)
#undef V8_COMPILER_IS_GCC
#endif

#if defined(V8_COMPILER_IS_MINGW)
#undef V8_COMPILER_IS_MINGW
#endif

#if defined(V8_COMPILER_IS_CLANG)
#undef V8_COMPILER_IS_CLANG
#endif

#if defined(V8_COMPILER_IS_MSVC)
#undef V8_COMPILER_IS_MSVC
#endif

///
/// Compiler detection macros. 
/// See : http://sourceforge.net/p/predef/wiki/Compilers/

#if defined(_MSC_VER)

#define     V8_COMPILER_IS_MSVC
#define     V8_COMPILER_STRING          "Microsoft Visual C++ Compiler"
 #define    V8FRAMEWORK_GRAPHICS_API_IS_DIRECTX

#elif defined(__GNUC__) && !defined(__clang__)

#define V8_COMPILER_IS_GCC
#define V8_COMPILER_STRING          "GNU C/C++ Compiler"

#elif defined(__MINGW32__)

#define V8_COMPILER_IS_MINGW          
#define V8_COMPILER_STRING          "MinGW Toolchain"

#elif defined(__GNUC__) && defined(__clang__)

/// clang defines both __GNUC__ and __clang__
#define V8_COMPILER_IS_CLANG
#define V8_COMPILER_STRING          "Clang C/C++ Compiler"

#else

//#error  Unsupported compiler.
#define V8_COMPILER_IS_MSVC
#define V8_COMPILER_STRING          "Microsoft Visual C++ Compiler"


#endif

///
/// OS detection macros. 
/// See : http://sourceforge.net/p/predef/wiki/OperatingSystems/

#if defined(V8_OS_IS_WINDOWS)
#undef V8_OS_IS_WINDOWS
#endif

#if defined(V8_OS_IS_POSIX_COMPLIANT)
#undef V8_OS_IS_POSIX_COMPLIANT
#endif

#if defined(V8_OS_IS_POSIX_FAMILY)
#undef V8_OS_IS_POSIX_FAMILY
#endif

#if defined(V8_OS_IS_LINUX)
#undef V8_OS_IS_LINUX
#endif

#if defined(V8_OS_IS_FREEBSD)
#undef V8_OS_IS_FREEBSD
#endif

#if defined(__FreeBSD__)

#define V8_OS_IS_POSIX_COMPLIANT
#define V8_OS_IS_POSIX_FAMILY
#define V8_OS_IS_FREEBSD
#define V8_OS_STRING                    "FreeBSD"

#elif defined(__gnu_linux__)

#define V8_OS_IS_POSIX_COMPLIANT
#define V8_OS_IS_POSIX_FAMILY
#define V8_OS_IS_LINUX                  
#define V8_OS_STRING                    "GNU Linux"

#elif defined(_WIN32)       || \
    defined(_WIN64)         || \
    defined(__WIN32__)      || \
    defined(__WINDOWS__)

#define V8_OS_IS_WINDOWS
#define V8_OS_STRING                    "Windows"

#else

#error Unsupported system

#endif

#if defined(V8_COMPILER_IS_MSVC)

#include <v8/internal/msvc.hpp>

#elif defined(V8_COMPILER_IS_MINGW)

#include <v8/internal/mingw.hpp>

#elif defined(V8_COMPILER_IS_GCC)

#include <v8/internal/gcc.hpp>

#elif defined(V8_COMPILER_IS_CLANG)

#include <v8/internal/clang.hpp>

#else
#error Unknown compiler
#endif

//! ANSI character type
typedef char                                                v8_char_t;
//! 8 bit integer
typedef V8_SI8BIT_BASE_TYPE                                 v8_int8_t;
//! 8 bit signed integer
typedef V8_SI8BIT_BASE_TYPE                                 v8_sint8_t;
//! 8 bit unsigned integer
typedef V8_UI8BIT_BASE_TYPE                                 v8_uint8_t;

//! 16 bit integer
typedef V8_SI16BIT_BASE_TYPE                                v8_int16_t;
//! 16 bit signed integer
typedef V8_SI16BIT_BASE_TYPE                                v8_sint16_t;
//! 16 bit unsigned integer
typedef V8_UI16BIT_BASE_TYPE                                v8_uint16_t;

//! 32 bit integer
typedef V8_SI32BIT_BASE_TYPE                                v8_int32_t;
//! 32 bit signed integer
typedef V8_SI32BIT_BASE_TYPE                                v8_sint32_t;
//! 32 bit unsigned integer
typedef V8_UI32BIT_BASE_TYPE                                v8_uint32_t;

//! 64 bit integer
typedef V8_SI64BIT_BASE_TYPE                                v8_int64_t;
//! 64 bit signed integer
typedef V8_SI64BIT_BASE_TYPE                                v8_sint64_t;
//! 64 bit unsigned integer
typedef V8_UI64BIT_BASE_TYPE                                v8_uint64_t;

//! Short integer
typedef short                                               v8_short_t;
//! Plain integer
typedef int                                                 v8_int_t;
//! Signed plain integer
typedef signed int                                          v8_sint_t;
//! Unsigned plain integer
typedef unsigned int                                        v8_uint_t;
//! Long integer
typedef long                                                v8_long_t;
//! Unsigned long integer
typedef unsigned long                                       v8_ulong_t;
//! Byte type
typedef v8_uint8_t                                          v8_byte_t;
//! Boolean type
typedef v8_uint_t                                           v8_bool_t;
//! Size type
typedef size_t                                              v8_size_t;
//! Pointer difference type
typedef ptrdiff_t                                           v8_ptrdiff_t;

#if defined(NO_CC_ASSIGN)
#undef NO_CC_ASSIGN
#endif

/*!
 * \def NO_CC_ASSIGN(type_name)
 * \brief   A macro to suppress automatic generation by the compiler of
 *          the copy constructor and assignment operator. It must appear
 *          in the private part of the class.
 * \code
 *  class U {
 *	private :
 *      NO_CC_ASSIGN(U);
 *      ...
 *  };
 * \endcode
 */
#define NO_CC_ASSIGN(type_name)                 \
    type_name(const type_name&);                \
    type_name& operator=(const type_name&)

//!
//! \def V8_GEN_OPAQUE_TYPE(type)
//! \brief Generates a unique type.
#define V8_GEN_OPAQUE_TYPE(type) \
    typedef struct v8_internal_opaque_type ## type { int i; } const* type;

#if defined(V8_COMPILER_IS_MSVC)

#ifndef SUPPRESS_WARNING_START
#define SUPPRESS_WARNING_START(warn_id)         \
    __pragma(warning(push))                     \
    __pragma(warning(disable : warn_id))
#endif /* !SUPPRESS_WARNING */    

#ifndef SUPPRESS_WARNING_END
#define SUPPRESS_WARNING_END()                  \
    __pragma(warning(pop))
#endif /* !SUPPRESS_WARNING_END */

#else /* V8_COMPILER_IS_MSVC */

#ifndef SUPPRESS_WARNING_START
#define SUPPRESS_WARNING_START(warn_id)
#endif /* !SUPPRESS_WARNING */    

#ifndef SUPPRESS_WARNING_END
#define SUPPRESS_WARNING_END()
#endif /* !SUPPRESS_WARNING_END */

#endif /* !V8_COMPILER_IS_MSVC */
