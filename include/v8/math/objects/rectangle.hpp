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

#include <v8/math/space_traits.hpp>

namespace v8 { namespace math {

/**
 * \class   rectangle
 * \ingroup Geometry.
 * \brief   A rectangle, stored in center-direction-extent format. A point in
 * 			the rectangle verifies the equation Q = C + a * U + b * V, where 
 * 			C is the center of the rectangle, U and V are orthonormal vectors,
 * 			a and b are real values, such that -e0 <= a <= e0 and -e1 <= b <= e1,
 * 			where e0 and e1 represent the extents along the axes indicated by
 * 			vectors U and V.
 */
template<typename real_t, int space_dim>
class rectangle {
public :
    typedef space_traits<real_t, space_dim>     space_traits_t;
    typedef typename space_traits_t::vector_t   vector_t;

    vector_t    center_;
    vector_t    axes_[2];
    real_t      extents_[2];

    rectangle() {}

    rectangle(
        const vector_t& center, 
        const vector_t& axis0, 
        const vector_t& axis1,
        real_t extent0,
        real_t extent1
        )
        : center_(center) {
        axes_[0] = axis0;
        axes_[1] = axis1;
        extents_[0] = extent0;
        extents_[1] = extent1;
    }

    /**
     * \brief   Gets the top left point : C + (-e0) * U + (e1) * V.
     */
    vector_t get_top_left() const {
        return center_ + -extents_[0] * axes_[0] + extents_[1] * axes_[1];
    };

    /**
     * \brief   Gets the top right point : C + (e0) * U + (e1) * V.
     */
    vector_t get_top_right() const {
        return center_ + extents_[0] * axes_[0] + extents_[1] * axes_[1];
    }

    /**
     * \brief   Gets the bottom left point : C + (-e0) * U + (-e1) * V.
     */
    vector_t get_bottom_left() const {
        return center_ - extents_[0] * axes_[0] - extents_[1] * axes_[1];
    }

    /**
     * \brief   Gets the top left point : C + (e0) * U + (-e1) * V.
     */
    vector_t get_bottom_right() const {
        return center_ + extents_[0] * axes_[0] - extents_[1] * axes_[1];
    }
};

} // namespace math
} // namespace v8
