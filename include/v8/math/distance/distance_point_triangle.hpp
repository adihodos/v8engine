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
#include <v8/math/objects/triangle.hpp>
#include <v8/math/space_traits.hpp>

namespace v8 { namespace math {

/** \addtogroup Distance
 *  @{
 */

/**
 * \brief Returns the square of the distance between a point and a triangle.
 * \param pt    The point.
 * \param tri   The triangle.
 * \see   <b>David Eberly, Geometric tools for Computer Graphics (2003)</b>
 *        for the point to triangle distance algorithm.
 */
template<typename real_t, int space_dim>
real_t distance_squared(
        const typename space_traits<real_t, space_dim>::vector_t& pt,
        const triangle<real_t, space_dim>& tri
        ) {
    typedef typename space_traits<real_t, space_dim>::vector_t vector_t;

    vector_t diff = tri.v0_ - pt;
    const vector_t edge0 = tri.v1_ - tri.v0_;
    const vector_t edge1 = tri.v2_ - tri.v0_;
    const real_t a00 = edge0.length_squared();
    const real_t a01 = dot_product(edge0, edge1);
    const real_t a11 = edge1.square_length();
    const real_t b0 = dot_product(edge0, diff);
    const real_t b1 = dot_product(edge1, diff);
    const real_t c = diff.length_squared();
    const real_t det = abs(a00 * a11 - a01 * a01);
    real_t s = a01 * b1 - a11 * b0;
    real_t t = a01 * b0 - a00 * b1;

    real_t sqr_dist;

    const real_t sum = s + t;
    if (operands_eq(sum, det) || sum < det) {
        if (s < real_t(0)) {
            if (t < real_t(0)) {
                //
                // region 4

                if (b0 < real_t(0)) {
                    t = real_t(0);

                    if (-b0 > a00 || operands_eq(-b0, a00)) {
                        s = real_t(1);
                        sqr_dist = a00 + real_t(2) * b0 + c;
                    } else {
                        s = -b0/a00;
                        sqr_dist = b0 * s + c;
                    }
                } else {
                    s = real_t(0);
                    if (b1 > real_t(0) || is_zero(b1)) {
                        t = real_t(0);
                        sqr_dist = c;
                    } else if (-b1 > a11 || operands_eq(-b1, a11)) {
                        t = real_t(1);
                        sqr_dist = a11 + real_t(2) * b1 + c;
                    } else {
                        t = -b1/a11;
                        sqr_dist = b1 * t + c;
                    }
                }
            } else {
                //
                // region 3
                s = real_t(0);
                if (b1 > real_t(0) || is_zero(b1)) {
                    t = real_t(0);
                    sqr_dist = c;
                } else if (-b1 > a11 || operands_eq(-b1, a11)) {
                    t = real_t(1);
                    sqr_dist = a11 + real_t(2) * b1 + c;
                } else {
                    t = -b1/a11;
                    sqr_dist = b1 * t + c;
                }
            }
        } else if (t < real_t(0)) {
            //
            // region 5
            t = real_t(0);
            if (b0 > real_t(0) || is_zero(b0)) {
                s = real_t(0);
                sqr_dist = c;
            } else if (-b0 > a00 || operands_eq(-b0, a00)) {
                s = real_t(1);
                sqr_dist = a00 + real_t(2) * b0 + c;
            } else {
                s = -b0/a00;
                sqr_dist = b0 * s + c;
            }
        } else {
            //
            // region 0
            // minimum at interior point
            typename div_wrap_t<real_t>::div_helper_t div_op(det);
            s = div_op(s);
            t = div_op(t);
            sqr_dist = s * (a00 * s + a01 * t + real_t(2) * b0)
                       + t * (a01 * s + a11 * t + real_t(2) * b1)
                       + c;
        }
    } else {
        real_t tmp0, tmp1, numer, denom;

        if (s < real_t(0)) {
            //
            // region 2
            tmp0 = a01 + b0;
            tmp1 = a11 + b1;
            if (tmp1 > tmp0) {
                numer = tmp1 - tmp0;
                denom = a00 - real_t(2) * a01 + a11;
                if (numer > denom || operands_eq(numer, denom)) {
                    s = real_t(1);
                    t = real_t(0);
                    sqr_dist = a00 + real_t(2) * b0 + c;
                } else {
                    s = numer/denom;
                    t = real_t(1) - s;
                    sqr_dist = s * (a00 * s + a01 * t + real_t(2) * b0)
                               + t * (a01 * s + a11 * t + real_t(2) * b1)
                               + c;
                }
            } else {
                s = real_t(0);
                if (tmp1 < real_t(0) || is_zero(tmp)) {
                    t = real_t(1);
                    sqr_dist = a11 + real_t(2) * b1 + c;
                } else if (is_zero(b1) || b1 > real_t(0)) {
                    t = real_t(0);
                    sqr_dist = c;
                } else {
                    t = -b1/a11;
                    sqr_dist = b1 * t + c;
                }
            }
        } else if (t < real_t(0)) {
            //
            // region 6
            tmp0 = a01 + b1;
            tmp1 = a00 + b0;
            if (tmp1 > tmp0) {
                numer = tmp1 - tmp0;
                denom = a00 - real_t(2) * a01 + a11;
                if (operands_eq(numer, denom) || numer > denom) {
                    t = real_t(1);
                    s = real_t(0);
                    sqr_dist = a11 + real_t(2) * b1 + c;
                } else {
                    t = numer/denom;
                    s = real_t(1) - t;
                    sqr_dist = s * (a00 * s + a01 * t + real_t(2) * b0)
                               + t * (a01 * s + a11 * t + real_t(2) * b1) + c;
                }
            } else {
                t = real_t(0);
                if (is_zero(tmp1) || tmp1 < real_t(0)) {
                    s = real_t(1);
                    sqr_dist = a00 + real_t(2) * b0 + c;
                } else if (is_zero(b0) || b0 > real_t(0)) {
                    s = real_t(0);
                    sqr_dist = c;
                } else {
                    s = -b0/a00;
                    sqr_dist = b0 * s + c;
                }
            }
        } else {
            //
            // region 1
            numer = a11 + b1 - a01 - b0;
            if (is_zero(numer) || numer < real_t(0)) {
                s = real_t(0);
                t = real_t(1);
                sqr_dist = a11 + real_t(2) * b1 + c;
            } else {
                denom = a00 - real_t(2) * a01 + a11;
                if (numer > denom || operands_eq(numer, denom)) {
                    s = real_t(1);
                    t = real_t(0);
                    sqr_dist = a00 + real_t(2) * b0 + c;
                } else {
                    s = numer/denom;
                    t = real_t(1) - s;
                    sqr_dist = s * (a00 * s + a01 * t + real_t(2) * b0)
                               + t * (a01 * s + a11 * t + real_t(2) * b1)
                               + c;
                }
            }
        }
    }

    if (sqr_dist < real_t(0)) {
        //
        // correct any rounding errors.
        sqr_dist = real_t(0);
    }

    return sqr_dist;
}

/** @} */

} // namespace math
} // namespace v8
