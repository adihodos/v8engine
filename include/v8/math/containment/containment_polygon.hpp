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

#include <v8/v8.hpp>
#include <v8/math/objects/polygon2.hpp>
#include <v8/math/vector2.hpp>

namespace v8 { namespace math {

/** \addtogroup Containment
 *  @{
 */

//! \brief Test if a point lies inside a polygon.
//! \param poly Reference to a polygon.
//! \param pt_x X coordinate of the point to be tested.
//! \param pt_y Y coordinate of the point to be tested.
//! \returns 1 if the point is inside or on the boundary, 0 if the point is
//! outside or on the boundary.
//! \note For a detailed explanation of the algorithm see :
//! http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html .
template<typename real_t>
inline v8_int8_t point_in_polygon(
    const polygon2<real_t>& poly,
    real_t pt_x,
    real_t pt_y
    ) {
    v8_int8_t ret_val = 0;
    const v8_size_t num_vertices = poly.get_vertex_count();

    for (v8_size_t i = 0, j = num_vertices - 1; i < num_vertices; j = i, ++i) {
        if (((poly[i].y_ > pt_y) != (poly[j].y_ > pt_y))
                && (pt_x < (poly[j].x_ - poly[i].x_) * (pt_y - poly[i].y_)
                    / (poly[j].y_ - poly[i].y_) + poly[i].x_)) {
            ret_val = !ret_val;
        }
    }

    return ret_val;
}

//! \brief Test if a point is inside a polygon.
//! \see point_in_polygon
template<typename real_t>
inline v8_int_t point_in_polygon(
    const polygon2<real_t>& poly,
    const vector2<real_t>& pt
    ) {
    return point_in_polygon(poly, pt.x_, pt.y_);
}

/** @} */

} // namespace math
} // namespace v8
