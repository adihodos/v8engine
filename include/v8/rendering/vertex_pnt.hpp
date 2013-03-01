#pragma once

#include <v8/v8.hpp>
#include <v8/math/vector2.hpp>
#include <v8/math/vector3.hpp>

namespace v8 { namespace rendering {

//! \defgroup Vertex formats.
//! @{

//!
//! A vertex format, consisting of position and texture coordinates.
struct vertex_pnt {

    vertex_pnt() {}

    vertex_pnt(
        float px, float py, float pz, 
        float nx, float ny, float nz,
        float tu, float tv
        )
        :   position(px, py, pz),
            normal(nx, ny, nz),
            texcoord(tu, tv) {
    }

    vertex_pnt(
        const v8::math::vector3F& pos,
        const v8::math::vector3F& normal_vec,
        const v8::math::vector2F& texc
        )
        :   position(pos),
            normal(normal_vec),
            texcoord(texc) {
    }

    static const char* name() NOEXCEPT {
        return "vertex_pnt";
    }
    
    //! Position in space.
    v8::math::vector3F  position;

    //! Normal at vertex.
    v8::math::vector3F  normal;

    //! Texture coordinates.
    v8::math::vector2F  texcoord;
};

//! @}

} // namespace rendering
} // namespace v8
