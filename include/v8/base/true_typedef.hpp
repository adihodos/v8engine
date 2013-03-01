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
//! \file true_typedef.hpp

#include <v8/v8.hpp>
#include <v8/meta_programming/param_select.hpp>

#if defined(V8_COMPILER_HAS_CXX11_INITIALIZER_LISTS)
#include <initializer_list>
#endif /* V8_COMPILER_HAS_CXX11_INITIALIZER_LISTS */

#if defined(V8_COMPILER_HAS_CXX11_MOVE_SEMANTICS)
#include <utility>
#endif /* V8_COMPILER_HAS_CXX11_MOVE_SEMANTICS */

namespace v8 { namespace base {

//! Enables strongly typed types, based on the same underlying type.
//!
//! The following code creates two difference conceptual types 
//! (a mutex handle and a file handle) based on the same underlying
//! type (a Windows API HANDLE).
//! \code
//! V8_GEN_OPAQUE_TYPE(file_descriptor_u)
//! V8_GEN_OPAQUE_TYPE(mutex_descriptor_u);
//! 
//! typedef true_typedef<HANDLE, file_descriptor_u>     H_file;
//! typedef true_typedef<HANDLE, mutex_descriptor_u>    H_mutex;
//!
//! H_file fp = CreateFile(...);
//! H_mutex mtx = CreateMutex(...);
//! fp = mtx; // Compile error, cannot assign H_mutex to H_file
//! H_mutex mtx2(mtx); // ok
//! \endcode
//! \note The "true typedef" concept is described in detail in
//! "Imperfect C++ - Practical Solutions for Real-Life Programming",
//! section 18.4 
template<typename T, typename U>
class true_typedef {
public :
    //! \name Type definitions
    //! @{

    //! The value type.
    typedef T                                               value_type;

    //! The unique type.
    typedef U                                               unique_type;

    //! This class's type.
    typedef true_typedef<T, U>                              self_type;

    //! Parameter type.
    typedef typename param_select<T>::param_type            param_type;

    //! Const parameter type.
    typedef typename param_select<T>::const_param_type      const_param_type;

    //! @}

public :

    //! \name Constructors
    //! @{

    true_typedef() : val_() {}

    explicit true_typedef(
        const_param_type val
        )
        : val_(val) {}

    true_typedef(
        const self_type& rhs
        )
        : val_(rhs.val_) {}

#if defined(V8_COMPILER_HAS_CXX11_MOVE_SEMANTICS)

    true_typedef(
        self_type&& rhs
        )
        : val_(std::move(rhs.val_)) {}

#endif /* V8_COMPILER_HAS_CXX11_MOVE_SEMANTICS */    

    ~true_typedef() {}

    //! @}

public :

    //! \name Underlying value access
    //! @{

    const_param_type base_type_value() const NOEXCEPT {
        return val_;
    }

    //! @}

public :
    //! \name Self assign operators
    //! @{

    self_type& operator=(
        const self_type& rhs
        ) {
        val_ = rhs.val_;
        return *this;
    }

#if defined(V8_COMPILER_HAS_CXX11_MOVE_SEMANTICS)    

    self_type& operator=(
        self_type&& rhs
        ) {
        val_ = std::move(rhs.val_);
        return *this;
    }

#endif /* V8_COMPILER_HAS_CXX11_MOVE_SEMANTICS */    

    //! @}

public :

    //! \name Pre/post increment/decrement.
    //! @{
    
    self_type& operator++() {
        ++val_;
        return *this;
    }

    self_type& operator--() {
        --val_;
        return *this;
    }

    self_type operator++(int) {
        self_type old_val(*this);
        ++*this;
        return old_val;
    }

    self_type operator--(int) {
        self_type old_val(*this);
        --*this;
        return old_val;
    }

    //! @}

public :
    //! \name Self assign arithmetic operators.
    //! @{

    self_type& operator+=(
        const self_type& rhs
        ) {
        val_ += rhs.val_;
        return *this;
    }

    self_type& operator-=(
        const self_type& rhs
        ) {
        val_ -= rhs.val_;
        return *this;
    }

    self_type& operator*=(
        const self_type& rhs
        ) {
        val_ *= rhs.val_;
        return *this;
    }

    self_type& operator/=(
        const self_type& rhs
        ) {
        val_ /= rhs.val_;
        return *this;
    }

    self_type& operator%=(
        const self_type& rhs
        ) {
        val_ %= rhs.val_;
        return *this;
    }

    //! @}

public :

    //! \name Self assign bitwise operators
    //! @{

    self_type& operator^=(
        const self_type& rhs
        ) {
        val_ ^= rhs.val_;
        return *this;
    }

    self_type& operator&=(
        const self_type& rhs
        ) {
        val_ &= rhs.val_;
        return *this;
    }

    self_type& operator|=(
        const self_type& rhs
        ) {
        val_ |= rhs.val_;
        return *this;
    }

    self_type& operator<<=(
        const self_type& rhs
        ) {
        val_ <<= rhs.val_;
        return *this;
    }

    self_type& operator>>=(
        const self_type& rhs
        ) {
        val_ >>= rhs.val_;
        return *this;
    }

    //! @}

private :
    T   val_; //! Stores the base type value.

private :
    //! \name Disabled operations
    //! @{

    self_type& operator=(
        const_param_type
        );

    //! @{
};

//! \name Equality/inequality operators
//! @{

template<typename T, typename U>
inline v8_bool_t operator==(
    const true_typedef<T, U>& lhs,
    const true_typedef<T, U>& rhs
    ) {
    return lhs.base_type_value() == rhs.base_type_value();
}

template<typename T, typename U>
inline v8_bool_t operator!=(
    const true_typedef<T, U>& lhs,
    const true_typedef<T, U>& rhs
    ) {
    return !(lhs == rhs);
}

template<typename T, typename U>
inline v8_bool_t operator<(
    const true_typedef<T, U>& lhs,
    const true_typedef<T, U>& rhs
    ) {
    return lhs.base_type_value() < rhs.base_type_value();
}

template<typename T, typename U>
inline v8_bool_t operator>(
    const true_typedef<T, U>& lhs,
    const true_typedef<T, U>& rhs
    ) {
    return lhs.base_type_value() > rhs.base_type_value();
}

template<typename T, typename U>
inline v8_bool_t operator<=(
    const true_typedef<T, U>& lhs,
    const true_typedef<T, U>& rhs
    ) {
    return !(lhs > rhs);
}

template<typename T, typename U>
inline v8_bool_t operator>=(
    const true_typedef<T, U>& lhs,
    const true_typedef<T, U>& rhs
    ) {
    return !(lhs < rhs);
}

//! @}

//! \name Arithmetic operators
//! @{

//!
//! \brief Unary minus.
template<typename T, typename U>
inline true_typedef<T, U> operator-(
    const true_typedef<T, U>& lhs
    ) {
    return true_typedef<T, U>(-lhs.base_type_value());
}

//!
//! \brief Binary plus.
template<typename T, typename U>
inline true_typedef<T, U> operator+(
    const true_typedef<T, U>& lhs,
    const true_typedef<T, U>& rhs
    ) {
    true_typedef<T, U> result(lhs);
    result += rhs;
    return result;
}

//!
//! \brief Binary minus.
template<typename T, typename U>
inline true_typedef<T, U> operator-(
    const true_typedef<T, U>& lhs,
    const true_typedef<T, U>& rhs
    ) {
    true_typedef<T, U> result(lhs);
    result -= rhs;
    return result;
}

template<typename T, typename U>
inline true_typedef<T, U> operator*(
    const true_typedef<T, U>& lhs,
    const true_typedef<T, U>& rhs
    ) {
    true_typedef<T, U> result(lhs);
    result *= rhs;
    return result;
}

template<typename T, typename U>
inline true_typedef<T, U> operator/(
    const true_typedef<T, U>& lhs,
    const true_typedef<T, U>& rhs
    ) {
    true_typedef<T, U> result(lhs);
    result /= rhs;
    return result;
}

template<typename T, typename U>
inline true_typedef<T, U> operator%(
    const true_typedef<T, U>& lhs,
    const true_typedef<T, U>& rhs
    ) {
    true_typedef<T, U> result(lhs);
    result %= rhs;
    return result;
}

//! @}

//! \name Bitwise operators
//! @{

template<typename T, typename U>
inline true_typedef<T, U> operator^(
    const true_typedef<T, U>& lhs,
    const true_typedef<T, U>& rhs
    ) {
    true_typedef<T, U> result(lhs);
    result ^= rhs;
    return result;
}

template<typename T, typename U>
inline true_typedef<T, U> operator&(
    const true_typedef<T, U>& lhs,
    const true_typedef<T, U>& rhs
    ) {
    true_typedef<T, U> result(lhs);
    result &= rhs;
    return result;
}

template<typename T, typename U>
inline true_typedef<T, U> operator|(
    const true_typedef<T, U>& lhs,
    const true_typedef<T, U>& rhs
    ) {
    true_typedef<T, U> result(lhs);
    result |= rhs;
    return result;
}

template<typename T, typename U>
inline true_typedef<T, U> operator~(
    const true_typedef<T, U>& lhs
    ) {
    return true_typedef<T, U>(~lhs.base_type_value());
}

template<typename T, typename U>
inline true_typedef<T, U> operator!(
    const true_typedef<T, U>& lhs
    ) {
    return true_typedef<T, U>(!lhs.base_type_value());
}

template<typename T, typename U>
inline true_typedef<T, U> operator<<(
    const true_typedef<T, U>& lhs,
    const true_typedef<T, U>& rhs
    ) {
    true_typedef<T, U> result(lhs);
    result <<= rhs;
    return result;
}

template<typename T, typename U>
inline true_typedef<T, U> operator>>(
    const true_typedef<T, U>& lhs,
    const true_typedef<T, U>& rhs
    ) {
    true_typedef<T, U> result(lhs);
    result >>= rhs;
    return result;
}

//! @}

} // namespace base
} // namespace v8

