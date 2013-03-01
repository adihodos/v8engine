#include <Windows.h>
#include <DxErr.h>

#include "v8/rendering/directx/internal/debug_helpers.hpp"

const char* v8::directx::internal::dx_get_error_string_from_hresult(
    HRESULT ret_code
    ) {

    const char* kStr = DXGetErrorDescriptionA(ret_code);

    if (!kStr) {
        kStr = "no info available";
    }

    return kStr;
}