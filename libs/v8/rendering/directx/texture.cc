#include <third_party/directx_tex/DirectXTex.h>
#include <third_party/stlsoft/platformstl/filesystem/path.hpp>

#include "v8/base/crt_handle_policies.hpp"
#include "v8/base/scoped_handle.hpp"
#include "v8/global_state.hpp"
#include "v8/io/filesystem.hpp"
#include "v8/rendering/directx/renderer.hpp"
#include "v8/rendering/directx/constants.hpp"
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

v8::directx::texture::texture(
    const texture_info_t& tex_info,
    v8::directx::renderer* rsys
    )
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

v8_bool_t v8::directx::texture::initialize( 
    const texture_info_t& tex_info, v8::directx::renderer* rsys
    ) {
    if (resource_) {
        return true;
    }

    //
    //  Load image bytes
    //  Create texture of corresponding type (1D, 2D, 3D)
    //  Create a shader resource view by default
    //  for each aditional resource views specified
    //      create the corresponding view
    platformstl::path textureFilePath(state->file_sys()->get_dir_path(
        filesys::Dir::Textures));
    textureFilePath.push(tex_info.tex_filename.c_str());
    utility::win32::scoped_wide_string_t filePathWideStr(
        utility::win32::multibyte_string_to_wide_string(textureFilePath.c_str()));

    using namespace v8::base;
    DirectX::TexMetadata    tex_metadata;
    DirectX::ScratchImage   scratch_image;
    HRESULT                 ret_code;

    if (!strcmp(textureFilePath.get_ext(), "dds")) {
        ret_code = DirectX::LoadFromDDSFile(
            scoped_pointer_get(filePathWideStr), DirectX::DDS_FLAGS_NONE, 
            &tex_metadata, scratch_image
            );
    } else {
        ret_code = DirectX::LoadFromWICFile(
            scoped_pointer_get(filePathWideStr), DirectX::WIC_FLAGS_NONE, 
            &tex_metadata, scratch_image
            );
    }

    if (FAILED(ret_code)) {
        return false;
    }

    width_      = static_cast<v8_uint32_t>(tex_metadata.width);
    height_     = static_cast<v8_uint32_t>(tex_metadata.height);
    depth_      = static_cast<v8_uint32_t>(tex_metadata.depth);
    format_     = tex_metadata.format;
    array_size_ = static_cast<v8_uint32_t>(tex_metadata.arraySize);
    flags_      = tex_info.tex_bindflags;

    const D3D11_USAGE res_usage_flags   = D3D11_USAGE_IMMUTABLE;
    const UINT res_bind_flags           = map_bindflags_to_directx_bindflags(flags_);
    const UINT cpu_access               = 0u;
    ID3D11Device* graphics_device       = rsys->internal_np_get_device();
    v8_int_t texture_type;

    if (tex_metadata.height == 1) {
        //
        // TODO : add code for 1D texture creation
        texture_type = TextureType::Tex_1D;
        return false;
    } else if (tex_metadata.depth == 1) {
        texture_type = TextureType::Tex_2D;
        //
        // Create 2D texture
        D3D11_TEXTURE2D_DESC tex_desc = {
            width_,
            height_,
            static_cast<UINT>(tex_metadata.mipLevels),
            array_size_,
            format_,
            { 1, 0 },
            res_usage_flags,
            res_bind_flags,
            cpu_access,
            0
        };

        D3D11_SUBRESOURCE_DATA tex_init_data = {
            scratch_image.GetPixels(), width_ * height_, 0
        };

        ID3D11Texture2D* tex2d_res = nullptr;
        CHECK_D3D(
            &ret_code,
            graphics_device->CreateTexture2D(&tex_desc, &tex_init_data,
                                             &tex2d_res));
        resource_ = tex2d_res;
    } else {
        //
        // TODO : add code for 3D textures and texture cubes
        texture_type = TextureType::Tex_3D;
        return false;
    }

    if (!resource_) {
        return false;
    }

    if (res_bind_flags & D3D11_BIND_SHADER_RESOURCE) {
        tex_srv_.view_desc.Format = format_;
        tex_srv_.view_desc.ViewDimension = srv_dimension_mapping[texture_type];
        tex_srv_.view_desc.Texture2D.MipLevels = static_cast<UINT>(tex_metadata.mipLevels);
        tex_srv_.view_desc.Texture2D.MostDetailedMip = 0;

        CHECK_D3D(
            &ret_code,
            graphics_device->CreateShaderResourceView(
                scoped_pointer_get(resource_), &tex_srv_.view_desc,
                scoped_pointer_get_impl(tex_srv_.view)
            ));
        if (FAILED(ret_code)) {
            return false;
        }
    }

    if (res_bind_flags & D3D11_BIND_UNORDERED_ACCESS) {
        tex_uav_.view_desc.Format = format_;
        tex_uav_.view_desc.ViewDimension = uav_dimension_mapping[texture_type];
        tex_uav_.view_desc.Texture2D.MipSlice = 0;

        CHECK_D3D(
            &ret_code,
            graphics_device->CreateUnorderedAccessView(
                scoped_pointer_get(resource_), &tex_uav_.view_desc,
                scoped_pointer_get_impl(tex_uav_.view)
                ));
        if (FAILED(ret_code)) {
            return false;
        }
    }

    //
    // TODO add code for RTVs.
    return true;
}
