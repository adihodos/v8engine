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
#include <v8/base/scoped_resource.hpp>
#include <v8/base/resource_policies/self_managed_storage.hpp>
#include <v8/base/resource_policies/com_refcount_ownership.hpp>

namespace v8 { namespace base {

///
/// \brief Helper template that defines a typedef for a shared pointer
/// to a COM interface.
/// \param COM_Interface_Type A type representing a COM interface.
///
/// \code
/// typedef com_shared_pointer<ID3D11Device>::type  sp_device_t;
/// ...
/// sp_device_t graphics_device = ...
/// graphics_device->CreateBuffer(...)
/// \endcode
template<typename COM_Interface_Type>
struct com_shared_pointer {
    
    typedef v8::base::scoped_resource
    <
        COM_Interface_Type,
        v8::base::self_managed_storage,
        v8::base::com_refcount_ownership
    >                                                                      type;
    
private :
    ~com_shared_pointer() {}
};

} // namespace base
} // namespace v8
