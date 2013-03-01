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

#include <utility>
#include <v8/v8.hpp>

namespace v8 { namespace base {

#if defined(V8_COMPILER_HAS_CXX11_LAMBDAS)

//! \addtogroup v8_base_lib
//! @{

//! \class scope_exit
//! \brief Executes user defined statements when exiting from the scope where
//! the object was defined. Execution can be toggled on/off, by assigning a
//! boolean variable to the named object. The default is to execute
//! the statements.
//!
//! Example :
//! \code
//! v8_int_t var0 = 0;
//! v8_int_t var1 = 0;
//!
//! {
//!     // operations are executed when leaving this scope
//!     V8_ON_SCOPE_EXIT((++var0), (--var1));
//! }
//! //
//! // Now var0 is 1 and var1 is -1.
//! \endcode
//!
//! To prevent the execution of statements, one needs a named object. Then,
//! a boolean flag can be passed to the object representing the actions, to
//! prevent it from carrying them out.
//!
//! \code
//! v8_int_t var0 = 0;
//! v8_int_t var1 = 0;
//!
//! {
//!     //
//!     // Use a named action.
//!     V8_ON_SCOPE_EXIT_NAMED(increment_vars, (++var0), (++var1));
//!     //
//!     // Prevent the operations from being carried out.
//!     increment_vars = false;
//! }
//! //
//! // Bot var0 and var1 retain their values, since the assignments were not
//! // performed.
//! \endcode
//!
template<typename scope_exit_function>
class scope_exit {
public :

    typedef scope_exit<scope_exit_function>             class_type;

    scope_exit(scope_exit_function exit_func)
        : exit_fn_(exit_func),
          call_fn_(true) {}

    scope_exit(class_type&& rhs)
        : exit_fn_(std::move(rhs.exit_fn_)),
          call_fn_(rhs.call_fn_) {
        rhs.call_fn_ = false;
    }

    ~scope_exit() {
        if (call_fn_)
            exit_fn_();
    }

    class_type& operator=(v8_bool_t call_fun) {
        call_fn_ = call_fun;
        return *this;
    }

private :
    scope_exit_function     exit_fn_;
    v8_bool_t               call_fn_;

private :
    class_type& operator=(const class_type&);
    class_type& operator=(class_type&&);
    scope_exit(const class_type&);
};

template<typename scope_exit_function>
inline scope_exit<scope_exit_function>
mk_scope_exit(
    scope_exit_function exit_fn
    ) {
    return scope_exit<scope_exit_function>(exit_fn);
}

//! @}

#endif /* V8_COMPILER_HAS_CXX11_LAMBDAS */

} // namespace base
} // namespace v8


#if defined(V8_COMPILER_HAS_CXX11_LAMBDAS)

//! \def V8_ON_SCOPE_EXIT_NAMED
//! \brief Creates a named action to be performed on scope exit.
#define V8_ON_SCOPE_EXIT_NAMED(name, ...) \
    auto name = v8::base::mk_scope_exit( [&]() { __VA_ARGS__; } )

//! \def V8_ON_SCOPE_EXIT
//! \brief Creates and anonymous action to be performed on scope exit.
#define V8_ON_SCOPE_EXIT(...)   \
    V8_ON_SCOPE_EXIT_NAMED(V8_PASTE_X_Y(scope_exit_local_var, __LINE__), \
                           __VA_ARGS__)

#endif /* V8_COMPILER_HAS_CXX11_LAMBDAS */
