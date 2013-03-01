#pragma once

/*!
 * \file clang.hpp 
 * \brief CLANG compiler specific configuration
 */

//
// 8 bit integer
#define V8_8BIT_INT_SUPPORT
#define V8_SI8BIT_BASE_TYPE     signed      char
#define V8_UI8BIT_BASE_TYPE     unsigned    char

//
// 16 bit integer
#define V8_16BIT_INT_SUPPORT
#define V8_SI16BIT_BASE_TYPE    signed      short
#define V8_UI16BIT_BASE_TYPE    unsigned    short

//
// 32 bit integer
#define V8_32BIT_INT_SUPPORT 
#define V8_SI32BIT_BASE_TYPE    signed      int
#define V8_UI32BIT_BASE_TYPE    unsigned    int

//
// 64 bit integer
#define V8_64BIT_INT_SUPPORT
#define V8_SI64BIT_BASE_TYPE    signed      long long
#define V8_UI64BIT_BASE_TYPE    unsigned    long long

#define NOEXCEPT                noexcept
#define CONSTEXPR               constexpr

#if (__clang_major__ == 2) && (__clang_minor__ >= 9)

///! CLANG has C++11 features since version 2.9 and upwards.
#define V8_COMPILER_HAS_CXX11_SUPPORT

#endif /* (__clang_major__ == 2) && (__clang_minor__ >= 9) */


#if (__clang_major__ >= 3)

///! CLANG has C++11 features since version 2.9 and upwards.
#define V8_COMPILER_HAS_CXX11_SUPPORT


///! Support for static (compile time) assertions
#if __has_feature(cxx_static_assert)

#define V8_COMPILER_HAS_CXX11_STATIC_ASSERT

#endif

///! Support for generalized constant expressions
#if __has_feature(cxx_constexpr)

#define V8_COMPILER_HAS_CXX11_CONSTEXPR

#endif

//! Support for move semantics
#if __has_feature(cxx_implicit_moves)

#ifndef V8_COMPILER_HAS_CXX11_MOVE_SEMANTICS

#define V8_COMPILER_HAS_CXX11_MOVE_SEMANTICS

#endif /* V8_COMPILER_HAS_CXX11_MOVE_SEMANTICS */

#endif

#if __has_feature(cxx_variadic_templates)

///! Support for templates with variable number of arguments
#define V8_COMPILER_HAS_CXX11_VARIADIC_TEMPLATES

#endif

#if __has_feature(cxx_decltype)

///! Support for decltype() operator
#define V8_COMPILER_HAS_CXX11_DECLTYPE

#endif

#if __has_feature(cxx_strong_enums)

///! Support for strongly typed enumeration types
#define V8_COMPILER_HAS_CXX11_STRONGLY_TYPES_ENUMS

///! Support for forward declaration of enumerated types
#define V8_COMPILER_HAS_CXX11_FORWARD_DECLARED_ENUMS

#endif

#if __has_feature(cxx_nullptr)

///! Support for nullptr
#define V8_COMPILER_HAS_CXX11_NULLPTR

#endif

#if __has_feature(cxx_defaulted_functions) && __has_feature(cxx_deleted_functions)
///! Support for defaulted and deleted functions
#define V8_COMPILER_HAS_CXX11_DEFAULTED_DELETED_FUNCTIONS

#endif

#if __has_feature(cxx_lambdas)

///! Support for lambdas
#define V8_COMPILER_HAS_CXX11_LAMBDAS

#endif

#if __has_feature(cxx_generalized_initializers)

///! Support for initializer lists
#define V8_COMPILER_HAS_CXX11_INITIALIZER_LISTS

#endif

#endif /* (__clang_major__ >= 3) */
