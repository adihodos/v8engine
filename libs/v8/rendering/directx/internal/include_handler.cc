#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <third_party/stlsoft/platformstl/filesystem/path.hpp>

#include "v8/base/crt_handle_policies.hpp"
#include "v8/base/debug_helpers.hpp"
#include "v8/base/scoped_handle.hpp"

#include "v8/io/filesystem.hpp"
#include "v8/global_state.hpp"
#include "v8/utility/string_ext.hpp"

#include "v8/rendering/directx/internal/include_handler.hpp"

HRESULT v8::directx::internal::compiler_include_handler::Open(
    D3D10_INCLUDE_TYPE IncludeType, 
    LPCSTR pFileName, 
    LPCVOID pParentData, 
    LPCVOID *ppData, 
    UINT *pBytes
    ) {

    UNREFERENCED_PARAMETER(pParentData);
    UNREFERENCED_PARAMETER(IncludeType);

    *ppData = nullptr;
    *pBytes = 0;

    using namespace v8;

    platformstl::path include_file_path(state->Filesys->get_dir_path(
        filesys::Dir::Shaders));

    include_file_path.push(pFileName);

    using namespace v8::base;

    struct _stat file_data;

    if (::_stat(include_file_path.c_str(), &file_data)) {
        OUTPUT_DBG_MSGA("Failed to obtain file stats for %s, compiler will fail",
                        include_file_path.c_str());
        return E_INVALIDARG;
    }

    scoped_handle<crt_file_handle> fp(fopen(include_file_path.c_str(), "r"));

    if (!fp) {
        return E_FAIL;
    }

    file_contents_ = new char[file_data.st_size + 1];

    char tmp_buff[1024];
    v8_uint32_t offset = 0;

    while (fgets(tmp_buff, _countof(tmp_buff), scoped_handle_get(fp))) {

        v8_uint32_t byte_count = static_cast<v8_uint32_t>(strlen(tmp_buff));
        memcpy(scoped_pointer_get(file_contents_) + offset, tmp_buff, byte_count);
        offset += byte_count;

    }

    *ppData = scoped_pointer_get(file_contents_);
    *pBytes = offset;

    return S_OK;
}

HRESULT v8::directx::internal::compiler_include_handler::Close(
    LPCVOID
    ) {
    file_contents_ = nullptr;
    return S_OK;
}