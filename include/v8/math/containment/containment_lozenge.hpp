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
#include <v8/math/vector3.hpp>
#include <v8/math/distance/distance_point_rectangle.hpp>
#include <v8/math/objects/lozenge.hpp>

namespace v8 { namespace math {

/** \addtogroup Containment
 *  @{
 */

/**
 * \brief   Test if a point is inside a lozenge. A point P is inside a lozenge
 *          L with rectangle R and radius Q if the distance from the point P
 *          to the rectangle R is at most equal to the radius Q.
 * \param lz    Reference to a lozenge object.
 * \param pt    Reference to a point.
 */
template<typename real_t>
inline bool contains_object(
    const lozenge<real_t>& lz,
    const vector3<real_t>& pt) {
    real_t closest;
    const real_t sqdst = squared_distance(pt, lz.lz_rectangle_, &closest);
    return operands_le(sqdst, lz.lz_radius_ * lz.lz_radius_);
}

/** @} */

} // namespace math
} // namespace v8
