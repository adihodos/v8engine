#pragma once

#include <v8/event/fwd_event_types.hpp>
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

    void update(const float);

    void on_input_event(const input_event& ev_input);

    std::string get_debug_stats();
};

} // namespace scene
} // namespace v8
