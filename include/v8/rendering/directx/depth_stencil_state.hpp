#pragma once

#include <D3D11.h>

#include <v8/v8.hpp>
#include <v8/rendering/directx/api_state.hpp>
#include <v8/rendering/directx/fwd_renderer.hpp>
#include <v8/rendering/directx/constants.hpp>

namespace v8 { namespace directx {

struct depthstencil_op_t {
    StencilOp::Type     sten_fail_op;
    StencilOp::Type     sten_pass_depth_fail_op;
    StencilOp::Type     sten_pass_depth_pass_op;
    CompareFunc::Type   stencil_func;

    depthstencil_op_t()  {
        sten_fail_op = StencilOp::Keep;
        sten_pass_depth_fail_op = StencilOp::Keep;
        sten_pass_depth_pass_op = StencilOp::Keep;
        stencil_func = CompareFunc::Always;
    }
};

inline bool operator==(
    const depthstencil_op_t& lhs, const depthstencil_op_t& rhs
    ) {
    if (lhs.stencil_func != rhs.stencil_func) {
        return false;
    }
    if (lhs.sten_fail_op != rhs.sten_fail_op) {
        return false;
    }
    if (lhs.sten_pass_depth_fail_op != rhs.sten_pass_depth_fail_op) {
        return false;
    }
    if (lhs.sten_pass_depth_pass_op != rhs.sten_pass_depth_pass_op) {
        return false;
    }
    return true;
}

inline bool operator!=(
    const depthstencil_op_t& lhs, const depthstencil_op_t& rhs
    ) {
    return !(lhs == rhs);
}

struct depthstencil_descriptor_t {
    v8_bool_t                       depth_enable;
    DepthWriteMask::Type            depth_write_mask;
    CompareFunc::Type               depth_func;
    v8_bool_t                       stencil_enable;
    v8_uint8_t                      stencil_read_mask;
    v8_uint8_t                      stencil_write_mask;
    depthstencil_op_t               front_face;
    depthstencil_op_t               back_face;

    depthstencil_descriptor_t()
        :       depth_enable(true)
            ,   depth_write_mask(DepthWriteMask::All)
            ,   depth_func(CompareFunc::Less)
            ,   stencil_enable(false)
            ,   stencil_read_mask(0xff)
            ,   stencil_write_mask(0xff)
            ,   front_face()
            ,   back_face()
    {
    }

};

inline bool operator==(
    const depthstencil_descriptor_t& lhs, const depthstencil_descriptor_t& rhs
    ) {
    if (lhs.back_face != rhs.back_face)
        return false;

    if (lhs.depth_enable != rhs.depth_enable)
        return false;

    if (lhs.depth_func != rhs.depth_func)
        return false;

    if (lhs.depth_write_mask != rhs.depth_write_mask)
        return false;

    if (lhs.front_face != rhs.front_face)
        return false;

    if (lhs.stencil_enable != rhs.stencil_enable)
        return false;

    if (lhs.stencil_read_mask != rhs.stencil_read_mask)
        return false;

    if (lhs.stencil_write_mask != rhs.stencil_write_mask)
        return false;

    return true;
}

inline bool operator!=(
    const depthstencil_descriptor_t& lhs, const depthstencil_descriptor_t& rhs
    ) {
    return !(lhs == rhs);
}

struct depthstencil_traits {
    typedef ID3D11DepthStencilState                             handle_type;
    typedef depthstencil_descriptor_t                           descriptor_type;

    static handle_type* create_state(
        const depthstencil_descriptor_t& descriptor, renderer* rsys
        );
};

typedef api_state<depthstencil_traits>                      depth_stencil_state;

} // namespace directx
} // namespace v8
