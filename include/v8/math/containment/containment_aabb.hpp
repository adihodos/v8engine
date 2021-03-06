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
#include <v8/math/objects/axis_aligned_bounding_box.hpp>
#include <v8/math/math_utils.hpp>

namespace v8 { namespace math {

/**
 * \brief   Test if a point is inside an axis aligned bounding box.
 * \param aabb  Reference to an aabb.
 * \param pt    Reference to a point.
 * \remarks A point P is inside an aabb Q whenever Q.min <= P <= Q.max.
 * \return True if point is contained in aabb, false if not.
 */
template<typename real_t, int space_dim>
inline bool contains_object(
    const axis_aligned_bounding_box<real_t, space_dim>& aabb,
    const typename space_traits<real_t, space_dim>::vector_t& pt
    ) {
    for (size_t i = 0; i < (size_t) space_dim; ++i) {
        if (pt.elements_[i] < aabb.min_point_.elements_[i] ||
            pt.elements_[i] > aabb.max_point_.elements_[i]) {
            return false;
        }
    }
    return true;
}

} // namespace math
} // namespace v8
