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
 * \class   circle3
 * \ingroup Geometry.
 * \brief   A circle in R3. The plane containing the circle has the equation
 * 			N . (X - C) = 0, where X is any point in the plane.  
 * 			A point in the circle is parameterized by the equation 
 * 			Q = C + r*cos(theta)*U + r*sin(theta)*V, where 
 * 			{U, V, N} form an orthonormal left handed set.
 */
template<typename real_t>
class circle3 {
public :
    typedef vector3<real_t>     vector3_t;

    vector3_t   center_;	/*!< The center point. */
    vector3_t   normal_;	/*!< The normal to the plane containing the circle. */
    vector3_t   dir_U_; /*!< Unit length vector, such that {U, V, N} is a 
                        left handed orthonormal set. */
    vector3_t   dir_V_; /*!< Unit length vector, such that {U, V, N} is a
                        left handed orthonormal set. */
    real_t      radius_;	/*!< The radius of the circle. */

    circle3() {}

    circle3(const vector3_t& center, 
            const vector3_t& normal, 
            const vector3_t& dir0,
            const vector3_t& dir1,
            real_t radius)
        : center_(center), normal_(normal), dir_U_(dir0), dir_V_(dir1),
          radius_(radius) {}
};

/**
 * \typedef circle3<float> circle3F
 * \brief   Circle in R3, using simple precision floating point components.
 */
typedef circle3<float>      circle3F;

/**
 * \typedef circle3<double> circle3D
* \brief   Circle in R3, using double precision floating point components.
 */
typedef circle3<double>     circle3D;

} // namespace math
} // namespace v8
