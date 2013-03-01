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
//! \file param_select.hpp

#include <v8/v8.hpp>
#include <v8/meta_programming/if_then_else.hpp>

namespace v8 { namespace base {

//! \addtogroup Metaprogramming
//! @{

//!
//! Enables efficient passing/returning of values to/from functions.
//! If the type's size is bigger than the size of a pointer, it will be passed
//! as a reference and returned as a reference, otherwise is it used "as is".
//! \code
//! template<typename T>
//! class custom_container {
//! public :
//!     void add_object(typename param_select<T>::const_param_type obj) {
//!         ...
//!     }
//! };
//! 
//! custom_container<char> c;
//! c.add_object('65'); // passes parameter by value
//! 
//! struct client_socket_t {
//!     int sock_fd;
//!     int in_port;
//!     int out_port;
//!     int time_stamp;
//! };
//!
//! custom_container<client_socket_t> client_pool;
//! //
//! // passes parameter by reference
//! client_pool.add_object(client_socket_t(...)); 
//! \endcode
template<typename T>
struct param_select {

    enum {
        condition = sizeof(T) > sizeof(void*)
    };

    //! Alias for non const parameter.
    typedef typename if_then_else<
        condition, 
        T&, 
        T
    >::type param_type;

    //! Alias for const parameter.
    typedef typename if_then_else<
        condition, 
        T const&, 
        T
    >::type const_param_type;
};

//! @}    

} // namespace base
} // namespace v8

