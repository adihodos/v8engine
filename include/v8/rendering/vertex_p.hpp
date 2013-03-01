#pragma once

#include <v8/v8.hpp>
#include <v8/math/vector3.hpp>

namespace v8 { namespace rendering {

//! \name Vertex formats.
//! @{

//!
//! A vertex format, consisting of position.
struct vertex_p {

    vertex_p() {}

    vertex_p(
        float px, float py, float pz
        )
        :   position(px, py, pz) {}

    vertex_p(
        const v8::math::vector3F& pos
        )
        :   position(pos) {}

    //! Position in space.
    v8::math::vector3F  position;
};

//! @}

} // namespace rendering
} // namespace v8
