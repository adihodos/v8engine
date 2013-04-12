#pragma once

#include <v8/v8.hpp>
#include <v8/base/scoped_pointer.hpp>
#include <v8/gui/basic_window.hpp>
#include <v8/event/fwd_event_types.hpp>

class main_window : public v8::gui::basic_window {

private :
    typedef v8::gui::basic_window                         base_class;

public :
    main_window();

    ~main_window();

    v8_bool_t initialize_scene();

protected :

    void app_frame_tick();

    void frame_draw_impl();

private :
    struct scene_details;
    v8::base::scoped_ptr<scene_details>                 scene_;    

private :
    NO_CC_ASSIGN(main_window);
};