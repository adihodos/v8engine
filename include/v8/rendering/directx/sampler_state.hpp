#pragma once

#include <D3D11.h>

#include <v8/v8.hpp>
#include <v8/math/color.hpp>
#include <v8/rendering/directx/api_state.hpp>
#include <v8/rendering/directx/fwd_renderer.hpp>
#include <v8/rendering/directx/constants.hpp>

namespace v8 { namespace directx {

struct sampler_descriptor_t {
    Filter::Type                    filter_type;
    Texture_Address_Mode::Type      tex_address_u;
    Texture_Address_Mode::Type      tex_address_v;
    Texture_Address_Mode::Type      tex_address_w;
    float                           mip_lod_bias;
    v8_uint32_t                     max_aniso_level;
    CompareFunc::Type               compare_func;
    v8::math::color_rgb             border_color;
    float                           max_lod;
    float                           min_lod;

    sampler_descriptor_t() {
        set_defaults();
    }
    
    void set_defaults() {
        filter_type = Filter::Min_Mag_Mip_Linear;
        tex_address_u = Texture_Address_Mode::Clamp;
        tex_address_v = Texture_Address_Mode::Clamp;
        tex_address_w = Texture_Address_Mode::Clamp;
        mip_lod_bias = 0.0f;
        max_aniso_level = 16U;
        compare_func = CompareFunc::Never;
        border_color = v8::math::color_rgb::C_Black;
        max_lod = std::numeric_limits<float>::max(),
        min_lod = std::numeric_limits<float>::min();
    }

};

inline bool operator==(
    const sampler_descriptor_t& lhs, const sampler_descriptor_t& rhs
    ) {
    if (lhs.filter_type != rhs.filter_type)
        return false;
        
    if (lhs.tex_address_u != rhs.tex_address_u)
        return false;
        
    if (lhs.tex_address_v != rhs.tex_address_v)
        return false;
        
    if (lhs.tex_address_w != rhs.tex_address_w)
        return false;
        
    return true;
}

inline bool operator!=(
    const sampler_descriptor_t& lhs, const sampler_descriptor_t& rhs
    ) NOEXCEPT {
    return !(lhs == rhs);
}

struct samplerstate_traits {
    typedef ID3D11SamplerState                                      handle_type;
    typedef sampler_descriptor_t                                descriptor_type;

    static handle_type* create_state(
        const sampler_descriptor_t& descriptor, renderer* rsys
        );
};

typedef api_state<samplerstate_traits>                          sampler_state;

} // namespace directx
} // namespace v8
