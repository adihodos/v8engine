#pragma once

#include <v8/v8.hpp>
#include <v8/math/color.hpp>
#include <v8/math/vector3.hpp>

namespace v8 { namespace rendering {

//! \name Vertex formats.
//! @{

//!
//! A vertex format, consisting of position and color.
struct vertex_pc {

    vertex_pc() {}

    vertex_pc(
        float px, float py, float pz,
        float red, float green, float blue, float alpha = 1.0f
        )
        :   position(px, py, pz),
            color(red, green, blue, alpha) {}

    vertex_pc(
        const v8::math::vector3F& pos,
        const v8::math::color_rgb& clr
        )
        :   position(pos),
            color(clr) {}

    //! Position in space.
    v8::math::vector3F  position;

    //! Color.
    v8::math::color_rgb color;
};

//! @}

} // namespace rendering
} // namespace v8
