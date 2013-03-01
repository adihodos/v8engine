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

#include <v8/math/tridimensional_space_traits.hpp>
#include <v8/math/objects/axis_aligned_bounding_box.hpp>
#include <v8/math/objects/plane.hpp>
#include <v8/math/math_utils.hpp>
#include <v8/math/vector3.hpp>

namespace v8 { namespace math {

/** \defgroup   Culling Culling methods.
 *
 */

/** \addtogroup Culling
 *  @{
 */

/**
 * \brief Cull an axis aligned bounding box with a plane.
 * \param Culling plane.
 * \param AABB to cull.
 * \return True if the AABB can be culled.
 * \remarks The AABB is culled if (Pmin, Pmax) are in the negative side of the
 * 			plane.
 * \see Real Time Rendering, second edition.
 */
template<typename real_t>
inline bool cull_object(
    const plane<real_t>& P,
    const axis_aligned_bounding_box<real_t, Space_Tridimensional>& aabb) {

    typedef plane<real_t> plane_t;
    typedef vector3<real_t> vector3_t;
    vector3_t vmin;
    vector3_t vmax;

    //
    // Find the min and max points of the aabb that are most aligned with the
    // plane's normal.
    if (operands_ge(P.normal_.x_, real_t(0))) {
        vmin.x_ = aabb.min_point_.x_;
        vmax.x_ = aabb.max_point_.x_;
    } else {
        vmin.x_ = aabb.max_point_.x_;
        vmax.x_ = aabb.min_point_.x_;
    }

    if (operands_ge(P.normal_.y_, real_t(0))) {
        vmin.y_ = aabb.min_point_.y_;
        vmax.y_ = aabb.max_point_.y_;
    } else {
        vmin.y_ = aabb.max_point_.y_;
        vmax.y_ = aabb.min_point_.y_;
    }

    if (operands_ge(P.normal_.z_, real_t(0))) {
        vmin.z_ = aabb.min_point_.z_;
        vmax.z_ = aabb.max_point_.z_;
    } else {
        vmin.z_ = aabb.max_point_.z_;
        vmax.z_ = aabb.min_point_.z_;
    }

    //
    // Cull if vmax is on the plane's negative side.
    return P.which_side(vmax) == plane_t::Plane_Negative_Side;
}

/** @} */

} // namespace math
} // namespace v8
