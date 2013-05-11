#pragma once

#include <v8/v8.hpp>
#include <v8/gui/basic_window.hpp>

class fractal_window : public v8::gui::basic_window {

private :
    typedef v8::gui::basic_window                         base_class;

public :
    fractal_window();

    ~fractal_window();

private :
    NO_CC_ASSIGN(fractal_window);
};