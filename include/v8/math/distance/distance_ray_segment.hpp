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

#include <v8/math/objects/ray.hpp>
#include <v8/math/objects/segment.hpp>
#include <v8/math/distance/distance_line_segment.hpp>
#include <v8/math/distance/distance_point_segment.hpp>

namespace v8 { namespace math {

/** \addtogroup Distance
 *  @{
 */

template<typename real_t, int space_dim>
real_t squared_distance(
        const ray<real_t, space_dim>& ry,
        const segment<real_t, space_dim>& seg,
        real_t* r_closest,
        real_t* s_closest) {
    //
    // Convert ray to line
    line<real_t, space_dim> r_line(ry.direction_, ry.origin_);
    real_t squared_dst = squared_distance(r_line, seg, r_closest, s_closest);

    if (*r_closest < real_t(0)) {
        *r_closest = real_t(0);
        squared_dst = squared_distance_point_segment(ry.origin_, seg, s_closest);
    }
    return squared_dst;
}

/** @} */

} // namespace math
} // namespace v8
