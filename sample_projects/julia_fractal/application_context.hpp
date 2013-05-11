#pragma once

#include <v8/rendering/fwd_renderer.hpp>
#include <v8/gui/fwd_basic_window.hpp>
#include <v8/io/fwd_filesystem.hpp>

struct fractal_app_context {
    v8::gui::basic_window*      Window;
    v8::rendering::renderer*    Renderer;
    v8::filesys*                Filesys;

    fractal_app_context()
        :       Window(nullptr)
            ,   Renderer(nullptr)
            ,   Filesys(nullptr)
    {}
};