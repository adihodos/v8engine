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

/**
 * \file    win32_device_context.hpp
 * \brief   Traits for Windows OpenGL context resources.
 */

#include <windows.h>
#include <v8/v8.hpp>
#include <v8/base/handle_traits.hpp>

namespace v8 { namespace base {

///
/// \brief  Traits class for Windows device context resource handles obtained
///         by calling the GetDC() API.
struct win_device_context : public handle_traits_base<HDC> {
    static handle_t null_handle() {
        return nullptr;
    }

    static void dispose(handle_t context) {
        if (context != null_handle()) {
            HWND associated_window = WindowFromDC(context);
            if (associated_window != nullptr) {
                ReleaseDC(associated_window, context);
            }
        }
    }
};

///
/// \brief  Traits class for Windows device context resource handles obtained
///         by calling the CreateCompatibleDC(), CreateDC() APIs.
struct win_compatible_device_context : public handle_traits_base<HDC> {
    static handle_t null_handle() {
        return nullptr;
    }

    static void dispose(handle_t context) {
        if (context != null_handle()) {
            DeleteDC(context);
        }
    }
};

} // namespace base
} // namespace v8
