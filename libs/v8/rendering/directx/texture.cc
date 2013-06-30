#include <third_party/stlsoft/platformstl/filesystem/path.hpp>
#include <third_party/stlsoft/platformstl/filesystem/memory_mapped_file.hpp>

#include "v8/base/shims/scoped_ptr.hpp"
#include "v8/io/filesystem.hpp"
#include "v8/rendering/directx/renderer.hpp"
#include "v8/rendering/directx/constants.hpp"
#include "v8/rendering/directx/internal/dds_texture_loader.hpp"
#include "v8/rendering/directx/internal/debug_helpers.hpp"
#include "v8/utility/win_util.hpp"

#include "v8/rendering/directx/texture_info.hpp"
#include "v8/rendering/directx/texture.hpp"

namespace {

UINT map_bindflags_to_directx_bindflags(const v8_uint32_t res_usage) {
    UINT d3d_bindflags = 0u;
    if (res_usage & v8::directx::BindingFlag::ShaderResource) {
        d3d_bindflags |= D3D11_BIND_SHADER_RESOURCE;
    }
    if (res_usage & v8::directx::BindingFlag::DepthStencil) {
        d3d_bindflags |= D3D11_BIND_DEPTH_STENCIL;
    }
    if (res_usage & v8::directx::BindingFlag::RenderTarget) {
        d3d_bindflags |= D3D11_BIND_RENDER_TARGET;
    }
    if (res_usage & v8::directx::BindingFlag::StreamOutput) {
        d3d_bindflags |= D3D11_BIND_STREAM_OUTPUT;
    }
    if (res_usage & v8::directx::BindingFlag::UnorderedAccess) {
        d3d_bindflags |= D3D11_BIND_UNORDERED_ACCESS;
    }
    return d3d_bindflags;
}

struct TextureType {
    enum value {
        Tex_1D,
        Tex_2D,
        Tex_3D
    };
};

const D3D11_SRV_DIMENSION srv_dimension_mapping[] = {
    D3D11_SRV_DIMENSION_TEXTURE1D,
    D3D11_SRV_DIMENSION_TEXTURE2D,
    D3D11_SRV_DIMENSION_TEXTURE3D
};

const D3D11_UAV_DIMENSION uav_dimension_mapping[] = {
    D3D11_UAV_DIMENSION_TEXTURE1D,
    D3D11_UAV_DIMENSION_TEXTURE2D,
    D3D11_UAV_DIMENSION_TEXTURE3D
};

}

v8::directx::texture::texture()
    :       width_(0)
        ,   height_(0)
        ,   depth_(0)
        ,   array_size_(0)
        ,   format_(DXGI_FORMAT_UNKNOWN)
        ,   flags_(0)
{}

v8::directx::texture::texture(const texture_info_t&         tex_info,
                              v8::directx::renderer*        rsys)
    :       width_(0)
        ,   height_(0)
        ,   depth_(0)
        ,   array_size_(0)
        ,   format_(DXGI_FORMAT_UNKNOWN)
        ,   flags_(0)
{
    initialize(tex_info, rsys);
}

v8::directx::texture::~texture() {}

v8_bool_t 
v8::directx::texture::initialize(const texture_info_t&      tex_info, 
                                 v8::directx::renderer*     rsys) {
    if (resource_) {
        return true;
    }

    //
    //  Load image bytes
    //  Create texture of corresponding type (1D, 2D, 3D)
    //  Create a shader resource view by default
    //  for each aditional resource views specified
    //      create the corresponding view
    platformstl::path textureFilePath(tex_info.tex_filename.c_str());
    utility::win32::scoped_wide_string_t filePathWideStr(
        utility::win32::multibyte_string_to_wide_string(textureFilePath.c_str()));

    using namespace v8::base;

    if (strcmp(textureFilePath.get_ext(), "dds")) {
        OUTPUT_DBG_MSGA("Error, only DDS textures are supported for now");
        return false;
    }

    platformstl::memory_mapped_file mmfile_texture(tex_info.tex_filename.c_str());
    internal::TextureResourceInfo_t texture_data;
    HRESULT                         ret_code;

    CHECK_D3D(
        &ret_code, 
        internal::CreateDDSTextureFromMemory(
            rsys->internal_np_get_device(),
            static_cast<const byte*>(mmfile_texture.memory()),
            mmfile_texture.size(),
            &texture_data,
            reinterpret_cast<ID3D11Resource**>(raw_ptr_ptr(resource_)),
            tex_info.tex_bindflags & BindingFlag::ShaderResource ? 
                raw_ptr_ptr(tex_srv_.view) : nullptr));
                
    if (FAILED(ret_code)) {
        return false;
    }
    return true;
}
