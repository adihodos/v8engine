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
 * \class   ellipsoid
 * \brief   An ellipsoid in R3. It is stored using a center-direction-extent
 * 			representation. Given an ellipsoid E with center C and an orthonormal
 * 			set of vectors { R, U, D }, representing axis directions, any point X
 * 			on the ellipsoid verifies the relation :
 * 			(X - C)^T * M * Q * M^T * (X - C) = 1, where M is a rotation matrix
 * 			whose columns are the direction vectors {R, U, D} and Q is a diagonal
 * 			matrix with elements (1/extent_x)^2, (1/extent_y)^2, (1/extent_z)^2.
 */
template<typename real_t>
class ellipsoid {
public :
    typedef vector3<real_t>     vector3_t;

    vector3_t   center_;
    vector3_t   vec_D_;
    vector3_t   vec_U_;
    vector3_t   vec_R_;
    real_t      extents_[3];

    ellipsoid() {}

    ellipsoid(
        const vector3_t& origin,
        const vector3_t& x_axis,
        const vector3_t& y_axis,
        const vector3_t& z_axis,
        real_t extent_x_axis,
        real_t extent_y_axis,
        real_t extent_z_axis
        ) {
        center_ = origin;
        vec_R_ = x_axis;
        vec_U_ = y_axis;
        vec_D_ = z_axis;
        extents_[0] = extent_x_axis;
        extents_[1] = extent_y_axis;
        extents_[2] = extent_z_axis;
    }
};

/**
 * \typedef ellipsoid<float> ellipsoidF
 * \brief   Ellipsoid using single precision floating point components.
 */
typedef ellipsoid<float>    ellipsoidF;

/**
 * \typedef ellipsoid<double> ellipsoidD
 * \brief   Ellipsoid using double precision floating point components.
 */
typedef ellipsoid<double>   ellipsoidD;

} // namespace math
} // namespace v8
