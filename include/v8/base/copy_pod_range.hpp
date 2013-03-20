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

#include <cstring>

#include <v8/v8.hpp>
#include <v8/meta_programming/constraints.hpp>

namespace v8 { namespace base {

struct tag_pod_copy_memcpy {};

struct tag_pod_copy_assign {};

template<typename T, typename U>
void copy_pod_range_impl(
    const U* input, const v8_size_t count, T* output, tag_pod_copy_memcpy
    ) {
    memcpy(output, input, count * sizeof(T));
}

template<typename T, typename U>
void copy_pod_range_impl(
    const U* input, const v8_size_t count, T* output, tag_pod_copy_assign
    ) {
    for (v8_size_t i = 0; i < count; ++i) {
        output[i] = input[i];
    }
}

template<bool same_size = true>
struct copy_type {
    typedef tag_pod_copy_memcpy tag;
};

template<>
struct copy_type<false> {
    typedef tag_pod_copy_assign tag;
};

template<typename T, typename U>
void copy_pod_range(const U* input, const v8_size_t count, T* output) {
    constraint_must_be_pod<T>();
    constraint_must_be_pod<U>();
    copy_pod_range_impl(input, count, output, copy_type<sizeof(T) == sizeof(U)>::tag());
}

} // namespace base
} // namespace v8
