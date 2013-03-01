#include "v8/rendering/directx/internal/debug_helpers.hpp"
#include "v8/rendering/directx/renderer.hpp"
#include "v8/rendering/directx/blend_state.hpp"

namespace {

const D3D11_BLEND kBlendStatesMapping[] = {
    D3D11_BLEND_ZERO,
    D3D11_BLEND_ONE,
    D3D11_BLEND_SRC_COLOR,
    D3D11_BLEND_INV_SRC_COLOR,
    D3D11_BLEND_SRC_ALPHA,
    D3D11_BLEND_INV_SRC_ALPHA,
    D3D11_BLEND_DEST_ALPHA,
    D3D11_BLEND_INV_DEST_ALPHA,
    D3D11_BLEND_DEST_COLOR,
    D3D11_BLEND_INV_DEST_COLOR,
    D3D11_BLEND_SRC_ALPHA_SAT,
    D3D11_BLEND_BLEND_FACTOR,
    D3D11_BLEND_INV_BLEND_FACTOR,
    D3D11_BLEND_SRC1_COLOR,
    D3D11_BLEND_INV_SRC1_COLOR,
    D3D11_BLEND_SRC1_ALPHA,
    D3D11_BLEND_INV_SRC1_ALPHA
};

const D3D11_BLEND_OP kBlendOpMapping[] = {
    D3D11_BLEND_OP_ADD,
    D3D11_BLEND_OP_SUBTRACT,
    D3D11_BLEND_OP_REV_SUBTRACT,
    D3D11_BLEND_OP_MIN,
    D3D11_BLEND_OP_MAX
};

}

v8::directx::blendstate_traits::handle_type*
v8::directx::blendstate_traits::create_state(
    const blend_descriptor_t& bstate_desc, renderer* rsys
    ) {
    D3D11_BLEND_DESC blend_description;
    blend_description.AlphaToCoverageEnable = false;
    blend_description.IndependentBlendEnable = false;
    D3D11_RENDER_TARGET_BLEND_DESC* rtbd = blend_description.RenderTarget;

    rtbd->BlendEnable = bstate_desc.enable;
    rtbd->SrcBlend = kBlendStatesMapping[bstate_desc.src_blend];
    rtbd->DestBlend = kBlendStatesMapping[bstate_desc.dst_blend];
    rtbd->BlendOp = kBlendOpMapping[bstate_desc.blend_op_rgb];
    rtbd->SrcBlendAlpha = kBlendStatesMapping[bstate_desc.src_alpha_blend];
    rtbd->DestBlendAlpha = kBlendStatesMapping[bstate_desc.dst_alpha_blend];
    rtbd->BlendOpAlpha = kBlendOpMapping[bstate_desc.blend_op_alpha];
    rtbd->RenderTargetWriteMask = bstate_desc.target_write_mask;

    HRESULT ret_code;
    handle_type* state_handle = nullptr;
    ID3D11Device* graphics_device = rsys->internal_np_get_device();
    CHECK_D3D(
        &ret_code, graphics_device->CreateBlendState(&blend_description, &state_handle)
        );
    return state_handle;
}
