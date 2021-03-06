﻿//
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
 *\file fundamental_types.hpp
 * \brief Type identification utilities.
 */
namespace v8 { namespace base {

template<typename T, typename U>
struct types_eq {
    enum {
        Yes = 0,
        No = !Yes
    };
};

template<typename T>
struct types_eq<T, T> {
    enum {
        Yes = 1,
        No = !Yes
    };
};

template<typename T>
struct is_floating_point_type {
    enum {
        Yes = types_eq<T, float>::Yes || types_eq<T, double>::Yes 
              || types_eq<T, long double>::Yes,
        No = !Yes
    };
};

template<typename T>
struct is_arithmetic_type {
    enum {
        Yes = types_eq<T, char>::Yes || types_eq<T, unsigned char>::Yes
              || types_eq<T, short>::Yes || types_eq<T, unsigned short>::Yes
              || types_eq<T, int>::Yes || types_eq<T, unsigned int>::Yes
              || types_eq<T, long>::Yes || types_eq<T, unsigned long>::Yes
              || types_eq<T, long long>::Yes 
              || types_eq<T, unsigned long long>::Yes
              || is_floating_point_type<T>::Yes,
        No = !Yes
    };
};

template<typename T>
struct is_builtin_type {
    enum {
        Yes = 0,
        No = 1
    };
};

#ifndef MAKE_BUILTIN_TYPE
#define MAKE_BUILTIN_TYPE(type) \
    template<> \
    struct is_builtin_type<type> { \
        enum { \
            Yes = 1, \
            No = 0 \
        }; \
    }
#endif

MAKE_BUILTIN_TYPE(bool);
MAKE_BUILTIN_TYPE(signed char);
MAKE_BUILTIN_TYPE(unsigned char);
MAKE_BUILTIN_TYPE(char);
MAKE_BUILTIN_TYPE(signed short);
MAKE_BUILTIN_TYPE(unsigned short);
MAKE_BUILTIN_TYPE(signed int);
MAKE_BUILTIN_TYPE(unsigned int);
MAKE_BUILTIN_TYPE(signed long);
MAKE_BUILTIN_TYPE(unsigned long);
MAKE_BUILTIN_TYPE(signed long long);
MAKE_BUILTIN_TYPE(unsigned long long);
MAKE_BUILTIN_TYPE(float);
MAKE_BUILTIN_TYPE(double);
MAKE_BUILTIN_TYPE(long double);

} // namespace base
} // namespace v8
