#include <third_party/stlsoft/platformstl/filesystem/path.hpp>

#include "v8/v8.hpp"
#include "v8/base/count_of.hpp"
#include "v8/base/crt_handle_policies.hpp"
#include "v8/base/debug_helpers.hpp"
#include "v8/base/scoped_handle.hpp"

#include "v8/event/input_event.hpp"
#include "v8/global_state.hpp"
#include "v8/io/config_file_reader.hpp"
#include "v8/io/filesystem.hpp"
#include "v8/input/key_syms.hpp"
#include "v8/utility/string_ext.hpp"

#include "v8/scene/cam_controller_spherical_coordinates.hpp"

const float v8::scene::camera_controller_spherical_coords::
    kPHIMin = 10.0f;

const float v8::scene::camera_controller_spherical_coords::
    kPHIMax = 170.0f;

const float v8::scene::camera_controller_spherical_coords::
    kZoomMin = 5.0f;

const float v8::scene::camera_controller_spherical_coords::
    kZoomMax = 500.0f;

void v8::scene::camera_controller_spherical_coords::
handle_key_press(const v8_int_t key_code) {
    using namespace v8::input;

    switch (key_code) {
    case Key_Sym_t::Left :
        rotate_z(true);
        break;

    case Key_Sym_t::Right :
        rotate_z(false);
        break;

    case Key_Sym_t::Up :
        rotate_y(false);
        break;

    case Key_Sym_t::Down :
        rotate_y(true);
        break;

    case Key_Sym_t::KP_Add :
        zoom(true);
        break;

    case Key_Sym_t::KP_Minus :
        zoom(false);
        break;

    case Key_Sym_t::F9 :
        initialize();
        break;

    default :
        break;
    }
}
//
void v8::scene::camera_controller_spherical_coords::
handle_mouse_wheel(
    const v8_int_t k_num_rotations,
    const v8_int_t /*k_xpos*/,
    const v8_int_t /*k_ypos*/
    ) {
    zoom(k_num_rotations > 0);
}

void v8::scene::camera_controller_spherical_coords::handle_mouse_moved(
    const v8_int_t xpos,
    const v8_int_t ypos
    ) {
    v8_int_t delta_x_axis = xpos - last_mouse_pos_.x_;
    v8_int_t delta_y_axis = ypos - last_mouse_pos_.y_;

    if (delta_y_axis) {
        rotate_y(delta_y_axis > 0);
        last_mouse_pos_.y_ = ypos;
    }

    if (delta_x_axis) {
        rotate_z(delta_x_axis > 0);
        last_mouse_pos_.x_ = xpos;
    }
}

v8_bool_t 
v8::scene::camera_controller_spherical_coords::initialize() {
    platformstl::path config_file_path(state->file_sys()->get_dir_path(
        filesys::Dir::Config));
    const char* const kCFGFileName = "cam_spherical_config.json";
    config_file_path.push(kCFGFileName);

    defaults();

    v8::io::V8ConfigFile conf_file(config_file_path.c_str());
    if (!conf_file.CheckIfValid()) {
        return true;
    }

    const v8::io::V8ConfigFile_ReadOnlySection conf_main_sec(
        conf_file.GetSection());

    conf_main_sec.GetValue("theta", angle_theta_);
    conf_main_sec.GetValue("phi", angle_phi_);
    conf_main_sec.GetValue("radius", radius_);
    conf_main_sec.GetValue("rotate_speed", rotate_speed_);
    conf_main_sec.GetValue("zoom_speed", zoom_speed_);
    conf_main_sec.GetValue("look_at", lookat_point_.elements_);
    return true;
}

void v8::scene::camera_controller_spherical_coords::on_input_event(
    const input_event& ev_input
    ) {
    if (ev_input.type == InputEventType::Mouse_Wheel) {
        handle_mouse_wheel(ev_input.mouse_wheel_ev.delta, 
                           ev_input.mouse_wheel_ev.x_pos, 
                           ev_input.mouse_wheel_ev.y_pos);
    } else if (ev_input.type == InputEventType::Mouse_Movement) {
        handle_mouse_moved(ev_input.mouse_move_ev.x_pos, 
                           ev_input.mouse_move_ev.y_pos);
    } else if (ev_input.type == InputEventType::Key) {
        if (ev_input.key_ev.down) {
            handle_key_press(ev_input.key_ev.key);
        }
    }
}
