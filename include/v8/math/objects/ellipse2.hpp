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
 * \class   ellipse2
 * \ingroup Geometry
 * \brief   An ellipse in R2. An ellipse is the set of point that verify the
 * 			equation <b>(x/a)^2 + (y/b)^2 = 1</b>, where a and b are extents
 * 			along the X and Y axis. The parameterized equation of a point Q
 * 			situated on an ellipse E with center C and extents a and b is :
 * 			Q = C + a*cos(theta)*U + b*sin(theta)*V, where U and V are 
 * 			orthonormal vectors and theta is an angle in the [0, 2*PI] range.
 */
template<typename real_t>
class ellipse2 {
public :
    typedef vector2<real_t>     vector_t;

    vector2_t   center_;	/*!< The center of the ellipse. */
    real_t      axis_[2];   /*!< Extents along the x and y axis. */

    ellipse2() {}

    ellipse2(const vector2_t& center, real_t x_axis, real_t y_axis)
        : center_(center) {
        axis_[0] = x_axis;
        axis_[1] = y_axis;
    }
};

/**
 * \typedef ellipse2<float> ellipse2F
 * \brief   2D ellipse, simple precision floating point components.
 */
typedef ellipse2<float>     ellipse2F;

/**
 * \typedef ellipse2<double> ellipse2D
 * \brief   2D ellipse, double precision floating point components.
 */
typedef ellipse2<double>    ellipse2D;

} // namespace math
} // namespace v8
