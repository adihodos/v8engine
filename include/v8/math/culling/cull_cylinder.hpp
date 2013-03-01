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

#include <v8/math/objects/plane.hpp>
#include <v8/math/objects/cylinder.hpp>

namespace v8 { namespace math {

/** \addtogroup Culling
 *  @}
 */

/**
 * \brief Cull a cylinder by a plane.
 * \param[in] cull_plane Culling plane.
 * \param[in] cyl Cylinder to be culled.
 * \return True if the cylinder can be culled, false otherwise.
 */
template<typename real_t>
inline bool cull_object(
    const plane<real_t>& cull_plane,
    const cylinder<real_t>& cyl
    ) {
    const real_t ndotd = dot_product(cull_plane.normal_, cyl.axis_.direction_);
    const real_t rval = real_t(0.5f) * cyl.height_ * abs(ndotd)
                       + cyl.radius_ * sqrt(real_t(1) - ndotd * ndotd);
    return (dot_product(cull_plane.normal_, cyl.axis_.origin_) + rval)
            < cull_plane.offset_;
}


/** @} */

} // namespace math
} // namespace v8
