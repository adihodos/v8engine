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
#include <v8/math/tridimensional_space_traits.hpp>
#include <v8/math/objects/line.hpp>
#include <v8/math/objects/segment.hpp>
#include <v8/math/objects/triangle.hpp>
#include <v8/math/vector3.hpp>

namespace v8 { namespace math {

/** \addtogroup Distance
 *  @{
 */

/**
 * \brief   Computes the square of the distance between a line and a triangle.
 * \param   ln                      The line.
 * \param   tri                     The triangle.
 * \param [in,out]  ln_closest      Pointer to a value that receives the
 * 					t-value for the point on the line that is closest to the
 * 					triangle. Must not be null.
 * \param [in,out]  p_barycentric   Pointer to an array of three values. These
 * 					are the barycentric coordinates of the triangle point that 
 * 					is closest to the line. Must not be null.
 * \see David Eberly, 3D game engne design, pages 651-654 for a description of
 * 		the algorithm.
 */
template<typename real_t>
real_t squared_distance(
    const line<real_t, Space_Tridimensional>& ln,
    const triangle<real_t, Space_Tridimensional>& tri,
    real_t* ln_closest,
    real_t* p_barycentric
    ) {
    typedef vector3<real_t> vector3_t;

    const vector3_t e0 = tri.v1_ - tri.v0_;
    const vector3_t e1 = tri.v2_ - tri.v0_;
    const vector3_t t_normal = cross_product(e0, e1).normalize();
    const real_t ndotd = dot_product(normal, ln.direction_);

    if (abs(ndotd) > constants::kEpsilon) {
        //
        // Line is not parallel with the triangle. Compute intersection point.
        
        //
        // Generate orthonormal vectors U, V
        vector3<real_t>& lineU;
        vector3<real_t>& lineV;
        generate_complement_basis(ln.direction_, &lineU, &lineV);

        //
        // P minus V0 (P = line's origin point)
        const vector3_t pmv0 = ln.origin_ - tri.v0_;
        //
        // U dot E0 and U dot E1
        const real_t u_dot_e0 = dot_product(lineU, e0);
        const real_t u_dot_e1 = dot_product(lineU, e1);
        //
        // U dot (P - V0)
        const real_t u_dot_pmv0 = dot_product(lineU, pmv0);
        //
        // V dot E0 and V dot E1
        const real_t v_dot_e0 = dot_product(lineV, e0);
        const real_t v_dot_e1 = dot_product(lineV, e1);
        //
        // V dot (P - V0)
        const real_t v_dot_pmv0 = dot_product(lineV, pmv0);
        //
        // Compute determinant value
        const real_t det_val = u_dot_e0 * v_dot_e1 - u_dot_e1 * v_dot_e0;

        typename div_wrap_t<real_t>::div_helper_t div_det(det_val);
        const real_t b1 = div_det(v_dot_e1 * u_dot_pmv0 - u_dot_e1 * v_dot_pmv0);
        const real_t b2 = div_det(u_dot_e0 * v_dot_pmv0 - v_dot_e0 * u_dot_pmv0);
        const real_t b0 = real_t(1) - b1 - b2;

        if (operands_ge(b0, real_t(0)) && operands_ge(b1, real_t(0)) 
            && operands_ge(b2, real_t(0))) {
            //
            // Intersection point is inside the triangle.
            *ln_closest = -dot_product(t_normal, pmv0) / ndotd;
            p_barycentric[0] = b0;
            p_barycentric[1] = b1;
            p_barycentric[2] = b2;
            return 0;
        }
    }

    //
    // We get here when :
    // a) the line and the triangle are not parallel and the line does not
    //    intersect the triangle.
    // b) the line is parallel to the triangle.
    real_t dist_squared = FLT_MAX;
    segment<real_t, Space_Tridimensional> seg;
    typename div_wrap_t<real_t>::div_helper_t div_half(2);
    for (int i0 = 2, i1 = 0; i1 < 3; i0 = i1++) {
        seg.centre_ = div_half(tri.vertices_[i0] + tri.vertices_[i1]);
        seg.direction_ = div_half(tri.vertices_[i1] - tri.vertices_[i0]);
        seg.extent_ = div_half(seg.direction_.length());
        seg.direction_.normalize();

        real_t temp_st;
        real_t temp_lt;
        const real_t temp_sd = squared_distance(ln, seg, &temp_lt, &temp_st);
        if (temp_sd < dist_squared) {
            dist_squared = temp_sd;

            *ln_closest = temp_lt;
            p_barycentric[i0] = div_half(real_t(1) - temp_st / seg.extent_);
            p_barycentric[i1] = real_t(1) - p_barycentric[i0];
            p_barycentric[3 - i0 - i1] = real_t(0);
        }
    }

    return dist_squared;
}

/** @} */

} // namespace math
} // namespace v8
