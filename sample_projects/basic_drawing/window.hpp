#pragma once

#include <v8/v8.hpp>
#include <v8/gui/basic_window.hpp>

class main_window : public v8::gui::basic_window {
protected :
    void frame_draw_impl(); 
};