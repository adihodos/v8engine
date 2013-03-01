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

#include <v8/math/math_utils.hpp>
#include <v8/math/space_traits.hpp>
#include <v8/math/objects/rectangle.hpp>

namespace v8 { namespace math {

/** \addtogroup Distance
 *  @{
 */

/**
 * \brief Computes the square of the distance between a point and a rectangle.
 * \param[in] The point.
 * \param[in] The rectangle.
 * \param[in,out] rclosest Pointer to an array of two values of type real_t.
 */
template<typename real_t, int space_dim>
real_t squared_distance(
        const typename space_traits<real_t, space_dim>::vector_t& pt,
        const rectangle<real_t, space_dim>& rc,
        real_t* rclosest) {
    typedef typename space_traits<real_t, space_dim>::vector_t vector_t;

    vector_t diff = rc.center_ - pt;
    const real_t b0 = dot_product(rc.axes_[0], diff);
    const real_t b1 = dot_product(rc.axes_[1], diff);
    real_t s = -b0;
    real_t t = -b1;

    s = clamp(s, -rc.extents_[0], rc.extents_[0]);
    t = clamp(t, -rc.extents_[1], rc.extents_[1]);
    rclosest[0] = s;
    rclosest[1] = t;

    return diff.square_length() + s * (s + 2 * b0) + t * (t + 2 * b1);
}

/** @} */

} // namespace math
} // namespace v8
