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
 * \class   plane
 * \brief   A plane in R3. The plane is stored using a unit length vector
 * 			perpendicular to the plane (the plane's normal) and an constant.
 * 			Thus, given a plane with unit length normal N(a, b, c) and a
 * 			constant D, any point P(X, Y, Z) in the plane verifies the equation
 * 			aX + bY + cZ + D = 0.
 */
template<typename real_type>
class plane {
public :
    typedef vector3<real_type>  vector_t;

    vector_t    normal_;	/*!< The plane's normal. */
    real_type   offset_;	/*!< The plane's constant. */

    /**
     * \enum Plane_Side
     * \brief Encodes possible positions of a point, relative to a plane.
     */
    enum Plane_Side {
        /**
         * A point is on the "positive" side of the plane if it verifies
         * the inequation : N dot P + D > 0.
         */
        Plane_Positive_Side,
        /**
         * A point is on the negative side if it verifies
         * the inequation N dot P + D < 0.
         */
        Plane_Negative_Side,
        /**
         * Points contained in the plane verify the equation N dot P + D = 0.
         */
        Plane_Contained
    };

    plane() {}

    plane(real_type Nx, real_type Ny, real_type Nz, real_type D)
        : normal_(Nx, Ny, Nz), offset_(D) {}

    /**
     * \brief   Constructs from a specified normal and constant
     * \param   normal  A vector3 representing the plane's normal. Must be unit 
     * 					length.
     * \param   offset  The offset quantity.
     */
    plane(const vector_t& normal, real_type offset) 
        : normal_(normal), offset_(offset) {}

    /**
     * \brief   Construct with an origin point and a specified normal vector.
     * \param   normal  The plane's normal. Must be a vector3 object 
     * 					of unit length.
     * \param   origin  The origin point of the plane.
     */
    plane(const vector_t& normal, const vector_t& origin)
        : normal_(normal) {
        offset_ = -dot_product(normal_, origin);
    }

    /**
     * \brief   Makes a plane from three non-collinear points.
     * \param   p0  First point.
     * \param   p1  Second point.
     * \param   p2  Third point.
     */
    static plane<real_type> make_from_points(
        const vector_t& p0, const vector_t& p1, const vector_t& p2
        ) {
        plane<real_type> new_plane;
        new_plane.normal_ = cross_product(p1 - p0, p2 - p0);
        new_plane.normal_.normalize();
        new_plane.offset_ = -dot_product(new_plane.normal_, p0);
        return new_plane;
    }

    /**
     * \brief   Makes a plane that is parallel with two directions and has
     * 			the specified point as its origin.
     * \param   d0      The first direction vector.
     * \param   d1      The second direction vector.
     * \param   origin  The origin point.
     */
    static plane<real_type> make_from_vectors_and_point(
        const vector_t& d0, const vector_t& d1, const vector_t& origin
        ) {
        vector_t plane_normal = cross_product(d0, d1);
        plane_normal.normalize();
        return plane<real_type>(plane_normal, origin);
    }

    int which_side(const vector_t& point) const {
        const real_type result = dot_product(normal_, point) + offset_;
        if (result < real_type(0))
            return Plane_Negative_Side;
        else if (result > real_type(0))
            return Plane_Positive_Side;
        else
            return Plane_Contained;
    }

    /**
     * \brief Normalizes a plane (makes the normal unit length and updates
     *        the offset value).
     */
    plane<real_type>& normalize() {
        const real_type n_length = normal_.magnitude();
        typename internals::div_wrap_t<real_type>::div_helper_t 
            div_val(n_length);
        normal_.x_ = div_val(normal_.x_);
        normal_.y_ = div_val(normal_.y_);
        normal_.z_ = div_val(normal_.z_);
        offset_ = div_val(offset_);
        return *this;
    }
};

/**
 * \brief   plane, single precision floating pt. components.
 */
typedef plane<float>   plane3F;

/**
 * \brief   plane, double precision floating pt. components.
 */
typedef plane<double>  plane3D;
  
} // namespace math
} // namespace v8
