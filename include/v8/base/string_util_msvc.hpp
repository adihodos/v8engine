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

#include <stdio.h>
#include <stddef.h>
#include <stdarg.h>

namespace v8 { namespace base {

//
// See this http://stackoverflow.com/questions/2915672/snprintf-and-visual-studio-2010
inline int 
vsnprintf(char* buffer, size_t size, const char* fmt, va_list args_ptr) {
    int count = -1;

    if (size) {
        count = _vsnprintf_s(buffer, size, _TRUNCATE, fmt, args_ptr);
    } 
    if (count == -1)
        count = _vscprintf(fmt, args_ptr);

    return count;
}

inline int
vsnwprintf(wchar_t* buff, size_t size, const wchar_t* fmt, va_list args_ptr) {
    int count = -1;

    if (size)
        count = _vsnwprintf_s(buff, size, _TRUNCATE, fmt, args_ptr);
    if (count == -1)
        count = _vscwprintf(fmt, args_ptr);

    return count;
}

inline int 
snprintf(char* buffer, size_t buff_size, const char* format, ...) {
    va_list args_ptr;
    va_start(args_ptr, format);
    int count = v8::base::vsnprintf(buffer, buff_size, format, args_ptr);
    va_end(args_ptr);
    return count;
}

inline int
snwprintf(wchar_t* buffer, size_t buff_size, const wchar_t* fmt, ...) {
    va_list args_ptr;
    va_start(args_ptr, fmt);
    int count = v8::base::vsnwprintf(buffer, buff_size, fmt, args_ptr);
    va_end(args_ptr);
    return count;
}

} // namespace base
} // namespace v8
