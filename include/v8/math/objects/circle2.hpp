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

#include <v8/math/vector2.hpp>

namespace v8 { namespace math {

/**
 * \class   circle2
 * \ingroup Geometry
 * \brief   A circle in R2. Given the circle C with center point P and radius r,
 * 			the parameterized equation of a point Q on the circle is
 * 			<b>Q = C + rsin(theta)U + rcos(theta)V</b>, 
 * 			where U and V are orthonormal vectors and theta is an angle 
 * 			in the [0, 2*PI] range.
 */
template<typename real_t>
class circle2 {
public :
    typedef vector2<real_t>     vector_t;

    vector_t    center_;	/*!< The center point of the circle. */
    real_t      radius_;	/*!< The radius of the circle. */

    circle() {}
  
    circle(const vector_t& centre, real_t radius) 
        : centre_(centre), radius_(radius) {}    
};

/**
 * \typedef circle2<float> circle2F
 * \brief   2D circle, using simple precision floating point components.
 */
typedef circle2<float>      circle2F;

/**
 * \typedef circle2<double> circle2D
 * \brief   2D circle, using double precision floating point components.
 */
typedef circle2<double>     circle2D;

} // namespace math
} // namespace v8
