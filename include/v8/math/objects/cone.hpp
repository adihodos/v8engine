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
#include <v8/math/vector3.hpp>

namespace v8 { namespace math {

/**
 * \class   cone
 * \brief   A cone.
 */
template<typename real_t>
class cone {
public :
    typedef vector3<real_t>     vector3_t;

    vector3_t   origin_;	/*!< The point of origin. */
    vector3_t   direction_; /*!< The unit lenght direction vector (the cone's axis). */
    real_t      height_;	/*!< The height (set to FLT_MAX if infinite). */
    real_t      cos_theta_; /*!< The cosine of the cone's angle. */
    real_t      sin_theta_; /*!< The sine of the cone's angle */

    cone() {}

    cone(const vector3_t& origin, const vector3_t direction, real_t height,
         real_t angle)
         : origin_(origin), direction_(direction), height_(height) {
        const real_t angle_rads = to_radians(angle);
        cos_theta_ = cos(angle);
        sin_theta_ = sin(angle);
    }
};

/**
 * \typedef cone<float> coneF
 * \brief   Cone object, simple precision floating point components.
 */
typedef cone<float>     coneF;

/**
 * \typedef cone<double> coneD
 * \brief   Cone object, double precision floating point components.
 */
typedef cone<double>    coneD;

} // namespace math
} // namespace v8
