#pragma once

#include <string>

#include <v8/v8.hpp>
#include <v8/event/fwd_event_types.hpp>

namespace v8 { namespace math {
    class camera;
} 
}

namespace v8 {
    struct resize_event;
    struct input_event;
}

namespace v8 { namespace scene {

class camera_controller {              

/// \name Constructors.
/// @{

public :

    explicit camera_controller(v8::math::camera* cam = nullptr)
        :   cam_ptr_(cam) 
    {}

    virtual ~camera_controller() {}
    
/// @}

/// \name Operations.
/// @{

public :

    v8::math::camera* get_camera() { return cam_ptr_; }

    void set_camera(v8::math::camera* cam) { cam_ptr_ = cam; }

    virtual v8_bool_t initialize(const void*) { return true; }

    virtual void update(const float delta_ms) = 0;

/// @}

/// \name Event handlers.
/// @{

public :

    ///
    /// \brief Called when the viewport is resized. The default implementation
    /// adjusts the camera's frustrum based on the new settings.
    virtual void on_viewport_resized(const resize_event& ev_resize);

    ///
    /// \brief Called when an input event was triggered.
    virtual void on_input_event(const input_event& ev_input) = 0;

/// @}    

protected :

    ///< Pointer to controlled camera.
    v8::math::camera*   cam_ptr_;
};

} // namespace scene
} // namespace v8
