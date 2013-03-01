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

#include <v8/math/bidimensional_space_traits.hpp>
#include <v8/math/tridimensional_space_traits.hpp>

namespace v8 { namespace math {

/**
 * \class   triangle
 * \brief   A triangle, stored by it's three vertices.
 */
template<typename real_type, int space_dim>
class triangle {
public :
    typedef space_traits<real_type, space_dim>  space_traits_t;
    typedef typename space_traits_t::vector_t   vector_t;

    union {
        struct {
            vector_t    v0_;
            vector_t    v1_;
            vector_t    v2_;
        };
        vector_t        vertices_[3];   /*!< Convenient array access to the vertices. */
    };

    triangle() {}

    triangle(const vector_t& v0, const vector_t& v1, const vector_t& v2)
        : v0_(v0), v1_(v1), v2_(v2) {}

    /**
     * \brief   Calculates the normal vector, using clockwise winding order
     * 			for the vertices.
     */
    const vector_t compute_normal_cw() const {
        const vector_t e0 = v1_ - v0_;
        const vector_t e1 = v2_ - v0_;
        return cross_product(e0, e1).normalize();
    }

    /**
     * \brief   Calculates the normal vector, using counter-clockwise winding 
     * 			order for the vertices.
     */
    const vector_t compute_normal_ccw() const {
        return -compute_normal_cw();
    }
};

/**
 * \brief   triangle in R2, single precision floating pt. components.
 */
typedef triangle<float, Space_Bidimensional>    triangle2F;

/**
 * \brief   triangle int R3, single precision floating pt. components.
 */
typedef triangle<float, Space_Tridimensional>   triangle3F;

/**
 * \brief   triangle in R2, double precision floating pt. components.
 */
typedef triangle<double, Space_Bidimensional>   triangle2D;

/**
 * \brief   triangle in R3, double precision floating pt. components.
 */
typedef triangle<double, Space_Tridimensional>  triangle3D;

} // namespace v8
} // namespace math
