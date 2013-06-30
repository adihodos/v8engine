//
// Copyright (c) 2011, 2012, Adrian Hodos
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the author nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR THE CONTRIBUTORS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include <v8/v8.hpp>

#include <v8/math/camera.hpp>
#include <v8/math/matrix4X4.hpp>
#include <v8/math/vector2.hpp>
#include <v8/math/vector3.hpp>

#include <v8/scene/camera_controller.hpp>

namespace v8 { namespace scene {

class camera_controller_spherical_coords : public camera_controller {

    float                       angle_phi_;
    float                       angle_theta_;
    float                       radius_;
    float                       zoom_speed_;
    float                       rotate_speed_;
    v8::math::vector3F          lookat_point_;
    mutable v8_bool_t           updated_;
    v8::math::vector2<int>      last_mouse_pos_;

    static const float          kPHIMin;
    static const float          kPHIMax;
    static const float          kZoomMin;
    static const float          kZoomMax;

    void defaults() {
        angle_phi_ = -90.0f;
        angle_theta_ = 0.0f;
        radius_ = 10.0f;
        rotate_speed_ = 0.2f;
        zoom_speed_ = 0.1f;
        lookat_point_ = v8::math::vector3F::zero;
        updated_ = false;
    }

    void update_cam_data() {
        using namespace v8::math;

        const vector3F cam_pos = point_from_spherical_coordinates(
            radius_, angle_phi_, angle_theta_
            );

        cam_ptr_->look_at(cam_pos, vector3F::unit_y, lookat_point_);
        updated_ = true;
    }

public :

    explicit camera_controller_spherical_coords(
        v8::math::camera* cam = nullptr
        ) 
        :   camera_controller(cam) {
        defaults();
    }

    camera_controller_spherical_coords(
        v8::math::camera* cam, 
        float phi, 
        float theta, 
        float radius, 
        float zoom_spd,
        float rotate_spd, 
        const v8::math::vector3F& look_at
        )
        :   camera_controller(cam),
            angle_phi_(phi), 
            angle_theta_(theta), 
            radius_(radius), 
            zoom_speed_(zoom_spd), 
            rotate_speed_(rotate_spd), 
            lookat_point_(look_at),
            updated_(false) 
    {
    }

    void set_zoom_speed(const float spd) {
        zoom_speed_ = spd;
    }

    void set_rotate_speed(const float spd) {
        rotate_speed_ = spd;
    }

    void rotate_y(const bool counter_clockwise) {
        const float modifier[2] = { 1.0f, -1.0f };
        const float new_val = 
            angle_phi_ + rotate_speed_ * modifier[counter_clockwise];

        if (fabs(new_val) >= kPHIMin && fabs(new_val) <= kPHIMax) {
            angle_phi_ = new_val;
            updated_ = false;
        }
    }

    void rotate_z(const bool counter_clockwise) {
        const float modifier[2] = { -1.0f, 1.0f };
        angle_theta_ += rotate_speed_ * modifier[counter_clockwise];
        updated_ = false;
    }

    void zoom(const bool inwards) {
        const float modifier[2] = { +1.0f, -1.0f };
        
        const float new_val = radius_ + zoom_speed_ * modifier[inwards];
        if (fabs(new_val) >= kZoomMin && fabs(new_val) <= kZoomMax) {
            radius_ = new_val;
            updated_ = false;
        }
    }

    void set_camera(v8::math::camera* cam) {
        cam_ptr_ = cam;
    }

    void set_angle_phi(const float phi) {
        angle_phi_ = phi;
        updated_ = false;
    }

    void set_angle_theta(const float theta) {
        angle_theta_ = theta;
        updated_ = false;
    }

    void update(const float /*delta*/) {
        if (!updated_)
            update_cam_data();
    }

    v8_bool_t 
    initialize(const void* config_file_path);

/// \name Event handling.
/// @{

public :

    void on_input_event(const input_event& ev_input);

private :

    void handle_key_press(const v8_int_t key_code);

    void handle_mouse_wheel(
        const v8_int_t k_num_rotations,
        const v8_int_t k_xpos,
        const v8_int_t k_ypos
        );

    void handle_mouse_moved(
        const v8_int_t xpos, 
        const v8_int_t ypos
        );

/// @}
};

} // namespace scene
} // namespace v8
