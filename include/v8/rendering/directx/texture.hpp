#pragma once

#include <d3d11.h>

#include <v8/base/com_exclusive_pointer.hpp>
#include <v8/base/operator_bool.hpp>
#include <v8/rendering/directx/constants.hpp>
#include <v8/rendering/directx/fwd_renderer.hpp>
#include <v8/rendering/texture_descriptor.hpp>

namespace v8 { namespace directx {   

class texture {

/// \name   Defined types.
/// @{

public :

    typedef ID3D11Texture1D*                    tex1D_native_handle_type;
    typedef ID3D11Texture2D*                    tex2D_native_handle_type;
    typedef ID3D11Texture3D*                    tex3D_native_handle_type;
    typedef ID3D11Resource*                     tex_generic_native_handle_type;

/// @}

/// \name   Construction, initialization.
/// @{

public :

    texture();

    texture(const v8::rendering::textureDescriptor_t&   tex_desc,
            const v8::directx::renderer&                rsys,
            const void**                                tex_data = nullptr);

    texture(const char*                    filename,
            const v8::directx::renderer&         rsys);

    v8_bool_t 
    initialize(const v8::rendering::textureDescriptor_t&     tex_desc,
               const v8::directx::renderer&                  rsys,
               const void**                                  tex_data = nullptr);

    v8_bool_t initialize(const char*                    filename,
                         const v8::directx::renderer&   rsys);

/// @}

/// \name   Sanity checking
/// @{

public :

    v8_bool_t is_valid() const {
        return resource_ != nullptr;
    }

    operator int v8::base::operator_bool::*() const {
        return is_valid() ? &v8::base::operator_bool::a_member : nullptr;
    }

/// @}

/// \name   Attributes
/// @{

public:

    v8_uint32_t width() const NOEXCEPT {
        assert(is_valid());
        return width_;
    }

    v8_uint32_t height() const NOEXCEPT {
        assert(is_valid());
        return height_;
    }

    v8_uint32_t array_size() const NOEXCEPT {
        assert(is_valid());
        return array_size_;
    }

    v8_bool_t is_array() const NOEXCEPT {
        assert(is_valid());
        return array_size_ > 1;
    }

    v8_uint32_t mip_levels() const NOEXCEPT {
        assert(is_valid());
        return mip_levels_;
    }

    tex_generic_native_handle_type handle() const NOEXCEPT {
        assert(is_valid());
        return v8::base::scoped_pointer_get(resource_);
    }

    v8_uint32_t format() const NOEXCEPT {
        assert(is_valid());
        return format_;
    }

    v8_uint32_t type() const NOEXCEPT {
        assert(is_valid());
        return type_;
    }

/// @}

/// \name   Private data types and members.
/// @{

private :

    typedef v8::base::com_exclusive_pointer
    <
        ID3D11Resource
    >::type                                             tex_resource_type_t;

    v8_bool_t 
    initialize_texture2D(const D3D11_TEXTURE2D_DESC&             desc_tex2D,
                         const void*                             data,
                         ID3D11Device*                           dev_context);

private :

    v8_uint32_t                                         width_;
    v8_uint32_t                                         height_;
    v8_uint32_t                                         array_size_;
    v8_uint32_t                                         type_;
    v8_uint32_t                                         pipeline_stage_bindings_;
    v8_uint32_t                                         mip_levels_;
    v8_uint32_t                                         format_;
    tex_resource_type_t                                 resource_;

/// @}

/// \name   Disabled functions
/// @{

private :

    NO_CC_ASSIGN(texture);

/// @}
};

} // namespace directx
} // namespace v8
