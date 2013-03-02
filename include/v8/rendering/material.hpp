#pragma once

#include <v8/v8.hpp>
#include <v8/math/color.hpp>
#include <v8/utility/hash_string.hpp>
#include <v8/rendering/fwd_effect.hpp>
#include <v8/rendering/fwd_texture.hpp>

namespace v8 { namespace rendering {

///
/// \brief Abstract representation of a surface's color properties.
struct material {

/// \name Defined types.
/// @{

public :

    enum Type {
        Type_Unknown,
        Type_Color,
        Type_Texture
    };

    typedef float float4[4];

/// @}

/// \name Constructors/initializing.
/// @{

public :

    material() {
        memset(this, 0, sizeof(*this));
    }

    v8_bool_t initialize(const char* const material_name) {
        return initialize(hash_string(material_name));
    }

    v8_bool_t initialize(const v8::hash_string& material_file);

/// @}

/// \name Operations.
/// @{

public :

    static material default();

    /// \brief Bind this material to the specified effect, using the predefined
    /// semantics (see the implementation for the names).
    void bind_to_effect(effect* eff);

/// @}

/// \name Attributes.
/// @{

    union {
        /// \brief Material color data.
        struct color_data {
            float4         emissive;
            float4         ambient;
            float4         diffuse;
            float4         specular;
        } color;

        /// \brief Material texture maps.
        struct texture_data {
            texture*           emissive;
            texture*           ambient;
            texture*           diffuse;
            texture*           specular;
        } tex;
    };

    v8_int_t type; ///< Type of information (rgb color data or texture data).

/// @}
};

} // namespace rendering
} // namespace v8
