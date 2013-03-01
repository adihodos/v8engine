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

namespace v8 { namespace math {

template<typename T>
struct numerics;

template<>
struct numerics<float> {
    static CONSTEXPR float epsilon() NOEXCEPT {
        return 1.0e-6f;
    }

    static CONSTEXPR float pi() NOEXCEPT {
        return 3.14159265f;
    }

    static CONSTEXPR float two_pi() NOEXCEPT {
        return 6.2831853f;
    }

    static CONSTEXPR float pi_over2() NOEXCEPT {
        return 1.5707963f;
    }

    static CONSTEXPR float pi_over180() NOEXCEPT {
        return 0.0174532925f;
    }

    static CONSTEXPR float one_eighty_over_pi() NOEXCEPT {
        return 57.29577957f;
    }
};

template<>
struct numerics<double> {
    static CONSTEXPR double epsilon() NOEXCEPT {
        return 1.0e-6;
    }

    static CONSTEXPR double pi() NOEXCEPT {
        return 3.14159265;
    }

    static CONSTEXPR double two_pi() NOEXCEPT {
        return 6.2831853;
    }

    static CONSTEXPR double pi_over2() NOEXCEPT {
        return 1.5707963;
    }

    static CONSTEXPR double pi_over180() NOEXCEPT {
        return 0.0174532925;
    }

    static CONSTEXPR double one_eighty_over_pi() NOEXCEPT {
        return 57.29577957;
    }
};

/*!
 * \brief   Numeric constants, single precision floating point.
 */
typedef numerics<float>     numericsF;

/*!
 * \brief   Numeric constants, double precision floating point.
 */
typedef numerics<double>    numericsD;

} // namespace math
} // namespace v8
