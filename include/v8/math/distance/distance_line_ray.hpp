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
#include <v8/math/objects/ray.hpp>
#include <v8/math/distance/distance_line_line.hpp>
#include <v8/math/distance/distance_point_line.hpp>

namespace v8 { namespace math {

/** \addtogroup Distance
 *  @{
 */

/**
 * \brief Computes the square of the distance between a line and a ray.
 * \param[in]   ln  The line.
 * \param[in]   ry  The ray.
 */
template<typename real_t, int space_dim>
real_t squared_distance_line_ray(
        const line<real_t, space_dim>& ln,
        const ray<real_t, space_dim>& ry,
        real_t* closest0,
        real_t* closest1) {
    //
    // convert the ray to line
    line<real_t, space_dim> new_line(ry.origin_, ry.direction_);
    const real_t squared_dist = squared_distance_line_line(
                ln, new_line, closest0, closest1);
    //
    // Clamp to [0, +inf)
    if ((*closest1) < real_t(0)) {
        *closest1 = real_t(0);
        squared_dist = squared_distance_point_to_line(ry.origin_, ln, closest0);
    }
    return squared_dist;
}

/** @} */

} // namespace math
} // namespace v8
