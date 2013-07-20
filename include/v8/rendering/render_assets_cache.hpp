#pragma once

#include <v8/v8.hpp>
#include <v8/base/scoped_pointer.hpp>
#include <v8/rendering/effect_info.hpp>
#include <v8/rendering/fwd_effect.hpp>
#include <v8/rendering/fwd_renderer.hpp>
#include <v8/rendering/fwd_texture.hpp>
#include <v8/rendering/constants.hpp>
#include <v8/utility/hash_string.hpp>

namespace v8 { namespace rendering {

// class simple_mesh;

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

    texture* get_texture(const char* tex_info);

/// @}

/// \name Effects.
/// @{

public :

    /*effect* get_effect(const effect_info_t&); */

/// @}

/// \name Meshes    
/// @{

    // simple_mesh* get_mesh(const hash_string& mesh_name);

/// @}    

private :
    struct implementation_details;

    v8::base::scoped_ptr<implementation_details>                        pimpl_;

private :
    NO_CC_ASSIGN(render_assets_cache);
};

} // namespace rendering
} // namespace v8
