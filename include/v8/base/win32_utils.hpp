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

#include <windows.h>

#include <v8/v8.hpp>

namespace v8 { namespace base { namespace win32 {

class scoped_dc {
private :
    NO_CC_ASSIGN(scoped_dc);

    HWND    owning_wnd_;
    HDC     devctx_;
public :
    scoped_dc(HWND wnd, HDC devctx) 
        : owning_wnd_(wnd), devctx_(devctx) {}

    ~scoped_dc() {
        if (devctx_)
            ::ReleaseDC(owning_wnd_, devctx_);
    }

    HDC get() const {
        return devctx_;
    }

    HWND get_owning_window() const {
        return owning_wnd_;
    }

    bool operator!() const {
        return devctx_ == nullptr;
    }

    HDC release() {
        HDC old_ctx = devctx_;
        devctx_ = nullptr;
        return old_ctx;
    }

    void reset(HWND window) {
        if (owning_wnd_ != window) {
            if (owning_wnd_) {
                if (devctx_) {
                    ReleaseDC(owning_wnd_, devctx_);
                }
                owning_wnd_ = nullptr;
                devctx_ = nullptr;
            }
            owning_wnd_ = window;
            devctx_ = GetDC(owning_wnd_);
        }
    }
};

void varargs_msgbox(
    UINT buttons, 
    const wchar_t* title, 
    const wchar_t* fmt, 
    ...
    );

} // namespace win32
} // namespace base
} // namespace v8
