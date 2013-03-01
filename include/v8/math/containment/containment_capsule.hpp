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
#include <v8/math/space_traits.hpp>
#include <v8/math/objects/capsule.hpp>
#include <v8/math/objects/line.hpp>
#include <v8/math/objects/segment.hpp>
#include <v8/math/distance/distance_point_segment.hpp>

namespace v8 { namespace math {

/** \addtogroup Containment
 *  @{
 */

/**
 * \brief   Test if a point is contained in a capsule. A point P is inside
 *          a capsule C with segment S and radius R if the distance from
 *          the point P to the segment S is less or equal to R.
 */
template<typename real_t, int space_dim>
inline bool contains_object(
    const capsule<real_t, space_dim>& cap,
    const typename space_traits<real_t, space_dim>::vector_t& pt) {
    real_t t_value;
    const real_t dist_squared = squared_distance(pt, cap.cap_segment_, &t_value);
    return operands_le(dist_squared, cap.cap_radius_ * cap.cap_radius_);
}

/**
 * \brief   Test if a capsule is contained in another capsule.
 * \param cap_a Reference to a capsule that will be tests against.
 * \param cap_b Capsule to test.
 * \return True if cap_a contains cap_b, false otherwise.
 */
template<typename real_t>
bool contains_object(
    const capsule<real_t, Space_Tridimensional>& cap_a,
    const capsule<real_t, Space_Tridimensional>& cap_b
    ) {
    //
    // Convert the capsule's end points to spheres
    const sphere<real_t> sph_left(cap_b.cap_segment_.get_start_point(),
                                  cap_b.cap_radius_);
    const sphere<real_t> sph_right(cap_b.cap_segment_.get_end_point(),
                                   cap_b.cap_radius_);

    //
    // Capsule A contains capsule B if it contains the spheres built using
    // capsule's B segment endpoints and radius.
    return contains_object(cap_a, sph_left)
            && contains_object(cap_a, sph_right);
}

/*!
 * \brief   Computes the union of two capsules. Given two capsules A and B,
 *          this function will compute the smallest capsule C such that
 *          it contains both capsule A and capsule B.
 * \param capsule_a Reference to the first capsule.
 * \param capsule_b Reference to the second capsule.
 * \param capsule_union Pointer to a capsule object that will receive the data
 *                      for a capsule containing both capsule_a and capsule_b.
 * \remarks See David H. Eberly, Modern 3D engine design, p. 629-630 for the
 *          capsule union algorithm.
 */
template<typename real_t, int space_dim>
void merge_capsules(const capsule<real_t, space_dim>& capsule_a,
                    const capsule<real_t, space_dim>& capsule_b,
                    capsule<real_t, space_dim>* capsule_union) {
    typedef typename space_traits<real_t, space_dim>::vector_t  vector_t;
    typedef line<real_t, space_dim>                             line_t;
    typedef segment<real_t, space_dim>                          segment_t;

    //
    // Test for the trivial case where either of A and B contains the other.
    if (contains_object(capsule_a, capsule_b)) {
        memcpy(capsule_union, &capsule_a, sizeof(capsule_a));
        return;
    }

    if (contains_object(capsule_b, capsule_a)) {
        memcpy(capsule_union, &capsule_b, sizeof(capsule_b));
        return;
    }

    const vector_t& D0 = capsule_a.cap_segment_.direction_;
    const vector_t& D1 = capsule_b.cap_segment_.direction_;

    const vector_t& cap0_C = capsule_a.cap_segment_.centre_;
    const vector_t& cap1_C = capsule_b.cap_segment_.centre_;

    //
    // Compute the line that contains the segment of the union capsule.
    // The direction vector is (D0 + D1) / ||D0 + D1||.
    // The origin point of the line is C = (C0.center() + C1.center()) / 2.
    line_t capsule_line;
    capsule_line.origin_ = (cap0_C + cap1_C) / 2;
    if (dot_product(D0, D1) < real_t(0)) {
        capsule_line.direction_ = normal_of(D0 - D1);
    } else {
        capsule_line.direction_ = normal_of(D0 + D1);
    }

    //
    // The radius of the union capsule must be large enough that it contains
    // both capsules. It is enough to consider the circle/sphere endpoints
    // of the capsules. Compute segment start and endpoints for the two capsules
    // and select the radius for the union capsule to be the maximum
    // of the sum between the distance from a capsule's endpoint to the line L
    // and its radius.

    //
    // Let P00, P01, P10, P11 be the start/end endpoints for the segments
    // of capsules C0 and C1.
    const vector_t cap_seg_points[4] = {
        //
        // P00
        capsule_a.cap_segment_.get_start_point(),
        //
        // P01
        capsule_a.cap_segment_.get_end_point(),
        //
        // P10
        capsule_b.cap_segment_.get_start_point(),
        //
        // P11
        capsule_b.cap_segment_.get_end_point()
    };

    const real_t cap_rad[4] = {
        capsule_a.cap_radius_, capsule_a.cap_radius_,
        capsule_b.cap_radius_, capsule_b.cap_radius_
    };

    real_t final_capsule_radius = real_t(0);
    for (size_t i = 0; i < 4; ++i) {
        const real_t dist = distance_squared(cap_seg_points[i], capsule_line)
                            + cap_rad[i];
        if (dist > final_capsule_radius)
            final_capsule_radius = dist;
    }

    //
    // Sphere centered at capsule 0 segment end point, radius of capsule 0
    vector_t delta = capsule_line.origin_ - cap_seg_points[1];
    real_t radius_diff = final_capsule_radius - capsule_a.cap_radius_;
    real_t radius_diff_sqrd = radius_diff * radius_diff;
    real_t k0 = delta.length_squared() - radius_diff_sqrd;
    real_t k1 = dot_product(delta, capsule_line.direction_);
    real_t eq_delta = k1 * k1 - k0;
    real_t sqr_delta = sqrt(abs(eq_delta));

    real_t t_positive = -k1 - sqr_delta;
    real_t t_negative = -k1 + sqr_delta;

    //
    // Sphere centered at capsule 0 segment start point, radius of capsule 0
    delta = capsule_line.origin_ - cap_seg_points[0];
    k0 = delta.length_squared() - radius_diff_sqrd;
    k1 = dot_product(delta, capsule_line.direction_);
    eq_delta = k1 * k1 - k0;
    sqr_delta = sqrt(abs(eq_delta));

    real_t tmp = -k1 - sqr_delta;
    if (tmp > t_positive)
        t_positive = tmp;

    tmp = -k1 + sqr_delta;
    if (tmp < t_negative)
        t_negative = tmp;

    //
    // Sphere centered at capsule 1 segment end point, radius of capsule 1
    delta = capsule_line.origin_ - cap_seg_points[3];
    radius_diff = final_capsule_radius - capsule_b.cap_radius_;
    radius_diff_sqrd = radius_diff * radius_diff;
    k0 = delta.length_squared() - radius_diff_sqrd;
    k1 = dot_product(delta, capsule_line.direction_);
    eq_delta = k1 * k1 - k0;
    sqr_delta = sqrt(abs(eq_delta));

    tmp = -k1 - sqr_delta;
    if (tmp > t_positive)
        t_positive = tmp;

    tmp = -k1 + sqr_delta;
    if (tmp < t_negative)
        t_negative = tmp;

    //
    // Sphere centered at capsule 1 segment start point, radius of capsule 1
    delta = capsule_line.origin_ - cap_seg_points[2];
    k0 = delta.length_squared() - radius_diff_sqrd;
    k1 = dot_product(delta, capsule_line.direction_);
    eq_delta = k1 * k1 - k0;
    sqr_delta = sqrt(abs(eq_delta));

    tmp = -k1 - sqr_delta;
    if (tmp > t_positive)
        t_positive = tmp;

    tmp = -k1 + sqr_delta;
    if (tmp < t_negative)
        t_negative = tmp;

    capsule_union->cap_radius_ = final_capsule_radius;
    capsule_union->cap_segment_.centre_ =
            real_t(0.5) * (t_positive + t_negative) * capsule_line.origin_;
    capsule_union->cap_segment_.direction_ = capsule_line.direction_;

    if (t_positive > t_negative) {
        capsule_union->cap_segment_.extent_ =
                real_t(0.5) * (t_positive - t_negative);
    } else {
        capsule_union->cap_segment_.extent_ = real_t(0);
    }
}

/** @} */

} // namespace math
} // namespace v8
