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
 * \class   line
 * \brief   A line in 2D/3D. The line has an origin point and a 
 * 			direction vector (unit length). Given a line with origin point P and
 * 			direction vector D, any point on the line verifies the equation
 * 			Q = P + t * D, where t is a real number in the (-inf, +inf) range.
 */
template<typename real_type, int space_dim>
class line {
public :
    typedef space_traits<real_type, space_dim>  space_traits_t;
    typedef line<real_type, space_dim>          line_t;
    typedef typename space_traits_t::vector_t   vector_t;

    vector_t    origin_;	/*!< The origin point of the line. */
    vector_t    direction_; /*!< The unit length direction vector. */

    line() {}

    /**
     * \brief Construct a line, given an origin point and a direction vector.
     * \param origin Origin point of the line.
     * \param direction Unit length direction vector.
     */
    line(const vector_t& origin, const vector_t& direction)
        : origin_(origin), direction_(direction) {}

    /**
     * \brief Construct a line that passes through two specified points.
     *        Given two points P0 and P1, the line's unit length direction vector
     *        is D = (P1 - P0) / ||P1 - P0|| and P0 is the line's origin.
     * \param v0 First point.
     * \param v1 Second point.
     */
    static line_t make_from_points(const vector_t& v0, const vector_t& v1) {
        vector_t line_dir = normal_of(v1 - v0);
        return line_t(v0, line_dir);
    }
};
    
} // namespace math
} // namespace v8
