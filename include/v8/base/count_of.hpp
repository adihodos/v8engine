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

namespace v8 { namespace base {
    
/*!
 * \brief Returns the number of elements in an array.
 */
template<typename T, size_t N>
inline CONSTEXPR size_t count_of_array(T (&)[N]) NOEXCEPT {
    return N;
}

namespace internals {

template<v8_int_t N>
struct dimensionof_helper {
    v8_char_t   arr[N];
};

template<typename T, v8_int_t N>
dimensionof_helper<N> dimensionof_helper_fn(T (&)[N]);

} // namespace internals
} // namespace base
} // namespace v8

/*!
 * \def dimension_of(an_array)
 * \brief   A macro to compute the number of elements in an array, at compile 
 *          time.
 * \code
 *  v8_int_t array1[20];
 *  auto num_elements = dimension_of(array1);
 * \endcode
 */
#if defined(dimension_of)
#undef dimension_of
#endif

#define dimension_of(an_array)  \
    (sizeof(v8::base::internals::dimensionof_helper_fn(an_array).arr))
