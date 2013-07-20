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

#include <d3d11.h>
#include <v8/v8.hpp>
#include <v8/base/operator_bool.hpp>
#include <v8/base/scoped_pointer.hpp>
#include <v8/base/shims/scoped_ptr.hpp>

namespace v8 { namespace directx {

class texture;
class renderer;

class texture_shader_binding {

public :
    typedef ID3D11ShaderResourceView*   binding_handle;    

public :
    
    texture_shader_binding() {}

    texture_shader_binding(const texture&        bound_texture,
                           const renderer&       rsys,
                           const v8_uint32_t     most_detailed_mip = 0) 
    {
        initialize(bound_texture, rsys, most_detailed_mip);
    }

    texture_shader_binding(const char*          texture_path,
                           const renderer&      rsys)
    {
        initialize(texture_path, rsys);
    }

    v8_bool_t initialize(const texture&         bound_texture,
                         const renderer&        rsys,
                         const v8_uint32_t      most_detailed_mip = 0);

    v8_bool_t initialize(const char*          texture_path,
                         const renderer&      rsys);

    v8_bool_t is_valid() const NOEXCEPT {
        return binding_ptr_ != nullptr;
    }

    operator int v8::base::operator_bool::*() const NOEXCEPT {
        return is_valid() ? &v8::base::operator_bool::a_member : nullptr;
    }

    binding_handle handle() const NOEXCEPT {
        assert(is_valid());
        return v8::base::raw_ptr(binding_ptr_);
    }

private :
    const texture*                                  texture_;

    v8::base::com_exclusive_pointer
    <
        ID3D11ShaderResourceView
    >::type                                         binding_ptr_;

private :
    NO_CC_ASSIGN(texture_shader_binding);
};

} // namespace directx
} // namespace v8
