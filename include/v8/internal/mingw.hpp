#pragma once

/*!
 * \file mingw.hpp
 * \brief MinGW specific configuration
 */

// 8 bit integer
#define V8_8BIT_INT_SUPPORT
#define V8_SI8BIT_BASE_TYPE     signed      char
#define V8_UI8BIT_BASE_TYPE     unsigned    char

// 16 bit integer
#define V8_16BIT_INT_SUPPORT
#define V8_SI16BIT_BASE_TYPE    signed      short
#define V8_UI16BIT_BASE_TYPE    unsigned    short

// 32 bit integer
#define V8_32BIT_INT_SUPPORT 
#define V8_SI32BIT_BASE_TYPE    signed      int
#define V8_UI32BIT_BASE_TYPE    unsigned    int

// 64 bit integer
#define V8_64BIT_INT_SUPPORT
#define V8_SI64BIT_BASE_TYPE    signed      long long
#define V8_UI64BIT_BASE_TYPE    unsigned    long long

#define NOEXCEPT                noexcept
#define CONSTEXPR               constexpr

#if (__GNUC__ >= 4)

///! C++11 features in GCC 4.3 series 
#if (__GNUC_MINOR__ >= 3)

///! GCC has C++11 features since version 4.3 and upwards.
#define V8_COMPILER_HAS_CXX11_SUPPORT

///! Support for templates with variable number of arguments
#define V8_COMPILER_HAS_CXX11_VARIADIC_TEMPLATES

//! Support for move semantics
#define V8_COMPILER_HAS_CXX11_MOVE_SEMANTICS 

///! Support for static (compile time) assertions
#define V8_COMPILER_HAS_CXX11_STATIC_ASSERT

///! Support for decltype() operator
#define V8_COMPILER_HAS_CXX11_DECLTYPE 

#endif // __GNUC_MINOR__ >= 3

///! C++11 features since 4.4 series
#if (__GNUC_MINOR__ >= 4) 

///! Support for initializer lists
#define V8_COMPILER_HAS_CXX11_INITIALIZER_LISTS

///! Support for strongly typed enumeration types
#define V8_COMPILER_HAS_CXX11_STRONGLY_TYPES_ENUMS

///! Support for defaulted and deleted functions
#define V8_COMPILER_HAS_CXX11_DEFAULTED_DELETED_FUNCTIONS 

#endif // __GNUC_MINOR__ >= 4 

#if (__GNUC_MINOR__ >= 5)

///! Support for lambdas 
#define V8_COMPILER_HAS_CXX11_LAMBDAS

#endif // __GNUC_MINOR__ >= 5 

///! C++11 features since 4.6 series
#if (__GNUC_MINOR__ >= 6)

///! Support for nullptr 
#define V8_COMPILER_HAS_CXX11_NULLPTR

///! Support for generalized constant expressions
#define V8_COMPILER_HAS_CXX11_CONSTEXPR

///! Support for forward declaration of enumerated types 
#define V8_COMPILER_HAS_CXX11_FORWARD_DECLARED_ENUMS 

#endif // __GNUC_MINOR__ >= 6

#if (__GNUC_MINOR__ >= 7)

///! Support for range based for loops 
#define V8_COMPILER_HAS_CXX11_RANGED_FOR_LOOP

#endif // __GNUC_MINOR__ >= 7 
 