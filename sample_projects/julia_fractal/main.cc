#include <v8/io/filesystem.hpp>
#include <v8/global_state.hpp>
#include <v8/rendering/constants.hpp>
#include <v8/rendering/renderer.hpp>
#include <v8/rendering/render_assets_cache.hpp>
#include <v8/utility/win_util.hpp>
#include <third_party/fast_delegate/fast_delegate.hpp>
#include <v8/v8.hpp>
#include <v8/base/debug_helpers.hpp>
#include <v8/base/scoped_pointer.hpp>
#include <v8/base/scoped_resource.hpp>
#include <v8/math/color.hpp>
#include <v8/math/vector3.hpp>
#include <v8/math/matrix4X4.hpp>
#include <v8/scene/scene_system.hpp>

#include "fractal.hpp"
#include "fractal_window.hpp"

namespace {

class fractal_app_context : public v8::application_state {
public :
    v8_bool_t initialize();

private :
    fractal julia_;
};

v8_bool_t fractal_app_context::initialize() {
    application_state::initialize();

    window_ = new fractal_window();
    const v8_uint32_t window_style = WS_OVERLAPPEDWINDOW;
    const v8_int32_t window_width = 1024;
    const v8_int32_t window_height = 1024;
    if (!window_->initialize(window_style, "FractalWindowClass", 
                             "Julia fractal explorer",
                             window_width, window_height)) {
        return false;
    }

    render_sys_ = new v8::rendering::renderer();
    if (!render_sys_->initialize(window_->get_handle(), 
                                 static_cast<float>(window_width),
                                 static_cast<float>(window_height),
                                 v8::rendering::ElementType::Unorm8, 4,
                                 24, 8)) {
        return false;
    }

    window_->Subscribers_ResizeEvent += fastdelegate::MakeDelegate(
        render_sys(), &v8::rendering::renderer::on_viewport_resized
        );

    asset_cache_ = new v8::rendering::render_assets_cache(render_sys());
    file_sys_ = new v8::filesys();
    const char* const app_data_dir = "D:\\games\\fractals";
    file_sys_->initialize(app_data_dir);

    g_fractal = &julia_;
    if (!g_fractal->initialize()) {
        return false;
    }

    window()->Subscribers_InputEvents += 
        fastdelegate::MakeDelegate(g_fractal, &fractal::on_input);
    window()->Subscribers_ResizeEvent +=
        fastdelegate::MakeDelegate(g_fractal, &fractal::on_resize);
    return true;
}

}

int WINAPI WinMain(
    HINSTANCE inst,
    HINSTANCE,
    LPSTR,
    int
    ) {
    //
    // Must be the first constructed object.
    v8::utility::win32::scoped_mem_leak_checker leak_check_obj;
    {
        fractal_app_context app_context;
        if (!app_context.initialize()) {
            return -1;
        }
        app_context.window()->message_loop();
    }
    return 0;
}
