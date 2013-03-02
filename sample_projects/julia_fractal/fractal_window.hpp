#pragma once

#include <v8/v8.hpp>
#include <v8/gui/basic_window.hpp>
#include <v8/event/fwd_event_types.hpp>

class fractal;
extern fractal* g_fractal;

class fractal_window : public v8::gui::basic_window {

private :
    typedef v8::gui::basic_window                         base_class;

public :
    fractal_window();

    ~fractal_window();

protected :

    void app_frame_tick();

    void frame_draw_impl();

private :

    NO_CC_ASSIGN(fractal_window);
};