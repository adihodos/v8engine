#pragma once

#include <v8/v8.hpp>
#include <v8/base/scoped_pointer.hpp>

#include <v8/utility/hash_string.hpp>
#include <v8/rendering/fwd_renderer.hpp>
#include <v8/rendering/fwd_texture.hpp>
#include <v8/rendering/constants.hpp>


namespace v8 { namespace rendering {

struct effect_info_t;

struct material;

///
/// Creates, caches and manages rendering resources (textures, shaders, etc).
class render_assets_cache {

/// \name Constructors/destructors
/// @{

public :

    render_assets_cache(renderer* rsys);

    ~render_assets_cache();

/// @}

/// \name Texture and material management.
/// @{

public :

    texture* get_texture(const char* texture_file_name) {
        return get_texture(hash_string(texture_file_name));
    }

    texture* get_texture(const hash_string& texture_file_name);

    material* get_material(const char* material_name) {
        return get_material(hash_string(material_name));
    }

    material* get_material(const hash_string& material_name);

/// @}

private :
    struct implementation_details;

    v8::base::scoped_ptr<implementation_details>                        pimpl_;

private :
    NO_CC_ASSIGN(render_assets_cache);
};

} // namespace rendering
} // namespace v8
