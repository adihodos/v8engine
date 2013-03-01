#pragma once

#include <cassert>
#include <v8/v8.hpp>
#include <v8/base/scoped_pointer.hpp>
#include <v8/rendering/fwd_renderer.hpp>
//#include <framework/rendering/fwd_resource_manager.hpp>
//#include <framework/scene/fwd_scene_manager.hpp>

namespace v8 { 

class filesys;

class application_state {
/// \name Construction.
/// @{

public :
    application_state();

    ~application_state();

/// @}

/// \name Data members.
/// @{

public :

    //! Renderer.

    v8::base::scoped_ptr<
        v8::rendering::renderer
    >                                                       Renderer;
/*
    //! Resource cache.
    v8::base::scoped_ptr<
        v8_framework::render_engine::resource_manager
    >                                                       AssetCache;

    //! Scene manager.
    v8::base::scoped_ptr<
        v8_framework::scene::scene_manager
    >                                                       Scene;
*/
    //! File system config.
    v8::base::scoped_ptr<
        v8::filesys
    >                                                       Filesys;

    //! Application window.
    //v8::base::scoped_ptr<
        //v8_framework::gui::basic_window
    //>                                                       Window;


/// @}

/// \name Disabled functions.
/// @{

private :
    NO_CC_ASSIGN(application_state);

/// @}

};

extern application_state*   state;

} // namespace v8
