#include <third_party/stlsoft/platformstl/filesystem/path.hpp>
#include <third_party/stlsoft/platformstl/filesystem/memory_mapped_file.hpp>
#include <third_party/stlsoft/stlsoft/shims/access/string.hpp>
#include <third_party/directx_tex/DirectXTex.h>

#include "v8/base/pointer_policies.hpp"
#include "v8/base/shims/scoped_ptr.hpp"
#include "v8/io/filesystem.hpp"
#include "v8/rendering/directx/renderer.hpp"
#include "v8/rendering/directx/constants.hpp"
#include "v8/rendering/directx/internal/constants_mappings.hpp"
#include "v8/rendering/directx/internal/utilities.hpp"
#include "v8/rendering/directx/internal/dds_texture_loader.hpp"
#include "v8/rendering/directx/internal/debug_helpers.hpp"
#include "v8/utility/win_util.hpp"

#include "v8/rendering/directx/texture_info.hpp"
#include "v8/rendering/directx/texture.hpp"

#include "v8/rendering/directx/texture_shader_binding.hpp"

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

v8_bool_t 
tex_descriptor_to_tex2D_descriptor(
    const v8::rendering::textureDescriptor_t&   tex_desc,
    D3D11_TEXTURE2D_DESC*                       d3d_tex_desc)
{
    d3d_tex_desc->Width = tex_desc.texWidth;
    d3d_tex_desc->Height = tex_desc.texHeight;
    d3d_tex_desc->MipLevels = 1;
    d3d_tex_desc->ArraySize = tex_desc.arraySize;
    d3d_tex_desc->Format = v8::directx::element_type_to_dxgi_format(
        tex_desc.elementType, tex_desc.elementsPerTexel);
    d3d_tex_desc->SampleDesc.Count = 1;
    d3d_tex_desc->SampleDesc.Quality = 0;
    d3d_tex_desc->Usage = v8::directx::internal::kResourceUsageMappings[tex_desc.usageFlags];
    d3d_tex_desc->BindFlags = map_bindflags_to_directx_bindflags(tex_desc.pipelineStageBindFlags);
    d3d_tex_desc->CPUAccessFlags = v8::directx::internal::kCPUAccessMappings[tex_desc.cpuFlags];
    d3d_tex_desc->MiscFlags = 0;

    return true;
}

ID3D11Texture2D*
make_texture2D(const D3D11_TEXTURE2D_DESC&             tex_desc,
               ID3D11Device*                           device,
               const D3D11_SUBRESOURCE_DATA*           init_data)
{
    ID3D11Texture2D* tex2D = nullptr;
    HRESULT ret_code;
    CHECK_D3D(&ret_code,
              device->CreateTexture2D(&tex_desc, init_data, &tex2D));
    return tex2D;
}

v8::base::com_exclusive_pointer<ID3D11ShaderResourceView>::type
make_srv(ID3D11Resource*                            resource,
         ID3D11Device*                              device,
         const D3D11_SHADER_RESOURCE_VIEW_DESC&     srv_data) 
{
    using namespace v8::base;

    com_exclusive_pointer<ID3D11ShaderResourceView>::type tex_srv;
    HRESULT ret_code;

    CHECK_D3D(&ret_code,
              device->CreateShaderResourceView(resource,
                                               &srv_data,
                                               raw_ptr_ptr(tex_srv)));

    return tex_srv;
}

v8::base::com_exclusive_pointer<ID3D11UnorderedAccessView>::type
make_uav(ID3D11Resource*                            resource,
         ID3D11Device*                              device,
         const D3D11_UNORDERED_ACCESS_VIEW_DESC&    uav_data) 
{
    using namespace v8::base;

    com_exclusive_pointer<ID3D11UnorderedAccessView>::type tex_uav;
    HRESULT ret_code;

    CHECK_D3D(&ret_code,
              device->CreateUnorderedAccessView(resource,
                                                &uav_data,
                                                raw_ptr_ptr(tex_uav)));

    return tex_uav;
}

} // anonymous namespace


//v8::directx::texture::texture()
//    :       width_(0)
//        ,   height_(0)
//        ,   depth_(0)
//        ,   array_size_(0)
//        ,   format_(DXGI_FORMAT_UNKNOWN)
//        ,   flags_(0)
//{}
//
//v8::directx::texture::
//texture(const v8::rendering::textureDescriptor_t&       tex_info,
//        v8::directx::renderer*                          rsys,
//        const void**                                    data)
//    :       width_(0)
//        ,   height_(0)
//        ,   type_(v8::rendering::textureType_t::Tex2D)
//        ,   array_size_(0)
//        ,   format_(DXGI_FORMAT_UNKNOWN)
//        ,   flags_(0)
//{
//    initialize(tex_info, rsys, data);
//}
//
//v8::directx::texture::texture(const char*               filename,
//                              v8::directx::renderer*    rsys)
//    :       width_(0)
//        ,   height_(0)
//        ,   type_(v8::rendering::textureType_t::Tex2D)
//        ,   array_size_(0)
//        ,   format_(DXGI_FORMAT_UNKNOWN)
//        ,   flags_(0)
//{
//    initialize(filename, rsys);
//}
//
//v8::directx::texture::~texture() {}
//
//v8_bool_t 
//v8::directx::texture::initialize(const char*    filename,
//                                 renderer*      rsys)
//{
//    if (resource_) {
//        return true;
//    }
//
//    //
//    // Determine if we're dealing with a DDS file.
//    const char* end_ptr = filename + strlen(filename);
//    auto itr_dot = std::find(filename, end_ptr, '.');
//
//    bool is_dds = (itr_dot != end_ptr) && !strcmp(itr_dot, ".dds");
//
//    utility::win32::scoped_wide_string_t path_widestr(
//        utility::win32::multibyte_string_to_wide_string(filename));
//
//    HRESULT ret_code;
//    DirectX::TexMetadata tex_metadata;
//
//    platformstl::memory_mapped_file mmtexfile(filename);
//
//    using namespace v8::base;
//
//    //
//    // Get texture metadata
//    if (is_dds) {
//        ret_code = DirectX::GetMetadataFromDDSMemory(mmtexfile.memory(),
//                                                     mmtexfile.size(),
//                                                     0,
//                                                     tex_metadata);
//    } else {
//        ret_code = DirectX::GetMetadataFromWICMemory(mmtexfile.memory(),
//                                                     mmtexfile.size(),
//                                                     0,
//                                                     tex_metadata);
//    }
//
//    if (FAILED(ret_code)) {
//        return false;
//    }
//
//    DirectX::ScratchImage scratch_img;
//
//    if (is_dds) {
//        ret_code = DirectX::LoadFromDDSMemory(mmtexfile.memory(),
//                                              mmtexfile.size(),
//                                              0,
//                                              &tex_metadata,
//                                              scratch_img);
//    } else {
//        ret_code = DirectX::LoadFromWICMemory(mmtexfile.memory(),
//                                              mmtexfile.size(),
//                                              0,
//                                              &tex_metadata,
//                                              scratch_img);
//    }
//
//    if (FAILED(ret_code)) {
//        return false;
//    }
//
//    ret_code = DirectX::CreateTexture(rsys->internal_np_get_device(),
//                                      scratch_img.GetImages(),
//                                      scratch_img.GetImageCount(),
//                                      tex_metadata,
//                                      raw_ptr_ptr(resource_));
//
//    if (FAILED(ret_code)) {
//        return false;
//    }
//
//    ret_code = DirectX::CreateShaderResourceView(rsys->internal_np_get_device(),
//                                                 scratch_img.GetImages(),
//                                                 scratch_img.GetImageCount(),
//                                                 tex_metadata,
//                                                 raw_ptr_ptr(tex_srv_.view));
//
//    if (FAILED(ret_code)) {
//        return false;
//    }
//
//    if (tex_metadata.IsCubemap()) {
//        type_ = rendering::textureType_t::TexCube;
//    } else if (tex_metadata.depth == 0) {
//        type_ = rendering::textureType_t::Tex1D;
//    } else if (tex_metadata.depth == 1) {
//        type_ = rendering::textureType_t::Tex2D;
//    } else if (tex_metadata.depth == 2) {
//        type_ = rendering::textureType_t::Tex3D;
//    } else {
//        return false;
//    }
//
//    flags_ = rendering::BindingFlag::ShaderResource;
//    array_size_ = tex_metadata.arraySize;
//    return true;    
//}
//
//v8_bool_t
//v8::directx::texture::
//initialize_texture2D(const D3D11_TEXTURE2D_DESC&             desc_tex2D,
//                     const void*                             data,
//                     ID3D11Device*                           dev_context)
//{
//    using namespace v8::base;
//
//    if (data) {
//
//        v8_size_t row_pitch;
//        v8_size_t depth_pitch;
//
//        DirectX::ComputePitch(desc_tex2D.Format, 
//                              desc_tex2D.Width,
//                              desc_tex2D.Height,
//                              row_pitch,
//                              depth_pitch);
//
//        DirectX::Image image = { 
//            desc_tex2D.Width, 
//            desc_tex2D.Height, 
//            desc_tex2D.Format,
//            row_pitch,
//            depth_pitch,
//            static_cast<uint8_t*>(const_cast<void*>(data))
//        };
//
//        DirectX::TexMetadata tex_metadata = {
//            desc_tex2D.Width,
//            desc_tex2D.Height,
//            1,
//            desc_tex2D.ArraySize,
//            desc_tex2D.MipLevels,
//            0,
//            0,
//            desc_tex2D.Format,
//            DirectX::TEX_DIMENSION_TEXTURE2D
//        };
//
//        HRESULT ret_code = DirectX::CreateTextureEx(dev_context,
//                                                    &image,
//                                                    1u,
//                                                    tex_metadata,
//                                                    desc_tex2D.Usage,
//                                                    desc_tex2D.BindFlags,
//                                                    desc_tex2D.CPUAccessFlags,
//                                                    desc_tex2D.MiscFlags,
//                                                    false,
//                                                    raw_ptr_ptr(resource_));
//    } else {
//        resource_ = make_texture2D(desc_tex2D, 
//                                   dev_context, 
//                                   nullptr);
//    }
//
//    if (!resource_) {
//        return false;
//    }
//
//    //
//    // Create SRV
//    if (desc_tex2D.BindFlags & D3D11_BIND_SHADER_RESOURCE) {
//        tex_srv_.view_desc.Format = desc_tex2D.Format;
//        tex_srv_.view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
//        tex_srv_.view_desc.Texture2D.MostDetailedMip = 0;
//        tex_srv_.view_desc.Texture2D.MipLevels = desc_tex2D.MipLevels;
//
//        tex_srv_.view = make_srv(v8::base::raw_ptr(resource_),
//                                 dev_context,
//                                 tex_srv_.view_desc);
//
//        if (!tex_srv_.view) {
//            return false;
//        }
//    }
//
//    //
//    // Create UAV
//    if (desc_tex2D.BindFlags & D3D11_BIND_UNORDERED_ACCESS) {
//        tex_uav_.view_desc.Format = desc_tex2D.Format;
//        tex_uav_.view_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
//        tex_uav_.view_desc.Texture2D.MipSlice = 0;
//
//        tex_uav_.view = make_uav(v8::base::raw_ptr(resource_),
//                                 dev_context,
//                                 tex_uav_.view_desc);
//
//        if (!tex_uav_.view) {
//            return false;
//        }
//    }
//
//    //
//    // TODO : add code to Create RTV
//    return true;
//}
//
//v8_bool_t 
//v8::directx::texture::
//initialize(const v8::rendering::textureDescriptor_t&     tex_desc,
//           renderer*                                     rsys,
//           const void**                                  tex_data)
//{
//    DirectX::TexMetadata                metadata;
//    v8_bool_t                           metadata_init = false;
//    std::vector<DirectX::ScratchImage>  scratch_images(tex_desc.arraySize);
//    size_t row_pitch;
//    size_t slice_pitch;
//
//    if (tex_data) {
//        for (v8_uint32_t idx = 0; idx < tex_desc.arraySize; ++idx) {
//
//            if (!metadata_init) {
//                metadata.arraySize = tex_desc.arraySize;
//
//                if (tex_desc.texType == rendering::textureType_t::Tex1D) {
//                    metadata.depth = 0;
//                    metadata.dimension = DirectX::TEX_DIMENSION_TEXTURE1D;
//                } else if (tex_desc.texType == rendering::textureType_t::Tex2D) {
//                    metadata.depth = 1;
//                    metadata.dimension = DirectX::TEX_DIMENSION_TEXTURE2D;
//                } else if (tex_desc.texType == rendering::textureType_t::Tex3D) {
//                    metadata.depth = 2;
//                    metadata.dimension = DirectX::TEX_DIMENSION_TEXTURE3D;
//                } else if (tex_desc.texType == rendering::textureType_t::TexCube) {
//                    assert(false);
//                    return false;
//                } else {
//                    return false;
//                }
//
//                metadata.format = element_type_to_dxgi_format(tex_desc.elementType,
//                                                              tex_desc.elementsPerTexel);
//                metadata.height = tex_desc.texHeight;
//                metadata.width = tex_desc.texWidth;
//                metadata.miscFlags = metadata.miscFlags2 = 0;
//
//                DirectX::ComputePitch(metadata.format, 
//                                      metadata.width, 
//                                      metadata.height, 
//                                      row_pitch, 
//                                      slice_pitch);
//
//                metadata_init = true;
//            }
//
//            DirectX::Image base_img = {
//                tex_desc.texWidth,
//                tex_desc.texHeight,
//                metadata.format,
//                row_pitch,
//                slice_pitch,
//                static_cast<uint8_t*>(const_cast<void*>(tex_data[idx]))
//            };
//
//            HRESULT ret_code;
//
//            if (tex_desc.generateMips > 1) {
//                ret_code = DirectX::GenerateMipMaps(base_img, 
//                                                    DirectX::TEX_FILTER_DEFAULT, 
//                                                    0, 
//                                                    scratch_images[idx]); 
//            } else {
//                ret_code = scratch_images[idx].InitializeFromImage(base_img);
//            }
//
//            if (FAILED(ret_code)) {
//                return false;
//            }
//        }
//
//        std::vector<DirectX::Image*>         images;
//        for (v8_size_t idx = 0; idx < scratch_images.size(); ++idx) {
//            images.insert(std::end(images), 
//                          scratch_images[idx].GetImages(),
//                          scratch_images[idx].GetImages() + scratch_images[idx].GetImageCount());
//        }
//
//        HRESULT ret_code = DirectX::CreateTextureEx(rsys->internal_np_get_device(),
//                                                    *&images[0],
//                                                    images.size(),
//                                                    metadata,
//                                                    internal::kResourceUsageMappings[tex_desc.usageFlags],
//                                                    map_bindflags_to_directx_bindflags(tex_desc.pipelineStageBindFlags),
//                                                    internal::kCPUAccessMappings[tex_desc.cpuFlags],
//                                                    0,
//                                                    false,
//                                                    v8::base::raw_ptr_ptr(resource_));
//
//        if (FAILED(ret_code)) {
//            return false;
//        }
//
//        format_ = metadata.format;
//    } else {
//        //
//        // Make texture without any initial data
//        format_ = element_type_to_dxgi_format(tex_desc.elementType,
//                                              tex_desc.elementsPerTexel);
//
//        if (tex_desc.texType == rendering::textureType_t::Tex1D) {
//            //
//            // 1D texture
//        } else if (tex_desc.texType == rendering::textureType_t::Tex2D) {
//            //
//            // 2D texture
//            D3D11_TEXTURE2D_DESC    desc_tex2D;
//
//            if (!tex_descriptor_to_tex2D_descriptor(tex_desc, &desc_tex2D)) {
//                return false;
//            }
//
//            if (!initialize_texture2D(desc_tex2D, nullptr, rsys->internal_np_get_device())) {
//                return false;
//            }
//
//        } else if (tex_desc.texType == rendering::textureType_t::Tex2D) {
//            //
//            // 3D texture
//        } else {
//            //
//            //
//            return false;
//        }
//    }
//
//    flags_ = tex_desc.pipelineStageBindFlags;
//
//    //
//    // make views
//    return make_resource_views(rsys->internal_np_get_device(), 1);
//}
//
//v8_bool_t
//v8::directx::texture::make_resource_views(ID3D11Device*         device,
//                                          const v8_uint32_t     mip_levels,
//                                          const v8_uint32_t     most_detailed_mip) {
//    assert(!tex_srv_.view);
//    assert(!tex_uav_.view);
//    assert(!tex_rtv_.view);
//
//    using namespace v8::base;
//
//    if (flags_ & rendering::BindingFlag::ShaderResource) {
//        D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
//        srv_desc.Format = static_cast<DXGI_FORMAT>(format_);
//
//        if (type_ == rendering::textureType_t::Tex1D) {
//
//        } else if (type_ == rendering::textureType_t::Tex2D) {
//
//            if (array_size_ > 1) {
//
//                srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
//                srv_desc.Texture2DArray.ArraySize = array_size_;
//                srv_desc.Texture2DArray.FirstArraySlice = 0;
//                srv_desc.Texture2DArray.MipLevels = mip_levels;
//                srv_desc.Texture2DArray.MostDetailedMip = most_detailed_mip;
//
//            } else {
//
//                srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
//                srv_desc.Texture2D.MipLevels = mip_levels;
//                srv_desc.Texture2D.MostDetailedMip = most_detailed_mip;
//
//            }
//
//            tex_srv_.view = make_srv(raw_ptr(resource_),
//                                     device,
//                                     srv_desc);
//            if (!tex_srv_.view) {
//                return false;
//            }
//
//        } else if (type_ == rendering::textureType_t::Tex3D) {
//
//        } else {
//        }
//    }
//
//    if (flags_ & rendering::BindingFlag::UnorderedAccess) {
//
//        D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;
//        uav_desc.Format = static_cast<DXGI_FORMAT>(format_);
//
//        if (type_ == rendering::textureType_t::Tex1D) {
//            assert(false);
//        } else if (type_ == rendering::textureType_t::Tex2D) {
//
//            if (array_size_ > 1) {
//                uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
//                uav_desc.Texture2DArray.ArraySize = array_size_;
//                uav_desc.Texture2DArray.FirstArraySlice = 0;
//                uav_desc.Texture2DArray.MipSlice = 0;
//            } else {
//                uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
//                uav_desc.Texture2D.MipSlice = 0;
//            }
//
//            tex_uav_.view = make_uav(raw_ptr(resource_),
//                                     device,
//                                     uav_desc);
//
//            if (!tex_uav_.view) {
//                return false;
//            }
//
//        } else if (type_ == rendering::textureType_t::Tex3D) {
//            assert(false);
//        } else {
//            assert(false);
//        }
//    }
//
//    if (flags_ & rendering::BindingFlag::RenderTarget) {
//        assert(false);
//    }
//
//    return true;
//}

    //v8_uint32_t                                         width_;
    //v8_uint32_t                                         height_;
    //v8_uint32_t                                         array_size_;
    //v8_uint32_t                                         pipeline_stage_bindings_;
    //v8_uint32_t                                         mip_levels_;
    //v8_uint32_t                                         format_;
    //tex_resource_type_t                                 resource_;

v8::directx::texture::texture()
    :       width_(0)
        ,   height_(0)
        ,   array_size_(1)
        ,   type_(v8::rendering::textureType_t::Tex2D)
        ,   pipeline_stage_bindings_(v8::rendering::BindingFlag::ShaderResource)
        ,   mip_levels_(1)
        ,   format_(DXGI_FORMAT_UNKNOWN)
        ,   resource_() 
{}

v8::directx::texture::texture(const char*                    filename,
                              const v8::directx::renderer&   rsys) {
    initialize(filename, rsys);
    
}


v8::directx::texture::
texture(const v8::rendering::textureDescriptor_t&   tex_desc,
        const v8::directx::renderer&                rsys,
        const void**                                tex_data) {
    initialize(tex_desc, rsys, tex_data);
}

v8_bool_t 
v8::directx::texture::
initialize(const v8::rendering::textureDescriptor_t&     tex_desc,
           const v8::directx::renderer&                  rsys,
           const void**                                  tex_data)
{
    if (resource_) {
        return true;
    }

    using namespace v8::base;

    width_ = tex_desc.texWidth;
    height_ = tex_desc.texHeight;
    array_size_ = tex_desc.arraySize;
    type_ = tex_desc.texType;
    pipeline_stage_bindings_ = tex_desc.pipelineStageBindFlags;
    format_ = element_type_to_dxgi_format(tex_desc.elementType, 
                                          tex_desc.elementsPerTexel);
    mip_levels_ = 1;

    DirectX::TexMetadata                metadata;
    v8_bool_t                           metadata_init = false;
    scoped_ptr<DirectX::ScratchImage, default_array_storage> scratch_images(
        new DirectX::ScratchImage[tex_desc.arraySize]);

    size_t row_pitch;
    size_t slice_pitch;

    if (tex_data) {
        for (v8_uint32_t idx = 0; idx < tex_desc.arraySize; ++idx) {

            if (!metadata_init) {
                metadata.arraySize = tex_desc.arraySize;

                if (tex_desc.texType == rendering::textureType_t::Tex1D) {
                    metadata.depth = 0;
                    metadata.dimension = DirectX::TEX_DIMENSION_TEXTURE1D;
                } else if (tex_desc.texType == rendering::textureType_t::Tex2D) {
                    metadata.depth = 1;
                    metadata.dimension = DirectX::TEX_DIMENSION_TEXTURE2D;
                } else if (tex_desc.texType == rendering::textureType_t::Tex3D) {
                    metadata.depth = 2;
                    metadata.dimension = DirectX::TEX_DIMENSION_TEXTURE3D;
                } else if (tex_desc.texType == rendering::textureType_t::TexCube) {
                    assert(false);
                    return false;
                } else {
                    return false;
                }

                metadata.format = element_type_to_dxgi_format(tex_desc.elementType,
                                                              tex_desc.elementsPerTexel);
                metadata.height = tex_desc.texHeight;
                metadata.width = tex_desc.texWidth;
                metadata.miscFlags = metadata.miscFlags2 = 0;

                DirectX::ComputePitch(metadata.format, 
                                      metadata.width, 
                                      metadata.height, 
                                      row_pitch, 
                                      slice_pitch);

                metadata_init = true;
            }

            DirectX::Image base_img = {
                tex_desc.texWidth,
                tex_desc.texHeight,
                metadata.format,
                row_pitch,
                slice_pitch,
                static_cast<uint8_t*>(const_cast<void*>(tex_data[idx]))
            };

            HRESULT ret_code;

            if (tex_desc.generateMips) {
                ret_code = DirectX::GenerateMipMaps(base_img, 
                                                    DirectX::TEX_FILTER_DEFAULT, 
                                                    0, 
                                                    scratch_images[idx]); 
            } else {
                ret_code = scratch_images[idx].InitializeFromImage(base_img);
            }

            if (FAILED(ret_code)) {
                return false;
            }
        }

        std::vector<const DirectX::Image*>  images;

        for (v8_size_t idx = 0; idx < tex_desc.arraySize; ++idx) {
            const DirectX::Image* img = scratch_images[idx].GetImages();
            v8_size_t offset = 0;

            std::generate_n(std::back_inserter(images), 
                            scratch_images[idx].GetImageCount(),
                            [img, &offset]() -> const DirectX::Image* {
                const DirectX::Image* next_img = img + offset;
                ++offset;
                return next_img;
            });
        }

        HRESULT ret_code = DirectX::CreateTextureEx(rsys.internal_np_get_device(),
                                                    *&images[0],
                                                    images.size(),
                                                    metadata,
                                                    internal::kResourceUsageMappings[tex_desc.usageFlags],
                                                    map_bindflags_to_directx_bindflags(tex_desc.pipelineStageBindFlags),
                                                    internal::kCPUAccessMappings[tex_desc.cpuFlags],
                                                    0,
                                                    false,
                                                    v8::base::raw_ptr_ptr(resource_));

        if (FAILED(ret_code)) {
            return false;
        }

        //
        // Update mip levels
        mip_levels_ = static_cast<v8_uint32_t>(metadata.mipLevels);
    } else {

        if (tex_desc.texType == rendering::textureType_t::Tex1D) {
            //
            // 1D texture
        } else if (tex_desc.texType == rendering::textureType_t::Tex2D) {
            //
            // 2D texture
            D3D11_TEXTURE2D_DESC    desc_tex2D;

            if (!tex_descriptor_to_tex2D_descriptor(tex_desc, &desc_tex2D)) {
                return false;
            }

            if (!initialize_texture2D(desc_tex2D, nullptr, rsys.internal_np_get_device())) {
                return false;
            }

        } else if (tex_desc.texType == rendering::textureType_t::Tex3D) {
            //
            // 3D texture
        } else {
            //
            //
            return false;
        }
    }

    return true;
}

v8_bool_t 
v8::directx::texture::initialize(const char*                    filename,
                                 const v8::directx::renderer&   rsys) {
    if (resource_) {
        return true;
    }

    //
    // Determine if we're dealing with a DDS file.
    const char* end_ptr = filename + strlen(filename);
    auto itr_dot = std::find(filename, end_ptr, '.');

    bool is_dds = (itr_dot != end_ptr) && !strcmp(itr_dot, ".dds");

    utility::win32::scoped_wide_string_t path_widestr(
        utility::win32::multibyte_string_to_wide_string(filename));

    HRESULT ret_code;
    DirectX::TexMetadata tex_metadata;

    platformstl::memory_mapped_file mmtexfile(filename);

    using namespace v8::base;

    //
    // Get texture metadata
    if (is_dds) {
        ret_code = DirectX::GetMetadataFromDDSMemory(mmtexfile.memory(),
                                                     mmtexfile.size(),
                                                     0,
                                                     tex_metadata);
    } else {
        ret_code = DirectX::GetMetadataFromWICMemory(mmtexfile.memory(),
                                                     mmtexfile.size(),
                                                     0,
                                                     tex_metadata);
    }

    if (FAILED(ret_code)) {
        return false;
    }

    DirectX::ScratchImage scratch_img;

    if (is_dds) {
        ret_code = DirectX::LoadFromDDSMemory(mmtexfile.memory(),
                                              mmtexfile.size(),
                                              0,
                                              &tex_metadata,
                                              scratch_img);
    } else {
        ret_code = DirectX::LoadFromWICMemory(mmtexfile.memory(),
                                              mmtexfile.size(),
                                              0,
                                              &tex_metadata,
                                              scratch_img);
    }

    if (FAILED(ret_code)) {
        return false;
    }

    ret_code = DirectX::CreateTexture(rsys.internal_np_get_device(),
                                      scratch_img.GetImages(),
                                      scratch_img.GetImageCount(),
                                      tex_metadata,
                                      raw_ptr_ptr(resource_));

    if (FAILED(ret_code)) {
        return false;
    }

    //
    // Update texture data.

    width_ = static_cast<v8_uint32_t>(tex_metadata.width);
    height_ = static_cast<v8_uint32_t>(tex_metadata.height);
    array_size_ = static_cast<v8_uint32_t>(tex_metadata.arraySize);

    if (tex_metadata.IsCubemap()) {
        type_ = rendering::textureType_t::TexCube;
    } else if (tex_metadata.depth == 0) {
        type_ = rendering::textureType_t::Tex1D;
    } else if (tex_metadata.depth == 1) {
        type_ = rendering::textureType_t::Tex2D;
    } else if (tex_metadata.depth == 2) {
        type_ = rendering::textureType_t::Tex3D;
    } else {
        return false;
    }

    pipeline_stage_bindings_ = rendering::BindingFlag::ShaderResource;
    mip_levels_ = static_cast<v8_uint32_t>(tex_metadata.mipLevels);
    format_ = tex_metadata.format;

    return true;    
}

v8_bool_t
v8::directx::texture::
initialize_texture2D(const D3D11_TEXTURE2D_DESC&             desc_tex2D,
                     const void*                             data,
                     ID3D11Device*                           dev_context)
{
    using namespace v8::base;

    if (data) {
        v8_size_t row_pitch;
        v8_size_t depth_pitch;

        DirectX::ComputePitch(desc_tex2D.Format, 
                              desc_tex2D.Width,
                              desc_tex2D.Height,
                              row_pitch,
                              depth_pitch);

        DirectX::Image image = { 
            desc_tex2D.Width, 
            desc_tex2D.Height, 
            desc_tex2D.Format,
            row_pitch,
            depth_pitch,
            static_cast<uint8_t*>(const_cast<void*>(data))
        };

        DirectX::TexMetadata tex_metadata = {
            desc_tex2D.Width,
            desc_tex2D.Height,
            1,
            desc_tex2D.ArraySize,
            desc_tex2D.MipLevels,
            0,
            0,
            desc_tex2D.Format,
            DirectX::TEX_DIMENSION_TEXTURE2D
        };

        DirectX::CreateTextureEx(dev_context,
                                 &image,
                                 1u,
                                 tex_metadata,
                                 desc_tex2D.Usage,
                                 desc_tex2D.BindFlags,
                                 desc_tex2D.CPUAccessFlags,
                                 desc_tex2D.MiscFlags,
                                 false,
                                 raw_ptr_ptr(resource_));
    } else {
        resource_ = make_texture2D(desc_tex2D, 
                                   dev_context, 
                                   nullptr);
    }

    return resource_ != nullptr;
}