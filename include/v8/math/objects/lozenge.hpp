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
#include <v8/math/objects/rectangle.hpp>

namespace v8 { namespace math {

/** \addtogroup Geometry
 *  @{
 */

/**
 * \brief   A lozenge. The lozenge is a sphere swept volume. A lozenge
 *          is the set of all point that are at most a distance R from a
 *          rectangle D.
 */
template<typename real_t, int space_dim>
class lozenge {
public :
    typedef space_traits<real_t, space_dim>             space_traits_t;
    typedef typename space_traits_t::vector_t           vector_t;
    typedef rectangle<real_t, Space_Tridimensional>     rectangle_t;

    /*!< Rectangle of lozenge. */
    rectangle_t    rectangle_;
    /*!< Radius of lozenge. */
    real_t         radius_;

    lozenge() {}

    lozenge(const rectangle_t& lrect, real_t lradius)
        : rectangle_(lrect), radius_(lradius) {}
};

/** @} */

} // namespace math
} // namespace v8
