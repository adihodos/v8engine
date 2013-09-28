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
 * \file msvc.hpp
 * \brief Configuration settings for MS Visual C++ compiler.
 */

// 8 bit integer
#define V8_8BIT_INT_SUPPORT
#define V8_SI8BIT_BASE_TYPE     __int8
#define V8_UI8BIT_BASE_TYPE     unsigned __int8

// 16 bit integer
#define V8_16BIT_INT_SUPPORT
#define V8_SI16BIT_BASE_TYPE    __int16
#define V8_UI16BIT_BASE_TYPE    unsigned __int16

// 32 bit integer
#define V8_32BIT_INT_SUPPORT
#define V8_SI32BIT_BASE_TYPE    __int32
#define V8_UI32BIT_BASE_TYPE    unsigned __int32

// 64 bit integer
#define V8_64BIT_INT_SUPPORT
#define V8_SI64BIT_BASE_TYPE    __int64
#define V8_UI64BIT_BASE_TYPE    unsigned __int64

#define NOEXCEPT                throw()
#define CONSTEXPR

///! C++11 features present in Visual C++ 10.0
#if (_MSC_VER >= 1600)

#define V8_COMPILER_HAS_CXX11_SUPPORT

///! Support for move semantics
#define V8_COMPILER_HAS_CXX11_MOVE_SEMANTICS

///! Support for lambdas 
#define V8_COMPILER_HAS_CXX11_LAMBDAS

///! Support for trailing return types
#define V8_COMPILER_HAS_CXX11_TRAILING_RETURN_TYPES

///! Support for nullptr 
#define V8_COMPILER_HAS_CXX11_NULLPTR

///! Support for static (compile time) assertions
#define V8_COMPILER_HAS_CXX11_STATIC_ASSERT

///! Support for decltype() operator
#define V8_COMPILER_HAS_CXX11_DECLTYPE 

#endif // _MSC_VER >= 1600

///! C++ features present in Visual C++ 11.0
#if (_MSC_VER >= 1700)

///! Support for strongly typed enumeration types
#define V8_COMPILER_HAS_CXX11_STRONGLY_TYPES_ENUMS

///! Support for forward declaration of enumerated types 
#define V8_COMPILER_HAS_CXX11_FORWARD_DECLARED_ENUMS
 
///! Support for range based for loops 
#define V8_COMPILER_HAS_CXX11_RANGED_FOR_LOOP

#endif // _MSC_VER >= 1700

///! C++ features present in Visual C++ 12.0
#if (_MSC_VER >= 1800)

///!    Support for defaulted and deleted functions
#define V8_COMPILER_HAS_CXX11_DEFAULTED_DELETED_FUNCTIONS

///!    Support for initializer list syntax
#define V8_COMPILER_HAS_CXX11_INITIALIZER_LISTS

#endif //   (_MSC_VER >= 1800)
