#include "v8/scene/null_camera_controller.hpp"

void v8::scene::null_camera_controller::update(float) {
}

void v8::scene::null_camera_controller::on_input_event(
    const v8::input_event&
    ) {
}

std::string v8::scene::null_camera_controller::get_debug_stats() {
    return "null_camera_controller";
}