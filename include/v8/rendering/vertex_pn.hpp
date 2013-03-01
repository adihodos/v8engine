#pragma once

#include <v8/v8.hpp>
#include <v8/math/vector2.hpp>
#include <v8/math/vector3.hpp>

namespace v8 { namespace rendering {

//! \defgroup Vertex formats.
//! @{

//!
//! A vertex format, consisting of position and normal vector.
struct vertex_pn {

    vertex_pn() {}

    vertex_pn(
        float px, float py, float pz, 
        float nx, float ny, float nz
        )
        :   position(px, py, pz),
            normal(nx, ny, nz)
    {
    }

    vertex_pn(
        const v8::math::vector3F& pos,
        const v8::math::vector3F& normal_vec
        )
        :   position(pos),
            normal(normal_vec) 
    {
    }

    static const char* name() NOEXCEPT {
        return "vertex_pn";
    }
    
    //! Position in space.
    v8::math::vector3F  position;

    //! Normal at vertex.
    v8::math::vector3F  normal;
};

//! @}

} // namespace rendering
} // namespace v8
