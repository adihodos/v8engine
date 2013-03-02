#include "v8/rendering/directx/internal/debug_helpers.hpp"
#include "v8/rendering/directx/renderer.hpp"
#include "v8/rendering/directx/rasterizer_state.hpp"

namespace {

const D3D11_CULL_MODE kCullModeMapping[] = {
    D3D11_CULL_NONE,
    D3D11_CULL_FRONT,
    D3D11_CULL_BACK
};

const D3D11_FILL_MODE kFillModeMapping[] = {
    D3D11_FILL_SOLID,
    D3D11_FILL_WIREFRAME
};

} // anonymous namespace

v8::directx::rasterizerstate_traits::handle_type* 
v8::directx::rasterizerstate_traits::create_state(
    const raster_descriptor_t& descriptor, renderer* rsys
    ) {
    D3D11_RASTERIZER_DESC raster_description = {
        kFillModeMapping[descriptor.fill_mode],
        kCullModeMapping[descriptor.cull_mode],
        descriptor.front_face_ccw,
        descriptor.depth_bias,
        descriptor.depth_bias_clamp,
        descriptor.slope_bias,
        descriptor.depth_clip,
        descriptor.scissor,
        descriptor.multisample,
        descriptor.antialias
    };

    handle_type* raster_state = nullptr;
    HRESULT ret_code;
    ID3D11Device* device = rsys->internal_np_get_device();
    CHECK_D3D(
        &ret_code,
        device->CreateRasterizerState(&raster_description, &raster_state));
    return raster_state;
}