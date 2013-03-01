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
#include <v8/math/math_utils.hpp>
#include <v8/math/vector3.hpp>

namespace v8 { namespace math {

/** \addtogroup Geometry
 *  @{
 */

/**!
 * \class axis_aligned_bounding_box3
 * \brief An axis aligned bounding box, in R3. It is represented with a
 *      minimum and maximum point. Given an AABB with minimum point Pmin and
 *      maximum point Pmax, a point Q is inside the AABB if Pmin <= Q <= Pmax.
 */
template<typename real_t, int space_dim>
class axis_aligned_bounding_box {
public :
    typedef space_traits<real_t, space_dim>     space_traits_t;
    typedef typename space_traits_t::vector_t   vector_t;

    vector_t min_point_;
    vector_t max_point_;

    axis_aligned_bounding_box() {}

    axis_aligned_bounding_box(
            const vector_t& aabb_min,
            const vector_t& aabb_max)
        : min_point_(aabb_min), max_point_(aabb_max) {}

    void get_center_and_extents(
        vector_t* center,
        vector_t* extents
        );
};

template<typename real_t, int space_dim>
void aabb_from_points(
    const typename space_traits<real_t, space_dim>::vector_t* pt_set,
    size_t pt_count,
    size_t pt_stride,
    axis_aligned_bounding_box<real_t, space_dim>* aabb
    );

/** @} */

} // namespace math
} // namespace v8

template<typename real_t, int space_dim>
void
v8::math::axis_aligned_bounding_box<real_t, space_dim>::get_center_and_extents(
    vector_t *center,
    vector_t *extents
    ) {
    using namespace v8::base;
    using namespace internals;
    typename div_wrap_t<real_t>::div_helper_t div_op(real_t(2));

    *center = (min_point_ + max_point_) / real_t(2);

    for (size_t i = 0; i < (size_t) space_dim; ++i) {
        extents->elements_[i] = div_op(max_point_.elements_[i]
                                       - min_point_.elements_[i]);
    }
}

template<typename real_t, int space_dim>
void v8::math::aabb_from_points(
    const typename space_traits<real_t, space_dim>::vector_t *pt_set,
    size_t pt_count,
    size_t pt_stride,
    axis_aligned_bounding_box<real_t, space_dim> *aabb
    ) {
    typedef typename space_traits<real_t, space_dim>::vector_t  vector_t;

    vector_t min = *pt_set;
    vector_t max = min;

    for (size_t i = 1; i < pt_count; ++i) {
        const vector_t& current_pt =
            *(vector_t*)((const char*) pt_set + i * pt_stride);
        for (size_t j = 0; j < (size_t) space_dim; ++j) {
            if (current_pt.elements_[j] < min.elements_[j]) {
                min.elements_[j] = current_pt.elements_[j];
            } else if (current_pt.elements_[j] > max.elements_[j]) {
                max.elements_[j] = current_pt.elements_[j];
            }
        }
    }

    memcpy(aabb->min_point_.elements_, min.elements_,
           sizeof(aabb->min_point_.elements_));
    memcpy(aabb->max_point_.elements_, max.elements_,
           sizeof(aabb->max_point_.elements_));
}
