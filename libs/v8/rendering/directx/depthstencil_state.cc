#include "v8/rendering/directx/internal/debug_helpers.hpp"
#include "v8/rendering/directx/depth_stencil_state.hpp"
#include "v8/rendering/directx/renderer.hpp"

namespace {

const D3D11_DEPTH_WRITE_MASK kDepthWriteMaskMapping[] = {
    D3D11_DEPTH_WRITE_MASK_ZERO,
    D3D11_DEPTH_WRITE_MASK_ALL
};

const D3D11_COMPARISON_FUNC kComparisonFuncMapping[] = {
    D3D11_COMPARISON_NEVER,
    D3D11_COMPARISON_LESS,
    D3D11_COMPARISON_EQUAL,
    D3D11_COMPARISON_LESS_EQUAL,
    D3D11_COMPARISON_GREATER,
    D3D11_COMPARISON_NOT_EQUAL,
    D3D11_COMPARISON_GREATER_EQUAL,
    D3D11_COMPARISON_ALWAYS
};

const D3D11_STENCIL_OP kStencilOpMapping[] = {
    D3D11_STENCIL_OP_KEEP,
    D3D11_STENCIL_OP_ZERO,
    D3D11_STENCIL_OP_REPLACE,
    D3D11_STENCIL_OP_INCR_SAT,
    D3D11_STENCIL_OP_DECR_SAT,
    D3D11_STENCIL_OP_INVERT,
    D3D11_STENCIL_OP_INCR,
    D3D11_STENCIL_OP_DECR 
};

inline D3D11_DEPTH_STENCILOP_DESC v8ds_to_d3d11ds(
    const v8::directx::depthstencil_op_t& ds_op_in
    ) {
    D3D11_DEPTH_STENCILOP_DESC ds_out = {
        kStencilOpMapping[ds_op_in.sten_fail_op],
        kStencilOpMapping[ds_op_in.sten_pass_depth_fail_op],
        kStencilOpMapping[ds_op_in.sten_pass_depth_pass_op],
        kComparisonFuncMapping[ds_op_in.stencil_func]
    };
    return ds_out;
}

} // anonymous namespace

v8::directx::depthstencil_traits::handle_type*
v8::directx::depthstencil_traits::create_state(
    const depthstencil_descriptor_t& descriptor, renderer* rsys
    ) {

    D3D11_DEPTH_STENCIL_DESC ds_description = {
        descriptor.depth_enable,
        kDepthWriteMaskMapping[descriptor.depth_write_mask],
        kComparisonFuncMapping[descriptor.depth_func],
        descriptor.stencil_enable,
        descriptor.stencil_read_mask,
        descriptor.stencil_write_mask,
        v8ds_to_d3d11ds(descriptor.front_face),
        v8ds_to_d3d11ds(descriptor.back_face)
    };

    handle_type* depthstencil_state = nullptr;
    HRESULT ret_code;
    ID3D11Device* device = rsys->internal_np_get_device();
    CHECK_D3D(
        &ret_code,
        device->CreateDepthStencilState(&ds_description, &depthstencil_state));
    return depthstencil_state;
}