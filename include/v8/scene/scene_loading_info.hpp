#pragma once

#include <third_party/stlsoft/stlsoft/string/static_string.hpp>
#include <v8/v8.hpp>
#include <v8/math/light.hpp>
#include <v8/rendering/material.hpp>

namespace v8 { namespace scene {

struct material_loading_info_t {
    ///< Unique name.
    stlsoft::basic_static_string<char, 64>          mat_name;
};

struct standard_object_loading_info_t {
    ///< Mesh name.
    stlsoft::basic_static_string<char, 64>          mesh;

    ///< Material name. If no material is specified, a default material is
    ///< attached.
    stlsoft::basic_static_string<char, 64>          material;

    ///< Translation vector.
    float                                           translate[3];

    ///< Rotation angles around the XYZ axes, expressed in radians.
    float                                           rotate[3];

    ///< Uniform scaling factor.
    float                                           scale;
};

struct userdefined_object_loading_info_t {
    ///< Object name.
    stlsoft::basic_static_string<char, 64>          name;

    ///< Configuration file name.
    stlsoft::basic_static_string<char, 64>          config_file;

    ///< Object type id.
    v8_int_t                                        type;
};

struct light_loading_info_t {
    light_loading_info_t()
        : state(false)
    {}

    v8::math::light                                 light;
    v8_bool_t                                       state;
};

} // namespace scene
} // namespace v8
