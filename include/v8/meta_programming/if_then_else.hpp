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
//! \file if_then_else.hpp

#include <v8/v8.hpp>

namespace v8 { namespace base {

//!    
//! \defgroup Metaprogramming Support tools for template metaprogramming.
//! @{

//! Selects a type from two type arguments, based on a condition.
//! \code
//! // 
//! // This will select int as the type of variable i, when condition is true,
//! // or void* when the condition evaluates to false.
//! select_type<sizeof(int) > sizeof(void*), int, void*>::type i = 0;
//! \endcode
template<bool condition, typename T, typename U>
struct if_then_else;

//! Specialisation when the condition evaluates to true. 
//! Selects its first type argument.
template<typename T, typename U>
struct if_then_else<true, T, U> {
    typedef T                       type; //! Alias for the first type.
};

//! Specialisation when the condition evaluates to false. 
//! Selects its second type argument.
template<typename T, typename U>
struct if_then_else<false, T, U> {
    typedef U                       type; //! Alias for the second type.
};

//! @}

} // namespace base
} // namespace v8

