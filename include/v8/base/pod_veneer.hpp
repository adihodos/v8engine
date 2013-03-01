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
//! \file pod_veneer.hpp

#include <v8/v8.hpp>
#include <v8/meta_programming/constraints.hpp>

namespace v8 { namespace base {

//!    
//! Applies RAII to an existing POD type, by callind the supplied functors
//! in constructor/destructor.
//! \param pod_type A pod type.
//! \param pod_constructor_fn Functor that is called to initialize the POD data.
//! \param pod_destructor_fn Functor that is called to cleanup the POD data.
//!
//! Suppose we have a C library that defines the following type, and we'd like
//! to take advantage of RAII.
//! \code
//!
//! //
//! // In the library's header
//! struct X11StateData {
//!     XWMHints*       wm_hints;
//!     XSizeHints*     size_hints;
//! };
//
//! int X11StateDataInitialize(struct X11StateData*);
//!
//! void X11StateDataCleanup(struct X11StateData*);
//!
//! //
//! // Client code 
//!
//! struct x11_state_data_ctor {
//!     void operator()(X11StateData* data) const {
//!         X11StateDataInitialize(data);   
//!     }
//! };
//!
//! struct x11_state_data_dtor {
//!     void operator()(X11StateData* data) const {
//!         X11StateDataCleanup(data);
//!     }
//! };
//!
//! //
//! // Can be used everywhere X11StateData is used.
//! typedef pod_veneer<
//!     X11StateData, 
//!     x11_state_data_ctor, 
//!     x11_state_data_dtor
//! >    veneer_x11_state_data;
//! \endcode
template<
    typename pod_type, 
    typename pod_constructor_fn,         
    typename pod_destructor_fn
> class pod_veneer : public pod_type {
public :
    typedef pod_veneer<pod_type, pod_constructor_fn, pod_destructor_fn>
        self_type;

    pod_veneer() {
        pod_constructor_fn()(static_cast<pod_type*>(this));
    }

    ~pod_veneer() {
        constraint_must_be_pod<pod_type>();
        constraint_must_be_same_size<pod_type, self_type>();

        pod_destructor_fn()(static_cast<pod_type*>(this));
    }
};

template
<
    typename pod_constructor_fn,         
    typename pod_destructor_fn
>
class pod_veneer<void, pod_constructor_fn, pod_destructor_fn> {
public :
    typedef pod_veneer<void, pod_constructor_fn, pod_destructor_fn>     self_type;
    
    pod_veneer() {
        pod_constructor_fn();
    }
    
    ~pod_veneer() {
        pod_destructor_fn();
    }
};    

} // namespace base
} // namespace v8
