#pragma once

#include <D3D11.h>

#include <v8/v8.hpp>
#include <v8/rendering/directx/api_state.hpp>
#include <v8/rendering/directx/fwd_renderer.hpp>
#include <v8/rendering/directx/constants.hpp>

namespace v8 { namespace directx {

struct blend_descriptor_t {
    v8_bool_t               enable; //!< Set to true to enable blending
    Blend_Factor::Type      src_blend; //!< Source pixel blend factor
    Blend_Factor::Type      dst_blend; //!< Destination pixel blend factor.
    Blend_Op::Type          blend_op_rgb; //!< Blend operation (RGB component).
    Blend_Factor::Type      src_alpha_blend; //!< Source pixel alpha blend factor.
    Blend_Factor::Type      dst_alpha_blend; //!< Dest pixel alpha blend factor.
    Blend_Op::Type          blend_op_alpha; //!< Blend operator (alpha channel).
    v8_uint8_t              target_write_mask; //!< Controls channels that get written.

    static const blend_descriptor_t& default() {
        static const blend_descriptor_t def_state = {
            true,
            Blend_Factor::One,
            Blend_Factor::Zero,
            Blend_Op::Add,
            Blend_Factor::One,
            Blend_Factor::Zero,
            Blend_Op::Add,
            Color_Write_Flag::All_Channels
        };

        return def_state;
    }
};

inline bool operator<(
    const blend_descriptor_t& lhs,
    const blend_descriptor_t& rhs
    ) NOEXCEPT {
        if (lhs.src_blend < rhs.src_blend)
            return true;

        if (lhs.dst_blend < rhs.dst_blend)
            return true;

        if (lhs.blend_op_rgb < rhs.blend_op_rgb)
            return true;

        if (lhs.blend_op_alpha < rhs.blend_op_alpha)
            return true;

        if (lhs.src_alpha_blend < rhs.src_alpha_blend)
            return true;

        if (lhs.dst_alpha_blend < rhs.dst_alpha_blend)
            return true;

        if (lhs.target_write_mask < rhs.target_write_mask)
            return true;

        return false;
}

inline bool operator==(
    const blend_descriptor_t& lhs,
    const blend_descriptor_t& rhs
    ) NOEXCEPT {
    if (lhs.src_blend != rhs.src_blend)
        return false;

    if (lhs.dst_blend != rhs.dst_blend)
        return false;

    if (lhs.blend_op_rgb != rhs.blend_op_rgb)
        return false;

    if (lhs.blend_op_alpha != rhs.blend_op_alpha)
        return false;

    if (lhs.src_alpha_blend != rhs.src_alpha_blend)
        return false;

    if (lhs.dst_alpha_blend != rhs.dst_alpha_blend)
        return false;

    if (lhs.target_write_mask != rhs.target_write_mask)
        return false;

    return true;
}

inline bool operator!=(
    const blend_descriptor_t& lhs,
    const blend_descriptor_t& rhs
    ) NOEXCEPT {
    return !(lhs == rhs);
}

struct blendstate_traits {
    typedef ID3D11BlendState                                handle_type;
    typedef blend_descriptor_t                              descriptor_type;

    static handle_type* create_state(
        const blend_descriptor_t& bsd, renderer* rsys
        );
};

typedef api_state<blendstate_traits>                                BlendState_t;

} // namespace directx
} // namespace v8
