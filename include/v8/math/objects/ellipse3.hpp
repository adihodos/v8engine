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

#include <v8/math/vector3.hpp>

namespace v8 { namespace math {

/**
 * \class   ellipse3
 * \ingroup Geometry.
 * \brief   An ellipse in R3. The equation for the plane containing the ellipse
 * 			is N . (X - C) = 0, where X is any point in the plane. A point on
 * 			the ellipse is parameterized by the equation
 * 			P = C + a * cos(theta) * U + b * sin(theta) * V, where
 * 			C is the center of the ellipse, a, b are the extents of the
 * 			ellipse's axes, {U, V, N} form an orthonormal left handed set and
 * 			theta is an angle in the [0, 2*PI] range.
 */
template<typename real_t>
class ellipse3 {
public :
    typedef vector3<real_t>     vector3_t;

    vector3_t   center_;	/*!< The center of the ellipse. */
    vector3_t   normal_;	/*!< The normal to the plane containing the ellipse. */
    vector3_t   dir_U_; /*!< Unit length direction vector, such that {U, V, N}
                        is a left handed orthonormal set. */
    vector3_t   dir_V_; /*!< Unit length direction vector, such that {U, V, N}
                        is a left handed orthonormal set. */
    real_t      ext_major_; /*!< The extent of the major axis. */
    real_t      ext_minor_; /*!< The extent of the minor axis. */

    ellipse3() {}

    ellipse3(const vector3_t& center, 
             const vector3_t& normal,
             const vector3_t& dir0,
             const vector3_t& dir1,
             real_t ext_major,
             real_t ext_minor)
        : center_(center), normal_(normal), dir_U_(dir0), dir_V_(dir1),
          ext_major_(ext_major), ext_minor_(ext_minor) {}
};

/**
 * \typedef ellipse3<float> ellipse3F
 * \brief   Ellipse in R3,  simple precision floating point components.
 */
typedef ellipse3<float>     ellipse3F;

/**
 * \typedef ellipse3<double> ellipse3D
 * \brief   Ellipse in R3,  simple precision floating point components.
 */
typedef ellipse3<double>    ellipse3D;

} // namespace math
} // namespace v8
