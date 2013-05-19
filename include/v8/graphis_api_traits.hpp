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

namespace v8 {

struct api_directx {};

struct api_opengl {};

template<typename real_type, typename api_type>
struct graphics_api_traits;

template<typename real_type>
struct graphics_api_traits<real_type, api_directx> {
    static real_type zmin() {
        return real_type(0);
    }

    static real_type zmax() {
        return real_type(1);
    }
};

template<typename real_type>
struct graphics_api_traits<real_type, api_opengl> {
    static real_type zmin() {
        return real_type(-1);
    }

    static real_type zmax() {
        return real_type(1);
    }
};

#if defined(V8FRAMEWORK_GRAPHICS_API_IS_DIRECTX)

typedef graphics_api_traits<float, api_directx>         graphics_traitsF;
typedef graphics_api_traits<double, api_directx>        graphics_traitsD;

#elif defined(V8FRAMEWORK_GRAPHICS_API_IS_DIRECTX)

typedef graphics_api_traits<float, api_opengl>          graphics_traitsF;
typedef graphics_api_traits<double, api_opengl>         graphics_traitsD;

#else

#error Unsupported graphics api!!

#endif

} // namespace v8
