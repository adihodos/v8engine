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
#include <v8/math/space_traits.hpp>

namespace v8 { namespace math {

/**
 * \class   segment
 * \brief   Abstracts a line segment. The segment is stored using a 
 * 			centre-direction-extent representation. Given a segment S with
 * 			the centre at point P, direction vector D and extent e, any point
 * 			on the segment verifies the inequality :
 * 			P + (-e) * D <= Q <= P + e * D.
 */
template<typename real_type, int space_dim>
class segment {
public :
    typedef space_traits<real_type, space_dim>  space_traits_t;
    typedef typename space_traits_t::vector_t   vector_t;

    vector_t    centre_;	/*!< The centre point. */
    vector_t    direction_; /*!< The direction vector (unit-length). */
    real_type   extent_;	/*!< The extent of the segment. */

    segment() {}

    segment(const vector_t& centre, const vector_t& direction, real_type extent)
        : centre_(centre), direction_(direction), extent_(extent) {}

    /*!
     * \brief Construct a segment, given the two endpoints.
     * \param endpoint0 First endpoint.
     * \param endpoint1 Second endpoint.
     * \remarks Let E0 and E1 be the endpoints. The segment S with center C,
     *          direction D and extent e is derived with the following :
     *          C = (E0 + E1) / 2
     *          D = (E1 - E0) / ||E1 - E0||
     *          e = (E1 - E0).length() / 2
     */
    segment(const vector_t& endpoint0, const vector_t& endpoint1) {
        centre_ = (endpoint0 + endpoint1) / real_type(2);
        const vector_t diff = (endpoint1 - endpoint0);
        direction_ = normal_of(diff);
        typename internals::div_wrap_t<real_type>::div_helper_t div_val(2);
        extent_ = div_val(diff.magnitude());
    }

    /**
     * \brief   Returns the start point (head) of the segment (the
     * 			point P = C + (-e) * D).
     */
    const vector_t get_start_point() const {
        return centre_ + (-extent_) * direction_;
    }

    /**
     * \brief   Returns the end point (tail) of the segment 
     * 			(the point P = C + e * D).
     */
    const vector_t get_end_point() const {
        return centre_ + extent_ * direction_;
    }
};

} // namespace math
} // namespace v8
