#include <v8/io/filesystem.hpp>
#include <v8/global_state.hpp>
#include <v8/rendering/constants.hpp>
#include <v8/rendering/renderer.hpp>
#include <v8/rendering/render_assets_cache.hpp>
#include <v8/utility/win_util.hpp>
#include <v8/fast_delegate/fast_delegate.hpp>
#include <v8/v8.hpp>
#include <v8/base/debug_helpers.hpp>
#include <v8/base/scoped_pointer.hpp>
#include <v8/base/scoped_resource.hpp>
#include <v8/event/window_event.hpp>
#include <v8/math/camera.hpp>
#include <v8/math/color.hpp>
#include <v8/math/vector3.hpp>
#include <v8/math/matrix4X4.hpp>
#include <v8/scene/scene_system.hpp>

#include "main_window.hpp"

namespace {

class animation_app_context : public v8::application_state {
public :
    v8_bool_t initialize();

public :
    void on_window_resized(const v8::resize_event& resize_evt);    

private :
};

v8_bool_t animation_app_context::initialize() {
    application_state::initialize();

    window_ = new main_window();
    const v8_uint32_t window_style = WS_OVERLAPPEDWINDOW;
    const v8_int32_t window_width = 1024;
    const v8_int32_t window_height = 1024;
    if (!window_->initialize(window_style, "AnimWindowClass", 
                             "Animation using quaternions",
                             window_width, window_height)) {
        return false;
    }

    render_sys_ = new v8::rendering::renderer();
    v8::rendering::render_init_params initialization_params;
    initialization_params.width = window_->get_width();
    initialization_params.height = window_->get_height();
    initialization_params.target_window = window_->get_handle();
    initialization_params.render_targets_count = 1;
    initialization_params.stencil_bits = 8;
    initialization_params.depth_bits = 24;
    initialization_params.buffer_element_type = v8::rendering::ElementType::Unorm8;
    initialization_params.buffer_element_count = 4;

    if (!render_sys_->initialize(initialization_params)) {
        return false;
    }

    window_->Subscribers_ResizeEvent += fastdelegate::MakeDelegate(
        render_sys(), &v8::rendering::renderer::on_viewport_resized
        );

    asset_cache_ = new v8::rendering::render_assets_cache(render_sys());
    file_sys_ = new v8::filesys();
    //
    // TODO : fix hard coded path, it sucks.
    const char* const app_data_dir = "E:\\games\\simple_anim";
    file_sys_->initialize(app_data_dir);

    cam_ptr_ = new v8::math::camera();
    cam_ptr_->look_at(v8::math::vector3F(0.0f, 10.0f, -10.0f),
                      v8::math::vector3F::unit_y,
                      v8::math::vector3F::zero);
    cam_ptr_->set_symmetric_frustrum_perspective(
        45.0f, window_->get_aspect_ratio(), 1.0f, 1000.0f
        );

    window_->Subscribers_ResizeEvent += fastdelegate::MakeDelegate(
        this, &animation_app_context::on_window_resized
        );

    return static_cast<main_window*>(window())->initialize_scene();
}

void animation_app_context::on_window_resized(const v8::resize_event&) {
    cam_ptr_->set_symmetric_frustrum_perspective(
        45.0f, window()->get_aspect_ratio(), 1.0f, 1000.0f
        );
}

}

int WINAPI WinMain(HINSTANCE inst, HINSTANCE, LPSTR, int) {
    //
    // Must be the first constructed object to report any leaked memory.
    //v8::utility::win32::scoped_mem_leak_checker leak_check_obj;
    {
        animation_app_context app_context;
        if (!app_context.initialize()) {
            return -1;
        }
        app_context.window()->message_loop();
    }
    return 0;
}
