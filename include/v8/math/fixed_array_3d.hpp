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

#include <algorithm>
#include <cassert>

#include <v8/v8.hpp>
#include <v8/math/out_of_bounds_array_access_policy.hpp>

namespace v8 { namespace math {

/** \addtogroup Algebra
 */

/**
 * @{
 */

/*!
 * \brief   A fixed size tridimensional array. It is parameterized by
 *          the element type, the number of size in the X direction,
 *          number of size in the Y direction and Z direction
 *          and an out of bounds access policy can be specified by the user.
 *                  /   Z
 *                 /
 *                /
 *               +----------- X
 *               |
 *               |
 *               | Y
 *          The array's memory layout is <b>row major ordering</b>.
 *          Given element with coordinates (x, y, z), its memory location
 *          is y * X * Z + x * Z + z. The out of bounds access policy
 *          determines what happens when an out of bounds element is accessed.
 *          For example, when specifying a clamping policy, any oob access
 *          will return the last element of the array.
 *          Element access uses <b>0 based indexing</b>.
 * \c {
 *  fixed_array_3D<int, 4, 4, 4, oob_clamp_policy> arr;
 *  ...
 *  //
 *  // With the oob_clamp_policy
 *  // trying to access element at (5, 4, 5) which is out of bounds
 *  // will allways return the last element (3, 3, 3)
 *  elem = arr(5, 4, 5);
 *  fixed_array_3D<int, 4, 4, 4, oob_wrap_policy>  arr2;
 *  //
 *  // With the oob_wrap_policy, any out of bound indices
 *  // will return an element that is within the array's limits
 *  // (row % num_rows, col % num_cols)
 *  elem = arr(5, 5, 6); // returns element arr(2, 2, 0)
 *  }
 */
template
<
    typename T,
    v8_size_t X = 4u,
    v8_size_t Y = 4u,
    v8_size_t Z = 4u,
    typename out_of_bounds_access_policy = oob_assert_policy
> class fixed_array_3D {
public :

    //! \name Defined types.
    //! @{

    //! Type of a stored element.
    typedef T                                       value_type;

    //! Type of an iterator.
    typedef T*                                      iterator;

    //! Type of a const iterator.
    typedef const T*                                const_iterator;

    //! Type of a reverse iterator.
    typedef std::reverse_iterator<iterator>         reverse_iterator;

    //! Type of a const reverse iterator.
    typedef std::reverse_iterator<const_iterator>   const_reverse_iterator;

    //! Type of reference to an element.
    typedef T&                                      reference;

    //! Type of a const reference to an element.
    typedef const T&                                const_reference;

    typedef v8_size_t                               size_type;

    typedef ptrdiff_t                               difference_type;

    //! @}

public :

    //! \name Construction.
    //! @{

    fixed_array_3D() {}

    fixed_array_3D(T const& value) {
        std::fill_n(data_, size(), value);
    }

    ~fixed_array_3D() {}

    //! @}

public :

    //! \name Element r/w access.
    //! @{

    reference operator()(v8_size_t x, v8_size_t y, v8_size_t z) {
        return get_at(oob_policy_t::at(x, X - 1),
                      oob_policy_t::at(y, Y - 1),
                      oob_policy_t::at(z, Z - 1));
    }

    const_reference operator()(v8_size_t x, v8_size_t y, v8_size_t z) const {
        return get_at(oob_policy_t::at(x, X - 1),
                      oob_policy_t::at(y, Y - 1),
                      oob_policy_t::at(z, Z - 1));
    }

    T* as_array() NOEXCEPT {
        return data_;
    }

    const T* as_array() const NOEXCEPT {
        return data_;
    }

    //! @}

public :

    //! \name Attributes.
    //! @{

    v8_size_t size() const NOEXCEPT {
        return X * Y * Z;
    }

    v8_size_t dimension_x() const NOEXCEPT {
        return X;
    }

    v8_size_t dimension_y() const NOEXCEPT {
        return Y;
    }

    v8_size_t dimension_z() const NOEXCEPT {
        return Z;
    }

    //! @}

public :

    //! \name Iteration.
    //! @}

    iterator begin() NOEXCEPT {
        return data_;
    }

    iterator end() NOEXCEPT {
        return data_ + size();
    }

    const_iterator begin() const NOEXCEPT {
        return data_;
    }

    const_iterator end() const NOEXCEPT {
        return data_ + size();
    }

    reverse_iterator rbegin() NOEXCEPT {
        return reverse_iterator(begin());
    }

    reverse_iterator rend() NOEXCEPT {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const NOEXCEPT {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator rend() const NOEXCEPT {
        return const_reverse_iterator(end());
    }

    //! @}

private :
    typedef out_of_bounds_access_policy oob_policy_t;

    //! \name Internal accessor functions.
    //! @{

    reference get_at(v8_size_t x, v8_size_t y, v8_size_t z) {
        const v8_size_t e_location = X * Y * z + X * y + x;
        assert(e_location < size());
        return data_[e_location];
    }

    const_reference get_at(v8_size_t x, v8_size_t y, v8_size_t z) const {
        const v8_size_t e_location = X * Y * z + X * y + x;
        assert(e_location < size());
        return data_[e_location];
    }

    //! @}

private :

    T   data_[X * Y * Z];
};

/** @} */

} // namespace math
} // namespace v8
