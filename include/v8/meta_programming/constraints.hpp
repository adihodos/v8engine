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

//!
//! \file constraints.hpp
//! \bref Class to enforce constraints on types, at compile time.

#if defined(V8_COMPILER_HAS_CXX11_SUPPORT)
#include <type_traits>
#endif /* V8_COMPILER_HAS_CXX11_INITIALIZER_LISTS */

#include <v8/v8.hpp>
#include <v8/base/util.hpp>

namespace v8 { namespace base {

//! \defgroup   Constraints 
//!  Template classes to enforce constraints on template parameters types.
//! @{

//!
//! \brief   Enforces that the two types passed to a template entity have the
//!          same size.    
//! \code
//!  struct A {
//!      char a[1];
//!  };
//!
//!  struct B : public A {
//!  };
//!
//!  struct C : public A {
//!      int c;
//!  };
//!
//!  must_be_same_size<A, B>();  // ok
//!  must_be_same_size<A, C>();  // compile error
//! \code
template<
    typename T,
    typename U
> struct must_be_same_size {
private :

    struct constraint {
        int T_must_be_same_size_as_U : (sizeof(T) == sizeof(U));
        int T_is_the_same_size_as_U;
    };

    static void check(int constraint::*) {}

public :
    ~must_be_same_size() {
        int constraint::*constraint_is_violated =
            &constraint::T_is_the_same_size_as_U;
        check(constraint_is_violated);
    }
};

//!
//! \brief   Enforces the constraint that the first template argument is a
//!          base class for the second template argument.
//! \code
//!  struct A {
//!      char a[1];
//!  };
//! 
//!  struct B {
//!  };
//!
//!  struct C : public A {
//!      int c;
//!  };
//!
//!  must_be_base_class<A, C>(); // ok
//!  must_be_base_class<A, B>(); // compile error
//! \code
template<
    typename base_class_type,
    typename derived_class_type
> struct must_be_base_class {
public :

    ~must_be_base_class() {
        void (*pfn)(base_class_type*, derived_class_type*) = check_constraint;
        internals::suppress_warn_unused(pfn);
    }    

private :

    static void check_constraint(
        base_class_type* base_class_ptr,
        derived_class_type* derived_class_ptr
        ) {
        base_class_ptr = derived_class_ptr;
        internals::suppress_warn_unused(base_class_ptr);
    }
};

//!
//! \brief Enforces the constraint that a type is subscriptable (is an array,
//!        pointer, or a user defined type that provides operator[]).
//! \code
//!  struct A {};
//!
//!  struct B {
//!      int arr[10];
//!
//!      int& operator[](int i) {
//!          return arr[i];
//!      }
//!  };
//!
//!  typedef int* int_ptr;
//!
//!  must_be_subscriptable<A>(); // compile error
//!  must_be_subscriptable<B>(); // ok
//!  must_be_subscriptable<int_ptr>(); // ok
//!  \code
template<typename T>
struct must_be_subscriptable {
public :

    ~must_be_subscriptable() {
        size_t (*constraint_is_violated)(T&) = check_constraint;
        internals::suppress_warn_unused(constraint_is_violated);
    }

private :

    static size_t check_constraint(T& T_is_not_subscriptable) {
        return sizeof(T_is_not_subscriptable[0]);
    }

};

//!
//! Enforce at compile time that the template argument is a POD type.
//! \code
//!
//! class AClass {
//! public :
//!     A() {}
//! };
//! 
//! class BClass {
//! public :
//!     B(int) {}
//! };
//!
//! struct X11Data {
//!     void*   dy;
//!     int     i;
//!     char    j;
//!     unsigned int arr[4];
//! };
//!
//! constraint_must_be_pod<int>(); // ok
//! constraint_must_be_pod<X11Data>(); // ok
//! constraint_must_be_pod<AClass>(); // compile error
//! constraint_must_be_pod<BClass>(); // compile error
//! \endcode
template<typename T>
struct constraint_must_be_pod {
public :
    ~constraint_must_be_pod() {
        check_constraint(&constraint::T_is_POD);
    }

private :

    union constraint {

#if defined(V8_COMPILER_HAS_CXX11_SUPPORT)

        v8_int_t        T_must_be_POD : std::is_pod<T>::value;

#else /* V8_COMPILER_HAS_CXX11_SUPPORT */

        T               T_must_be_POD;

#endif /* !V8_COMPILER_HAS_CXX11_SUPPORT */

        v8_int_t        T_is_POD;
    };

    static void check_constraint(v8_int_t constraint::*) {}
};

//! @}

} // namespace base
} // namespace v8
