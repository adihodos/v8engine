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
 * \file win32_handle_policies.hpp
 */

#include <windows.h>
#include <v8/base/handle_traits.hpp>

namespace v8 { namespace base {

/**
 *\brief Management policy for file handles returned by CreateHandle().
 */
struct win32_file_handle : public handle_traits_base<HANDLE> {
    static handle_t null_handle() {
        return INVALID_HANDLE_VALUE;
    }

    static void dispose(handle_t file_handle) {
        if (file_handle != INVALID_HANDLE_VALUE)
            ::CloseHandle(file_handle);
    }
};

/**
 * \brief Policy for file mapping resources.
 */
struct win32_file_mapping : public handle_traits_base<HANDLE> {
    static handle_t null_handle() {
        return nullptr;
    }
    
    static void dispose(handle_t mapping_handle) {
        if (mapping_handle != null_handle())
            ::CloseHandle(mapping_handle);
    }
};

/**
 * \brief Policy for mapped file views resources.
 */
struct win32_mapped_file_view : public handle_traits_base<void*> {
    static handle_t null_handle() {
        return nullptr;
    }
    
    static void dispose(handle_t mapped_fview) {
        if (mapped_fview != null_handle())
            ::UnmapViewOfFile(mapped_fview);
    }
};

} // namespace base
} // namespace v8
