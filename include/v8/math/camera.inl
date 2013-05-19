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

inline void v8::math::camera::update_projection_view_transform() {
    projection_view_matrix_ = projection_matrix_ * view_matrix_;
}

inline v8::math::camera& v8::math::camera::set_origin(
    const math::vector3F& origin
    ) {
    view_pos_ = origin;

    update_view_matrix();
    update_projection_view_transform();

    return *this;
}

inline const v8::math::vector3F& v8::math::camera::get_origin() const {
    return view_pos_;
}

inline const v8::math::vector3F& v8::math::camera::get_direction_vector() const {
    return view_dir_;
}

inline const v8::math::vector3F& v8::math::camera::get_up_vector() const {
    return view_up_;
}

inline const v8::math::vector3F& v8::math::camera::get_right_vector() const {
    return view_side_;
}

inline const v8::math::matrix_4X4F& v8::math::camera::get_view_transform() const {
    return view_matrix_;
}

inline 
const v8::math::matrix_4X4F& 
v8::math::camera::get_projection_transform() const {
    return projection_matrix_;
}

inline
void
v8::math::camera::set_projection_type(v8_int32_t projection_type) {
    this->projection_type_ = projection_type;
    //handle_frustrum_param_change();
}

inline 
void 
v8::math::camera::set_projection_matrix(
    const v8::math::matrix_4X4F& mtx
    ) {
    projection_matrix_ = mtx;
    update_projection_view_transform();
}

inline int v8::math::camera::get_projection_type() const {
    return projection_type_;
}

inline 
const v8::math::matrix_4X4F&
v8::math::camera::get_projection_wiew_transform() const {
    return projection_view_matrix_;
}

inline const float* v8::math::camera::get_frustrum() const {
    return frustrum_params_;
}

inline void v8::math::camera::copy_frustrum_params(float* fparams) const {
    memcpy(fparams, frustrum_params_, sizeof(frustrum_params_));
}

inline float v8::math::camera::get_dmin() const {
    return frustrum_params_[FrustrumParam::DMin];
}

inline float v8::math::camera::get_dmax() const {
    return frustrum_params_[FrustrumParam::DMax];
}

inline float v8::math::camera::get_umin() const {
    return frustrum_params_[FrustrumParam::UMin];
}

inline float v8::math::camera::get_umax() const {
    return frustrum_params_[FrustrumParam::UMax];
}

inline float v8::math::camera::get_rmin() const {
    return frustrum_params_[FrustrumParam::RMin];
}

inline float v8::math::camera::get_rmax() const {
    return frustrum_params_[FrustrumParam::RMax];
}

inline v8_uint32_t v8::math::camera::get_frameid() const {
    return frameid_;
}

inline v8_int32_t v8::math::camera::get_depth_range_type() const {
    return depth_type_;
}

inline void v8::math::camera::set_depth_range_type(
    v8_int32_t type
    ) {
    assert(type == DepthType::DirectX || type == DepthType::OpenGL);
    depth_type_ = type;
}
