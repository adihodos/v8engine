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
#include <v8/math/objects/sphere.hpp>

namespace v8 { namespace math {

/** \defgroup   Containment
 */

/** \addtogroup Containment
 *  @{
 */

/**
 * \brief   Test if a point is inside a sphere. A point P is inside a sphere S
 * 			with center C and radius R if |P - C| <= R.
 * \param   sph     The sphere.
 * \param   point   The point.
 */
template<typename real_t>
inline bool 
contains_object(
    const sphere<real_t>& sph,
    const vector3<real_t>& point
    ) {
    const vector3<real_t>& diff(point - sph.center_);
    const real_t len_squared = diff.sum_components_squared();
    const real_t radius_squared = sph.radius_ * sph.radius_;
    //
    // Compare d^2 to r^2 to avoid a square root op.
    return operands_le(len_squared, radius_squared);
}

/**
 * \brief   Compute the bounding sphere for a set of points. The approached used
 * 			is to compute the minimum AABB of the set of points and then
 * 			build a minimum volume sphere that encloses the AABB, with the center
 * 			at the AABB's center.
 * \ingroup Containment methods.
 * \param   pt_set                  Set the point belongs to.
 * \param   pt_count                Number of points.
 * \param   pt_stride               The point stride.
 * \param [in,out]  bounding_sphere If non-null, the bounding sphere.
 */
template<typename real_t>
void sphere_from_points_by_aabb(
    const vector3<real_t>*  pt_set,
    size_t                  pt_count,
    size_t                  pt_stride,
    sphere<real_t>*         bounding_sphere
    ) {
    typedef vector3<real_t> vector3_t;

    vector3_t min = *pt_set;
    vector3_t max = min;

    for (size_t i = 1; i < pt_count; ++i) {
        const vector3_t& current_pt = *(reinterpret_cast<const vector3_t*>(
            reinterpret_cast<const char*>(pt_set) + i * pt_stride));
        for (size_t j = 0; j < 3; ++j) {
            if (current_pt.elements_[j] < min.elements_[j]) {
                min.elements_[j] = current_pt.elements_[j];
            } else if (current_pt.elements_[j] > max.elements_[j]) {
                max.elements_[j] = current_pt.elements_[j];
            }
        }
    }

    bounding_sphere->center_ = real_t(0.5) * (max + min);
    bounding_sphere->radius_ = (real_t(0.5) * (max - min)).magnitude();
}

/**
 * \brief   Compute a bounding sphere for a set of points. This will result in
 * 			a sphere with the center at the average of points and a radius
 * 			that represents the smallest value for which the sphere encloses
 * 			the points.
 * \ingroup Containment methods.
 * \param   pt_set                  Set of points.
 * \param   pt_count                Number of points in the set.
 * \param   pt_stride               The point stride (number of bytes to skip 
 * 									in order to get the next point).
 * \param [in,out]  bounding_sphere Pointer to a sphere object. Must not be null.
 */
template<typename real_t>
void sphere_from_points_by_average(
    const vector3<real_t>*  pt_set,
    size_t                  pt_count,
    size_t                  pt_stride,
    sphere<real_t>*         bounding_sphere
    ) {
    typedef vector3<real_t> vector3_t;

    vector3_t sum(vector3_t::zero);
    for (size_t i = 0; i < pt_count; ++i) {
        sum += *(reinterpret_cast<const vector3_t*>(
            reinterpret_cast<const char*>(pt_set) + i * pt_stride));
    }

    bounding_sphere->center_ = sum / pt_count;

    real_t radius_squared = real_t(0);
    for (size_t i = 0; i < pt_count; ++i) {
        const vector3_t& diff = *(reinterpret_cast<const vector3_t*>(
            reinterpret_cast<const char*>(pt_set) + i * pt_stride)) 
            - bounding_sphere->center_;
        real_t dst_squared = diff.sum_components_squared();
        if (dst_squared > radius_squared)
            radius_squared = dst_squared;
    }

    bounding_sphere->radius_ = sqrt(radius_squared);
}

/**
 * \brief   Merges two spheres.
 * \param   s0              Reference to the first sphere.
 * \param   s1              Reference to the second sphere
 * \param [in,out]  s_union Pointer to a sphere object, representing the union
 * 					os S0 with S1.
 */
template<typename real_t>
void merge_spheres(
    const sphere<real_t>&   s0,
    const sphere<real_t>&   s1,
    sphere<real_t>*         s_union
    ) {
    typedef vector3<real_t> vector3_t;

    const vector3_t center_diff = s1.center_ - s0.center_;
    const real_t radius_diff = s1.radius_ - s0.radius_;
    //
    // Square radius to avoid a square root op.
    const real_t radius_diff_sqrd = radius_diff * radius_diff;
    //
    // Square diff vector length to avoid a square root op.
    const real_t length_diff_sqrd = center_diff.length_squared();

    if ((radius_diff_sqrd > length_diff_sqrd) ||
        operands_eq(radius_diff_sqrd, length_diff_sqrd)) {
        if (operands_ge(radius_diff_sqrd, real_t(0))) {
            s_union->center_ = s1.center_;
            s_union->radius_ = s1.radius_;
        } else {
            s_union->center_ = s0.center_;
            s_union->radius_ = s0.radius_;
        }
    } else {
        const real_t length_diff = sqrt(length_diff_sqrd);
        const real_t t = (length_diff + s1.radius_ - s0.radius_) 
                         / (real_t(2) * length_diff);
        s_union->center_ = s0.center_ + t * center_diff;
        s_union->radius_ = (length_diff + s1.radius_ + s0.radius_) / real_t(2);
    }
}

/** @} */

} // namespace math
} // namespace v8
