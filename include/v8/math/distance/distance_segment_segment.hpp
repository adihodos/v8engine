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

#include <v8/math/space_traits.hpp>
#include <v8/math/objects/segment.hpp>
#include <v8/math/distance/distance_line_segment.hpp>
#include <v8/math/distance/distance_point_segment.hpp>

namespace v8 { namespace math {

/** \addtogroup Distance
 *  @{
 */

template<typename real_t, int space_dim>
real_t squared_distance(
        const segment<real_t, space_dim>& s0,
        const segment<real_t, space_dim>& s1,
        real_t* s0_closest,
        real_t* s1_closest) {
    //
    // Convert segment to line
    line<real_t, space_dim> s_line(s0.centre_, s0.direction_);
    real_t square_dst = squared_distance(s_line, s1, s0_closest, s1_closest);

    typename space_traits<real_t, space_dim>::vector_t endp;
    if (*s0_closest < -s0.extent_) {
        *s0_closest = -s0.extent_;
        endp = s0.get_start_point();
        square_dst = squared_distance_point_segment(endp, s1, s1_closest);
    } else if (*s0_closest > s0.extent_) {
        *s0_closest = s0.extent_;
        endp = s0.get_end_point();
        square_dst = squared_distance_point_segment(endp, s1, s1_closest);
    }
    return square_dst;
}

/** @} */

} // namespace math
} // namespace v8
