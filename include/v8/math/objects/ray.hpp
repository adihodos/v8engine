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

namespace v8 { namespace math {

/**
 * \class   ray
 * \ingroup Geometric objects.
 * \brief   Abstract representation of a ray. A ray is represented with an
 * 			origin point and a unit length direction vector. Given a ray R
 * 			with origin point P and direction vector D, any point on the ray
 * 			verifies the equation Q = P + t * D, where t is a real number
 * 			in the [0, +inf) range.
 */
template<typename real_type, int space_dim>
class ray {
public :
    typedef space_traits<real_type, space_dim>  space_traits_t;
    typedef typename space_traits_t::vector_t   vector_t;
    typedef ray<real_type, space_dim>           ray_t;

    vector_t    origin_;	/*!< The origin point. */
    vector_t    direction_; /*!< The direction vector (unit length). */

    ray() {}

    /**
     * \brief Construct from origin point and unit length direction vector.
     */
    ray(const vector_t& origin, const vector_t& direction)
        : origin_(origin), direction_(direction) {}

    /**
     * \brief Given two points P0 and P1, construct a ray originating at P0
     *        and passing through P1. The direction vector is
     *        D = (P1 - P0) / ||P1 - P0||.
     * \param
     */
    static ray_t make_from_points(const vector_t& p0, const vector_t& p1) {
        const vector_t ray_dir = normal_of(p1 - p0);
        return ray_t(p0, ray_dir);
    }
};

} // namespace v8
} // namespace math
