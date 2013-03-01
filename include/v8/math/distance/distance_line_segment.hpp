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
#include <v8/math/objects/line.hpp>
#include <v8/math/objects/segment.hpp>
#include <v8/math/distance/distance_line_line.hpp>
#include <v8/math/distance/distance_point_line.hpp>

namespace v8 { namespace math {

/** \addtogroup Distance
 *  @{
 */

/**
 * \brief Computes the square of the distance between a line and a segment.
 * \param[in]   ln  The line.
 * \param[in]   seg The segment.
 */
template<typename real_t, int space_dim>
real_t squared_distance(
        const line<real_t, space_dim>& ln,
        const segment<real_t, space_dim>& seg,
        real_t* l_closest,
        real_t* s_closest) {
    //
    // Convert segment to line
    line<real_t, space_dim> s_line(seg.centre_, seg.direction_);
    real_t squared_dist = squared_distance_line_line(
                ln, s_line, l_closest, s_closest);

    typename space_traits<real_t, space_dim>::vector_t endp;
    //
    // Clamp to [-e, e] interval
    if (*s_closest < -seg.extent_) {
        *s_closest = -seg.extent_;
        endp = seg.get_start_point();
        squared_dist = squared_distance_point_to_line(endp, ln, l_closest);
    } else if (*s_closest > seg.extent_) {
        *s_closest = seg.extent_;
        endp = seg.get_end_point();
        squared_dist = squared_distance_point_to_line(endp, ln, l_closest);
    }
    return squared_dist;
}

/** @} */

} // namespace math
} // namespace v8
