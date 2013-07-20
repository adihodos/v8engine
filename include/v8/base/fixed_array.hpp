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

#include <cassert>
#include <iterator>
#include <utility>

#include <v8/v8.hpp>

namespace v8 { namespace base {

/// \addtogroup __grp_v8_base
/// @{

///
/// \brief A fixed size array class, that mimics built-in arrays.
/// \code
///     fixed_array<int, 8U> array { 0, 1, 2, 3, 4, 5, 6, 7 };
///     for (int i = 0; i < array.size(); ++i) {
///         array[i] *= 2;
///     }
///
///     for (const auto& val : array) {
///         printf("\n%d", val);
///     }
/// \endcode    
template
<
    typename T,
    v8_size_t k_dimension = 4U
>
class fixed_array {

/// \name Defined types.
/// @{

public :

    ///< Type of element stored in the array.
    typedef T                                           value_type;

    ///< Type of reference to an element.
    typedef T&                                          reference;

    ///< Type of reference to a const element.
    typedef const T&                                    const_reference;

    ///< Type of pointer to an array element.
    typedef T*                                          pointer;

    ///< Type of pointer to a const array element.
    typedef const T*                                    const_pointer;

    ///< Type of iterator.
    typedef T*                                          iterator;

    ///< Type of const iterator.
    typedef const T*                                    const_iterator;

    ///< Type of reverse iterator.
    typedef std::reverse_iterator<iterator>             reverse_iterator;

    ///< Type of const reverse iterator.
    typedef std::reverse_iterator<const_iterator>       const_reverse_iterator;

    ///< Type of result obtained by substracting two iterators.
    typedef ptrdiff_t                                   difference_type;

    ///< Type for capacity.
    typedef v8_size_t                                   size_type;

    ///< Fully qualified type of this class.
    typedef fixed_array<T, k_dimension>                 class_type;

/// @}

/// \name Constructors.
/// @{

public :

    fixed_array() {}

/// @}

/// \name Operators.
/// @{

public :

    reference operator[](const v8_size_t idx) NOEXCEPT {
        assert(idx < k_dimension);
        return array_data_[idx];
    }

    const_reference operator[](const v8_size_t idx) const NOEXCEPT {
        assert(idx < k_dimension);
        return array_data_[idx];
    }

/// @}

/// \name Attributes.
/// @{

public :

    CONSTEXPR v8_size_t size() const NOEXCEPT {
        return k_dimension;
    }

    pointer data() NOEXCEPT {
        return &array_data_[0];
    }

    const_pointer data() const NOEXCEPT {
        return &array_data_[0];
    }

/// @}

/// \name Element access.
/// @{

public :

    reference front() NOEXCEPT {
        return array_data_[0];
    }

    const_reference front() const NOEXCEPT {
        return array_data_[0];
    }

    reference back() NOEXCEPT {
        return array_data_[k_dimension - 1];
    }

    const_reference back() const NOEXCEPT {
        return array_data_[k_dimension - 1];
    }

/// @}

/// \name Iteration.
/// @{

public :

    iterator begin() NOEXCEPT {
        return &array_data_[0];
    }

    const_iterator begin() const NOEXCEPT {
        return &array_data_[0];
    }

    iterator end() NOEXCEPT {
        return &array_data_[size()];
    }

    const_iterator end() const NOEXCEPT {
        return &array_data_[size()];
    }

    reverse_iterator rbegin() NOEXCEPT {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rbegin() const NOEXCEPT {
        return const_reverse_iterator(begin());
    }

    reverse_iterator rend() NOEXCEPT {
        return reverse_iterator(end());
    }

    const_reverse_iterator rend() const NOEXCEPT {
        return const_reverse_iterator(end());
    }

/// @}

/// \name Data members.
/// @{

private :
    T               array_data_[k_dimension];

    NO_CC_ASSIGN(fixed_array);

/// @}
};

/// @}

} // namespace base
} // namespace v8
