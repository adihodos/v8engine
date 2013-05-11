#include <v8/rendering/constants.hpp>
#include <v8/rendering/renderer.hpp>
#include <v8/utility/win_util.hpp>
#include "window.hpp"

v8::rendering::renderer* graphics_sys_ptr;

int WINAPI WinMain(HINSTANCE inst, HINSTANCE, LPSTR, int) {
    v8::utility::win32::scoped_mem_leak_checker memchecker;
    {
        main_window app_window;
        v8::gui::window_init_params_t win_init_params = {
            1024, 1024, "Basic Drawing with D3D"
        };
        if (!app_window.initialize(win_init_params)) {
            return -1;
        }

        v8::rendering::renderer graphics_system;
        v8::rendering::render_init_params graphics_init_params;
        graphics_init_params.target_window        = app_window.get_handle();
        graphics_init_params.antialiasing         = false;
        graphics_init_params.buffer_element_type  = 
            v8::rendering::ElementType::Unorm8;
        graphics_init_params.buffer_element_count = 4;
        graphics_init_params.depth_bits           = 24;
        graphics_init_params.stencil_bits         = 8;
        graphics_init_params.full_screen          = false;
        graphics_init_params.handle_full_screen   = true;
        graphics_init_params.render_targets_count = 1;
        graphics_init_params.width                = 1024;
        graphics_init_params.height               = 1024;
        graphics_init_params.clear_color          = 
            v8::math::color_rgb::C_Darkorange;

        if (!graphics_system.initialize(graphics_init_params)) {
            return -1;
        }
        graphics_sys_ptr = &graphics_system;
        app_window.message_loop(&graphics_system);
    }
    return 0;
}