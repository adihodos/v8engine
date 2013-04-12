#pragma once

#include <D3D11.h>

#include <v8/v8.hpp>
#include <v8/rendering/directx/api_state.hpp>
#include <v8/rendering/directx/fwd_renderer.hpp>
#include <v8/rendering/directx/constants.hpp>

namespace v8 { namespace directx {

struct raster_descriptor_t {

    FillMode::Type                  fill_mode;
    CullMode::Type                  cull_mode;
    v8_bool_t                       front_face_ccw;
    v8_int32_t                      depth_bias;
    float                           depth_bias_clamp;
    float                           slope_bias;
    v8_bool_t                       depth_clip;
    v8_bool_t                       scissor;
    v8_bool_t                       multisample;
    v8_bool_t                       antialias;

    static const raster_descriptor_t& default_state() {
        static const raster_descriptor_t def_state = {
            FillMode::Solid,
            CullMode::Back,
            false,
            0,
            0.0f,
            0.0f,
            true,
            false,
            false,
            false
        };
        return def_state;
    }
};

inline bool operator==(
    const raster_descriptor_t&  lhs, const raster_descriptor_t& rhs
    ) {
    if (lhs.fill_mode != rhs.fill_mode)
        return false;
        
    if (lhs.cull_mode != rhs.cull_mode)
        return false;
        
    if (lhs.front_face_ccw != rhs.front_face_ccw)
        return false;
        
    if (lhs.depth_bias != rhs.depth_bias)
        return false;
        
    if (lhs.depth_bias_clamp != rhs.depth_bias_clamp)
        return false;
        
    if (lhs.slope_bias != rhs.slope_bias)
        return false;
        
    if (lhs.depth_clip != rhs.depth_clip)
        return false;
        
    if (lhs.scissor != rhs.scissor)
        return false;
        
    if (lhs.multisample != rhs.multisample)
        return false;
        
    if (lhs.antialias != rhs.antialias)
        return false;
        
    return true;
}

inline bool operator!=(
    const raster_descriptor_t&  lhs, const raster_descriptor_t& rhs
    ) {
    return !(lhs == rhs);
}

struct rasterizerstate_traits {
    typedef ID3D11RasterizerState                                   handle_type;
    typedef raster_descriptor_t                                 descriptor_type;

    static handle_type* create_state(
        const raster_descriptor_t& descriptor, renderer* rsys
        );
};

typedef api_state<rasterizerstate_traits>                       rasterizer_state;

} // namespace directx
} // namespace v8
