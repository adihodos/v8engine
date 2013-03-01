#pragma once

#include <windows.h>

#include <v8/v8.hpp>
#include <v8/base/debug_helpers.hpp>

namespace v8 { namespace directx { namespace internal {

const char* dx_get_error_string_from_hresult(
    HRESULT ret_code
    );

} // namespace internal
} // namespace directx
} // namespace v8

#ifndef CHECK_D3D
#define CHECK_D3D(ret_code_ptr, call_and_args)  \
    do {                                         \
        *(ret_code_ptr) = (call_and_args);              \
        if (FAILED(*(ret_code_ptr))) {                  \
            v8::base::debug::output_debug_string(__FILE__, __LINE__, \
                                "Call %s failed, HRESULT %#08x," \
                                "error string %s", \
                                V8_STRINGIZE_a(call_and_args), \
                                *(ret_code_ptr), \
                                v8::directx::internal::dx_get_error_string_from_hresult(*(ret_code_ptr))); \
        } \
__pragma(warning(push)) \
__pragma(warning(disable : 4127)) \
    } while (0); \
__pragma(warning(pop))
#endif
