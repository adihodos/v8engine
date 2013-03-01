#pragma once

#include <v8/v8.hpp>
#include <v8/math/vector2.hpp>
#include <v8/math/vector3.hpp>

namespace v8 { namespace rendering {

//! \defgroup Vertex formats.
//! @{

//!
//! A vertex format, consisting of position and texture coordinates.
struct vertex_pt {

    vertex_pt() {}

    vertex_pt(float px, float py, float pz, float tu, float tv)
        :   position(px, py, pz),
            texcoord(tu, tv) {
    }

    vertex_pt(
        const v8::math::vector3F& pos,
        const v8::math::vector2F& texc
        )
        :   position(pos),
            texcoord(texc) {
    }

    static const char* name() NOEXCEPT {
        return "vertex_pt";
    }
    
    //! Position in space.
    v8::math::vector3F  position;

    //! Texture coordinates.
    v8::math::vector2F  texcoord;
};

//! @}

} // namespace rendering
} // namespace v8
