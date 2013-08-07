//
// Copyright (c) 2011, 2012, 2013 Adrian Hodos
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
#include <v8/base/scoped_pointer.hpp>

namespace v8 { namespace math {

class random {
public :
    random();

    random(const v8_uint32_t seed);

    ~random();

public :

    ///
    /// Returns a non negative random number.
    v8_uint32_t next();

    ///
    /// Returns a random signed integer, in the [min, max) range.
    template<typename T>
    T next(const T min, const T max) {
        return static_cast<T>(next_double() * (max - min) + min);
    }

    ///
    /// Returns a random non negative integer, in the [0, max) range.
    v8_uint32_t next(const v8_uint32_t max);

    ///
    /// Returns a random number in the [0.0, 1.0) range.
    double next_double();

    float next_float() {
        return static_cast<float>(next_double());
    }

    ///
    /// Returns a random number in the [0.0, 1.0] range.
    double next_double_closed();

    float next_float_closed() {
        return static_cast<float>(next_double_closed());
    }

private :
    struct implementation_details;

    v8::base::scoped_ptr<implementation_details>    impl_;

private :
    NO_CC_ASSIGN(random);    
};    

} // namespace math
} // namespace v8

