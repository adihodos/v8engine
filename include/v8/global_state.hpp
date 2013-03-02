#pragma once

#include <v8/v8.hpp>
#include <v8/base/scoped_pointer.hpp>
#include <v8/rendering/fwd_renderer.hpp>
#include <v8/rendering/fwd_render_assets_cache.hpp>
//#include <framework/scene/fwd_scene_manager.hpp>

namespace v8 { namespace gui { 
    class basic_window;
}
}

namespace v8 {

class filesys;

class application_state {

public :

    typedef gui::basic_window* (*window_creation_fn)();

/// \name Construction.
/// @{

public :
    application_state();

    ~application_state();

    v8_bool_t initialize(window_creation_fn window_creator);

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

/// \name Data members.
/// @{

private :

    //! Renderer.

    base::scoped_ptr<rendering::renderer>                           render_sys_;    

    //! Resource cache.
    base::scoped_ptr<rendering::render_assets_cache>               asset_cache_;

/*
    //! Scene manager.
    v8::base::scoped_ptr<
        v8_framework::scene::scene_manager
    >                                                       Scene;
*/
    //! File system config.
    base::scoped_ptr<filesys>                                         file_sys_;

    //! Application window.
    base::scoped_ptr<gui::basic_window>                                 window_;


/// @}

/// \name Disabled functions.
/// @{

private :
    NO_CC_ASSIGN(application_state);

/// @}

};

extern application_state*   state;

} // namespace v8
