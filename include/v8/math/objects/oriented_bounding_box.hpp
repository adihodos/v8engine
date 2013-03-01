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
 * \class   oriented_bounding_box3
 * \brief   An oriented bounding box in R3. The box has an origin point O, 
 * 			a left handed coordinate axes system represented by a set of
 * 			three orthonormal vectors (R - x axis, U - y axis, D - z axis, 
 * 			R = U x D) and a maximum extent along each axis. Thus, any point 
 * 			P(u, v, w) inside the bounding box can be written as
 * 			P = O + u*R + v*U + w*D.
 * \remarks Care must be taken when manipulating the direction vectors. 
 * 			In particular, the user must ensure that they form an orthonormal
 * 			left handed set.
 */
template<typename real_t>
class oriented_bounding_box3 {
public :
    typedef vector3<real_t>     vector3_t;

    vector3_t   origin_;	/*!< The origin point of the bounding box. */
    vector3_t   vec_D_; /*!< Unit length direction vector (z axis). */
    vector3_t   vec_U_; /*!< Unit length direction vector (y axis). */
    vector3_t   vec_R_; /*!< Unit length direction vector (x axis) */
    real_t      extents_[3];	/*!< Max/min extents along each axis. */

public :
    oriented_bounding_box3() {}

    oriented_bounding_box(
        const vector3_t& origin,
        const vector3_t& x_axis,
        const vector3_t& y_axis,
        const vector3_t& z_axis,
        real_t extent_x_axis,
        real_t extent_y_axis,
        real_t extent_z_axis
        ) {
        origin_ = origin;
        vec_R_ = x_axis;
        vec_U_ = y_axis;
        vec_D_ = z_axis;
        extents_[0] = extent_x_axis;
        extents_[1] = extent_y_axis;
        extents_[2] = extent_z_axis;
    }

    void get_min_max_points(
        vector_t* minpt,
        vector_t* maxpt
        );
};

template<typename real_t>
void oriented_bounding_box3<real_t>::get_min_max_points(
    vector3<real_t>* minpt,
    vector3<real_t>* maxpt
    ) {
    minpt->x_ = -extents_[0] * vec_R_;
    minpt->y_ = -extents_[1] * vec_U_;
    minpt->z_ = -extents_[2] * vec_D_;

    maxpt->x_ = extents_[0] * vec_R_;
    maxpt->y_ = extents_[1] * vec_U_;
    maxpt->z_ = extents_[2] * vec_D_;
}

/**
 * \typedef oriented_bounding_box3<float> oriented_bounding_box3F
 * \brief   OBB with single precision floating point components.
 */
typedef oriented_bounding_box3<float>   oriented_bounding_box3F;

/**
 * \typedef oriented_bounding_box3<double> oriented_bounding_box3D
 * \brief   OBB with double precision floating point components.
 */
typedef oriented_bounding_box3<double>  oriented_bounding_box3D;

//
// TODO : add functions to construct OOBB from a set of points.

} // namespace math
} // namespace v8
