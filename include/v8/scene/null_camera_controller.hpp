#pragma once

#include <v8/scene/camera_controller.hpp>

//
// Forward declare for the camera class.
namespace v8 { namespace math {
    class camera;
} 
}

namespace v8 { namespace scene {

//!
//! \brief Null camera controller. 
//! Does not process any kind of user input, nor it is updated.
class null_camera_controller : public camera_controller {
public :
    typedef camera_controller                       base_class;

    explicit null_camera_controller(v8::math::camera* cam = nullptr)
        : base_class(cam) {}

    void update(float);

    v8_bool_t key_press_event(
        v8_int_t key_code
        );

    v8_bool_t key_depress_event(
        v8_int_t key_code
        );

    v8_bool_t mouse_wheel_event(
        v8_int_t rotations, 
        v8_int_t key_flags, 
        v8_int_t xpos, 
        v8_int_t ypos
        );

    v8_bool_t left_button_press_event(
        v8_int_t key_flags, 
        v8_int_t xpos, 
        v8_int_t ypos
        );

    v8_bool_t left_button_depress_event(
        v8_int_t key_flags, 
        v8_int_t xpos, 
        v8_int_t ypos
        );

    v8_bool_t mouse_moved_event(
        v8_int_t key_flags, 
        v8_int_t xpos, 
        v8_int_t ypos
        );

    std::string get_debug_stats();
};

} // namespace scene
} // namespace v8
