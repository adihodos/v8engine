#pragma once

#include <v8/math/matrix4X4.hpp>
#include <v8/rendering/fwd_renderer.hpp>

namespace v8 { namespace math {
    class camera;
}}

struct draw_context_t {
    draw_context_t(
        v8::rendering::renderer* r_sys,
        const v8::math::matrix_4X4F& view_proj,
        const v8::math::camera& cam
        )
        :       context_renderer(r_sys)
            ,   context_view_proj(view_proj)
            ,   context_camera(cam)
    {}

    v8::rendering::renderer*            context_renderer;
    const v8::math::matrix_4X4F&        context_view_proj;
    const v8::math::camera&             context_camera;
};
