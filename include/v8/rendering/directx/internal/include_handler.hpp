#pragma once

#include <d3d11.h>
#include <d3d10shader.h>
#include <v8/base/scoped_pointer.hpp>

namespace v8 { namespace directx { namespace internal {

class compiler_include_handler : public ID3D10Include {
private :
    const char* root_directory_;
    v8::base::scoped_ptr<char, v8::base::default_array_storage> file_contents_;

public :
    compiler_include_handler(const char* root_dir = nullptr);

    HRESULT __stdcall Open(
        D3D10_INCLUDE_TYPE IncludeType,
        LPCSTR pFileName,
        LPCVOID pParentData,
        LPCVOID *ppData,
        UINT *pBytes
    );

    HRESULT __stdcall Close(LPCVOID data_ptr);
};

} // namespace internal
} // namespace directx
} // namespace v8
