#include <cassert>
#include "v8/math/camera.hpp"

#include "v8/scene/camera_controller.hpp"
#include "v8/event/window_event.hpp"

void v8::scene::camera_controller::on_viewport_resized(
    const v8::resize_event& ev_resize
    ) {
    assert(ev_resize.width > 0);
    assert(ev_resize.height > 0);
    assert(cam_ptr_);
    
    cam_ptr_->set_symmetric_frustrum(
        cam_ptr_->get_dmin(), cam_ptr_->get_dmax(),
        static_cast<float>(ev_resize.width),
        static_cast<float>(ev_resize.height),
        cam_ptr_->get_projection_type()
        );
}
