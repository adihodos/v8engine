#pragma once

#include <v8/v8.hpp>
#include <v8/base/scoped_pointer.hpp>
#include <v8/rendering/fwd_renderer.hpp>
#include <v8/rendering/fwd_render_assets_cache.hpp>
#include <v8/scene/fwd_scene_system.hpp>

namespace v8 { 

namespace math {
    class camera;
}    

namespace gui { 
    class basic_window;
}

}

namespace v8 {

class filesys;

class application_state {

public :

/// \name Construction.
/// @{

public :
    application_state();

    virtual ~application_state();

    virtual v8_bool_t initialize() = 0;

/// @}

public :

    rendering::renderer* render_sys() const {
        return base::scoped_pointer_get(render_sys_);
    }

    rendering::render_assets_cache* asset_cache() const {
        return base::scoped_pointer_get(asset_cache_);
    }

    filesys* file_sys() const {
        return base::scoped_pointer_get(file_sys_);
    }

    gui::basic_window* window() const {
        return base::scoped_pointer_get(window_);
    }

    scene::scene_system* scene() const {
        return base::scoped_pointer_get(scene_sys_);
    }

    math::camera* camera() const {
        return cam_ptr_;
    }

/// \name Data members.
/// @{

protected :

    //! Renderer.
    base::scoped_ptr<rendering::renderer>                       render_sys_;    

    //! Resource cache.
    base::scoped_ptr<rendering::render_assets_cache>            asset_cache_;

    //! Scene system.
    base::scoped_ptr<scene::scene_system>                       scene_sys_;
    //! File system config.
    base::scoped_ptr<filesys>                                   file_sys_;

    //! Application window.
    base::scoped_ptr<gui::basic_window>                         window_;

    v8::math::camera*                                           cam_ptr_;

/// @}

/// \name Disabled functions.
/// @{

private :
    NO_CC_ASSIGN(application_state);

/// @}

};

extern application_state*   state;

} // namespace v8
