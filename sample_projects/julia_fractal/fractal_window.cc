#include <v8/v8.hpp>
#include <v8/base/string_util.hpp>
#include <v8/base/auto_buffer.hpp>

#include <v8/global_state.hpp>
#include <v8/rendering/renderer.hpp>
#include <v8/rendering/constants.hpp>
#include <v8/event/window_event.hpp>

#include "fractal.hpp"
#include "fractal_window.hpp"

fractal_window::fractal_window()
    :   base_class() {}

fractal_window::~fractal_window() {}

void fractal_window::app_frame_tick() {
    const float delta_ms = m_runstats.timer.tick();
    m_runstats.fpscount.frame_tick(delta_ms);
    g_fractal->evaluate(delta_ms);
}

void fractal_window::frame_draw_impl() {
    g_fractal->draw();

    const wchar_t* const C_fractal_stats_fmt_str = 
        L"Zoom : %4.6f \n"
        L"Iterations : %d \n"
        L"Offset : [%4.4f, %4.4f] \n"
        L"Constant : [%4.6f, %4.6f]";

    v8::base::auto_buffer<wchar_t> displ_string;
    v8::base::snwprintf(displ_string.begin(), displ_string.size(), 
                        C_fractal_stats_fmt_str, 
                        g_fractal->get_zoom_factor(),
                        g_fractal->get_iteration_count(),
                        g_fractal->get_x_offset(),
                        g_fractal->get_y_offset(),
                        g_fractal->get_constant().real(),
                        g_fractal->get_constant().imag());

    v8::state->render_sys()->draw_string(displ_string.begin(), 12.0f, 5.0f, 5.0f,
                                         v8::math::color_rgb::C_White);
}