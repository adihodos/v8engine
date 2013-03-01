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

/** \addtogroup Geometry
 *  @{
 */

/**
 * \class   sphere
 * \brief   A sphere in R3. A sphere is the set of all point that are situated
 * 			at a distance of <b>r</b> from a point <b>C</b>. Given a sphere S
 * 			with center at point C and radius r, any point X on the sphere
 * 			verifies the relation (x - Cx)^2 + (y - Cy)^2 + (z - Cz)^2 = r^2.
 */
template<typename real_t>
class sphere {
public :
    typedef vector3<real_t>     vector3_t;

    vector3_t   center_;	/*!< The center point of the sphere. */
    real_t      radius_;	/*!< The radius of the sphere. */

    sphere() {}

    sphere(const vector3_t& center, real_t radius)
        : center_(center), radius_(radius) {}
};

/**
 * \typedef sphere<float> sphereF
 *
 * \brief   Sphere object, using simple precision floating point components.
 */
typedef sphere<float>   sphereF;

/**
 * \typedef sphere<double> sphereD
 *
 * \brief   Sphere object, using double precision floating point components.
 */
typedef sphere<double>  sphereD;

/** @} */

} // namespace math
} // namespace v8
