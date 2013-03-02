#pragma once

#include <d3d11.h>
#include <v8/base/com_exclusive_pointer.hpp>

#include <v8/rendering/directx/constants.hpp>
#include <v8/rendering/directx/fwd_renderer.hpp>
#include <v8/base/operator_bool.hpp>

namespace v8 { namespace directx {

//struct textureDescriptor_t {
//    v8_uint32_t     texWidth;
//    v8_uint32_t     texHeight;
//    v8_uint32_t     texDepth;
//    v8_uint32_t     elementsPerPixel;
//    v8_int32_t      elementType;
//    v8_uint32_t     stageBindFlags;
//    v8_uint32_t     usageFlags;
//    v8_uint32_t     cpuFlags;
//    const void*     initialData;
//
//    textureDescriptor_t(
//        const v8_uint32_t width,
//        const v8_uint32_t height,
//        const v8_uint32_t depth = 1u,
//
//};

class texture {
/// \name Construction/init
/// @{

public :

    texture();

    texture(
        renderer* rsys, 
        const char* file_name, 
        const v8_uint32_t bind_flags
        );

    ~texture();

    //v8_bool_t Initialize(
    //    renderer* renderSys,
    //    v8_uint32_t texWidth,
    //    v8_uint32_t texHeight,
    //    v8_uint32_t texDepth,
    //    v8_uint32_t numElementsPerPixel,
    //    v8_uint32_t elementBitsCount,
    //    v8_uint32_t bindingFlags,
    //    v8_uint32_t usageFlags,
    //    v8_uint32_t cpuFlags,
    //    const void* initialData
    //    );

    v8_bool_t Initialize(
        renderer* rsys, 
        const char* file_name,
        const v8_uint32_t bind_flags
        );

/// @}

/// \name Sanity checking
/// @{

public :

    v8_bool_t CheckIfValid() const {
        return resource_ && tex_srv_.view;
    }

    operator int base::operator_bool::*() const {
        return CheckIfValid() ? &base::operator_bool::a_member : nullptr;
    }

/// @}

/// \name Attributes.
/// @{

public :

    ID3D11ShaderResourceView* GetSRV() const {
        assert(CheckIfValid());
        return v8::base::scoped_pointer_get(tex_srv_.view);
    }

    ID3D11UnorderedAccessView* GetUAV() const {
        assert(CheckIfValid());
        return v8::base::scoped_pointer_get(tex_uav_.view);
    }

    ID3D11RenderTargetView* GetRTV() const {
        assert(CheckIfValid());
        return v8::base::scoped_pointer_get(tex_rtv_.view);
    }

/// @}
    
/// \name Private helper types.
/// @{

private :

    struct texture_sr_view {
        texture_sr_view() : view(nullptr) {}

        v8::base::com_exclusive_pointer
        <
            ID3D11ShaderResourceView
        >::type                                     view;
        D3D11_SHADER_RESOURCE_VIEW_DESC             view_desc;
    };

    struct texture_ua_view {
        texture_ua_view() : view(nullptr) {}

        v8::base::com_exclusive_pointer
        <
            ID3D11UnorderedAccessView
        >::type                                     view;
        D3D11_UNORDERED_ACCESS_VIEW_DESC            view_desc;
    };

    struct texture_rt_view {
        texture_rt_view() : view(nullptr) {}

        v8::base::com_exclusive_pointer
        <
            ID3D11RenderTargetView
        >::type                                     view;
        D3D11_RENDER_TARGET_VIEW_DESC               view_desc;
    };

    typedef v8::base::com_exclusive_pointer
    <
        ID3D11Resource
    >::type                                 ScopedtextureResource_t;

/// @}

/// \name Data members.
/// @{

private :

    ScopedtextureResource_t                         resource_;
    texture_sr_view                                 tex_srv_;
    texture_ua_view                                 tex_uav_;
    texture_rt_view                                 tex_rtv_;
    v8_uint32_t                                     width_;
    v8_uint32_t                                     height_;
    v8_uint32_t                                     depth_;
    v8_uint32_t                                     array_size_;
    DXGI_FORMAT                                     format_;
    v8_uint32_t                                     flags_;

/// @}
    
private :
    NO_CC_ASSIGN(texture);
};

} // namespace directx
} // namespace v8

