#pragma once

#include <windows.h>

#include <v8/v8.hpp>
#include <v8/base/debug_helpers.hpp>
#include <v8/rendering/directx/internal/dxerr.h>

#ifndef CHECK_D3D
#define CHECK_D3D(ret_code_ptr, call_and_args)                                 \
    do {                                                                       \
        *(ret_code_ptr) = (call_and_args);                                     \
        if (FAILED(*(ret_code_ptr))) {                                         \
            v8::base::debug::output_debug_string(                              \
                __WFILE__, __LINE__,                                           \
                L"Call %s failed, HRESULT %#08x, error string %s",             \
                V8_STRINGIZE_w(call_and_args), *(ret_code_ptr),                \
                DXGetErrorStringW(*(ret_code_ptr))                             \
                );                                                             \
        }                                                                      \
    } while (0)
#endif
