#include "v8/scene/null_camera_controller.hpp"

void v8::scene::null_camera_controller::update(float) {
}

v8_bool_t v8::scene::null_camera_controller::key_press_event(
    v8_int_t /*key_code*/
    ) {
    return false;
}

v8_bool_t v8::scene::null_camera_controller::key_depress_event(
    v8_int_t /*key_code*/
    ) {
    return false;
}

v8_bool_t v8::scene::null_camera_controller::mouse_wheel_event(
    v8_int_t /*rotations*/, 
    v8_int_t /*key_flags*/, 
    v8_int_t /*xpos*/, 
    v8_int_t /*ypos*/
    ) {
    return false;
}

v8_bool_t v8::scene::null_camera_controller::left_button_press_event(
    v8_int_t /*key_flags*/, 
    v8_int_t /*xpos*/, 
    v8_int_t /*ypos*/
    ) {
    return false;
}

v8_bool_t v8::scene::null_camera_controller::left_button_depress_event(
    v8_int_t /*key_flags*/, 
    v8_int_t /*xpos*/, 
    v8_int_t /*ypos*/
    ) {
    return false;
}

v8_bool_t v8::scene::null_camera_controller::mouse_moved_event(
    v8_int_t /*key_flags*/, 
    v8_int_t /*xpos*/, 
    v8_int_t /*ypos*/
    ) {
    return false;
}

std::string v8::scene::null_camera_controller::get_debug_stats() {
    return "null_camera_controller";
}