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

/**
 * \file    containment_capsule3.hpp
 * \brief   Containment methods specific for capsules in R3.
 */
#pragma once

#include <v8/math/math_utils.hpp>
#include <v8/math/tridimensional_space_traits.hpp>
#include <v8/math/objects/capsule3.hpp>
#include <v8/math/objects/segment3.hpp>
#include <v8/math/objects/sphere.hpp>
#include <v8/math/distance/distance_point_segment.hpp>

namespace v8 { namespace math {

/** \addtogroup Containment
 *  @{
 */

/**
 * \brief   Test if a sphere is contained in a capsule. A sphere is contained in a
 *          capsule if the sphere's radius is less than or equal to the
 *          capsule's radius, and the center of the sphere is inside the capsule.
 * \param cap   Reference to a capsule object.
 * \param sph   Reference to a sphere object.
 * \return True if capsule contains sphere, false if not.
 */
template<typename real_t>
bool contains_object(
    const capsule<real_t, Space_Tridimensional>& cap,
    const sphere<real_t>& sph
    ) {
    //
    // Compute difference between the sphere's radius and the capsule's radius.
    // Trivially reject if the sphere's radius is greater than that of the
    // capsule (difference < 0).
    const real_t radius_diff = cap.cap_radius_ - sph.radius_;

    //
    // Test the distance from the sphere's center to the capsule's segment.
    if (operands_ge(radius_diff, real_t(0))) {
        real_t t_value;
        return operands_le(squared_distance(sph.center_, cap.cap_segment_,
                                            &t_value),
                           radius_diff * radius_diff);
    }
    return false;
}

/** @} */

} // namespace math
} // namespace v8
