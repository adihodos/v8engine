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
#include <v8/math/objects/ray.hpp>
#include <v8/math/distance/distance_line_ray.hpp>
#include <v8/math/distance/distance_point_ray.hpp>

namespace v8 { namespace math {

/** \addtogroup Distance
 *  @{
 */

template<typename real_t, typename space_dim>
real_t squared_distance(
        const ray<real_t, space_dim>& r0,
        const ray<real_t, space_dim>& r1,
        real_t* r0_closest,
        real_t* r1_closest) {
    //
    // Convert ray to line
    line<real_t, space_dim> r_line(r0.origin_, r0.direction_);
    real_t squared_dst = squared_distance_line_ray(r_line, r1, r0_closest,
                                                   r1_closest);
    //
    // Clamp to [0, +inf)
    if (*r0_closest < real_t(0)) {
        *r0_closest = real_t(0);
        squared_dst = squared_distance_point_ray(r0.origin_, r1, r1_closest);
    }
    return squared_dst;
}

/** @} */

} // namespace math
} // namespace v8
