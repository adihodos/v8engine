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

//!
//! \file lerp.hpp

#include <cassert>

#include <v8/v8.hpp>
#include <v8/math/math_utils.hpp>

namespace v8 { namespace math {

//! \addtogroup Algebra
//! @{

//!
//! \brief Interpolates lineraly between two quantities.
//! \param[in] q0 First quantity.
//! \param[in] q1 Second quantity.
//! \param[in] beta Interpolation factor. Must be a real number, in the [0, 1]
//! range.
//! \returns A quantity such that it lies beta percent of the way from q0 to q1.
template<typename Quantity_Type, typename Real_Type>
inline Quantity_Type lerp(
    const Quantity_Type& q0,
    const Quantity_Type& q1,
    const Real_Type& beta
    ) {

    assert(operands_ge(beta, 0));
    assert(operands_le(beta, 1));

    return (1 - beta) * q0 + beta * q1;
}

//! @{

} // namespace math
} // namespace v8
