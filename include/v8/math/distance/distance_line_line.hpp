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

#include <v8/math/objects/line.hpp>
#include <v8/math/math_utils.hpp>
#include <v8/math/math_constants.hpp>

namespace v8 { namespace math {

/** \addtogroup Distance
 *  @{
 */

//
// TODO : write explanation about the algorithm.

/**
 * \brief Returns the square of the distance between to lines.
 * \param[in]   line0   The first line.
 * \param[in]   line1   The second line.
 */
template<typename real_t, int space_dim>
real_t squared_distance_line_line(
        const line<real_t, space_dim>& line0,
        const line<real_t, space_dim>& line1,
        real_t* closest0,
        real_t* closest1) {
    typedef typename space_traits<real_t, space_dim>::vector_t vector_t;

    real_t squared_dist;

    vector_t diff = line0.origin_ - line1.origin_;
    const real_t a01 = -dot_product(line0.direction_, line1.direction_);
    const real_t b0 = dot_product(line0.direction_, diff);
    const real_t c = diff.length_squared();
    const real_t det = real_t(1) - a01 * a01;

    if (operands_eq(abs(det), constants::kEpsilon)
        || (abs(det) > constants::kEpsilon)) {
        //
        // Lines are not parallel.
        const real_t b1 = -dot_product(line1.direction_, diff);
        typename internals::div_wrap_t<real_t>::div_helper_t div_hlp(det);
        *closest0 = div_hlp(a01 * b1 - b0);
        *closest1 = div_hlp(a01 * b0 - b1);
        squared_dist = (*closest0) * ((*closest0) + a01 * (*closest1) + 2 * b0)
                + (*closest1) * (a01 * (*closest0) + (*closest1) + 2 * b1) + c;
    } else {
        //
        // Parallel lines. Any pair of points will do.
        *closest0 = -b0;
        *closest1 = real_t(0);
        squared_dist = b0 * (*closest0) + c;
    }

    return abs(squared_dist);
}

/** @} */

} // namespace math
} // namespace v8
