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

/*!
 * \file    Containment methods specific for capsules in R2.
 */
#pragma once

#include <v8/math/math_utils.hpp>
#include <v8/math/bidimensional_space_traits.hpp>
#include <v8/math/objects/capsule2.hpp>
#include <v8/math/objects/segment2.hpp>
#include <v8/math/objects/circle2.hpp>
#include <v8/math/distance/distance_point_segment.hpp>

namespace v8 { namespace math {

/** \addtogroup Containment
 *  @{
 */

/*!
 * \brief Test if a circle is contained in a capsule.
 * \param cap Reference to a capsule2 object.
 * \param circle Reference to a circle2 object.
 * \return True if circle is contained in capsule, false otherwise.
 * \remarks A circle C with center P and radius R is contained in a capsule
 *          U with segment S and radius T if the distance from the center C to
 *          the segment S is less or at most equal to the radius T and the
 *          circle's radius R is less than or equal to the capsule's radius.
 */
template<typename real_t>
bool contains_object(
    const capsule<real_t, Space_Bidimensional>& cap,
    const circle2<real_t>& circle) {
    //
    // Trivially reject if the capsule's radius is less than the circle's.
    const real_t radius_diff = cap.cap_radius_ - circle.radius_;
    if (operands_ge(radius_diff, real_t(0))) {
        //
        //Circle can fit, see if it is really inside the capsule.
        real_t t_value;
        const real_t dst_squared = squared_distance(
                    circle.center_, cap.cap_segment_, &t_value);
        return operands_le(dst_squared, cap.cap_radius_ * cap.cap_radius_);
    }
    return false;
}

/** @} */

} // namespace math
} // namespace v8
