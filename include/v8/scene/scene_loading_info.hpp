#pragma once

#include <third_party/stlsoft/stlsoft/string/static_string.hpp>
#include <v8/v8.hpp>
#include <v8/math/light.hpp>

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

struct standard_entity_load_info_t {
    ///< Object name.
    stlsoft::basic_static_string<char, 64>          ent_name;
    ///< Configuration file.
    stlsoft::basic_static_string<char, 64>          ent_conf_file;
    ///< Entity type id.
    v8_int_t                                        ent_type;
    ///< Translation vector.
    float                                           ent_translate[3];
    ///< Rotation along the X, Y, Z axis (degrees).
    float                                           ent_orientation[3];
    ///< Scale factor.
    float                                           ent_scale;
};

} // namespace scene
} // namespace v8
