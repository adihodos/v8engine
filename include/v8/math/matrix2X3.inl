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

template<typename real_t>
const v8::math::matrix_2X3<real_t>
v8::math::matrix_2X3<real_t>::zero(real_t(0), real_t(0), real_t(0),
                                   real_t(0), real_t(0), real_t(0));

template<typename real_t>
const v8::math::matrix_2X3<real_t>
v8::math::matrix_2X3<real_t>::identity(real_t(1), real_t(0), real_t(0),
                                       real_t(0), real_t(1), real_t(0));

template<typename real_t>
inline v8::math::matrix_2X3<real_t>::matrix_2X3(
    const real_t a11, const real_t a12, const real_t a13,
    const real_t a21, const real_t a22, const real_t a23
    ) {
    a11_ = a11; a12_ = a12; a13_ = a13;
    a21_ = a21; a22_ = a22; a23_ = a23;
}

template<typename real_t>
template<typename real_u>
inline v8::math::matrix_2X3<real_t>::matrix_2X3(
    const v8::math::matrix_2X3<real_u>& other
    ) {
    a11_ = other.a11_; a12_ = other.a12_; a13_ = other.a13_;
    a21_ = other.a21_; a22_ = other.a22_; a23_ = other.a23_;
}

template<typename real_t>
inline v8::math::matrix_2X3<real_t>::matrix_2X3(
    const real_t* data, const v8_size_t count) {
    memcpy(elements_, data, min(count_of_array(elements_), count));
}

template<typename real_t>
template<typename real_u>
inline v8::math::matrix_2X3<real_t>& 
v8::math::matrix_2X3<real_t>::operator=(
    const v8::math::matrix_2X3<real_u>& other) {
    a11_ = other.a11_; a12_ = other.a12_; a13_ = other.a13_;
    a21_ = other.a21_; a22_ = other.a22_; a23_ = other.a23_;
    return *this;
}

template<typename real_t>
template<typename real_u>
inline v8::math::matrix_2X3<real_t>& 
v8::math::matrix_2X3<real_t>::operator+=(
    const v8::math::matrix_2X3<real_u>& other
    ) {
    for (uint32_t i = 0; i < count_of_array(elements_); ++i)
        elements_[i] += other.elements_[i];
    return *this;
}

template<typename real_t>
template<typename real_u>
inline v8::math::matrix_2X3<real_t>& 
v8::math::matrix_2X3<real_t>::operator-=(
    const v8::math::matrix_2X3<real_u>& other
    ) {
    for (uint32_t i = 0; i < count_of_array(elements_); ++i)
        elements_[i] -= other.elements_[i];
    return *this;
}

template<typename real_t>
template<typename real_u>
inline v8::math::matrix_2X3<real_t>& 
v8::math::matrix_2X3<real_t>::operator*=(
    real_u conv_scalar
    ) {
    for (uint32_t i = 0; i < count_of_array(elements_); ++i)
        elements_[i] *= conv_scalar;
    return *this;
}

template<typename real_t>
inline v8::math::matrix_2X3<real_t>&
v8::math::matrix_2X3<real_t>::make_zero() {
    memcpy(elements_, matrix_2X3<real_t>::zero.elements_, 
           sizeof(elements_));
    return *this;
}

template<typename real_t>
inline v8::math::matrix_2X3<real_t>&
v8::math::matrix_2X3<real_t>::make_identity() {
    memcpy(elements_, matrix_2X3<real_t>::identity.elements_,
           sizeof(elements_));
    return *this;
}

template<typename real_t>
inline v8::math::matrix_2X3<real_t>&
v8::math::matrix_2X3<real_t>::make_scaling(const real_t sx, const real_t sy) {
    a11_ = sx;
    a22_ = sy;
    a12_ = a13_ = a21_ = a23_ = real_t(0);
    return *this;
}

template<typename real_t>
inline v8::math::matrix_2X3<real_t>&
v8::math::matrix_2X3<real_t>::make_translation(real_t tx, real_t ty) {
    a11_ = a22_ = real_t(1); 
    a12_ = a21_ = real_t(0);
    a13_ = tx; a23_ = ty;

    return *this;
}

template<typename real_t>
inline typename v8::math::matrix_2X3<real_t>::class_type&
v8::math::matrix_2X3<real_t>::make_rotation(const real_t radians) {
    const real_t cos_theta = cos(radians);
    const real_t sin_theta = sin(radians);

    a11_ = cos_theta; a12_ = -sin_theta; a13_ = real_t(0);
    a21_ = sin_theta; a22_ = +cos_theta; a23_ = real_t(0);

    return *this;
}

template<typename real_t>
inline typename v8::math::matrix_2X3<real_t>::class_type&
v8::math::matrix_2X3<real_t>::make_rotation_off_center(const real_t radians,
                                                       const real_t xpos,
                                                       const real_t ypos) {
    const real_t sin_theta = sin(radians);
    const real_t cos_theta = cos(radians);

    a11_ = cos_theta; a12_ = -sin_theta; a13_ = xpos * (cos_theta - 1) - ypos * sin_theta;
    a21_ = sin_theta; a22_ = +sin_theta; a23_ = ypos * (cos_theta - 1) + xpos * sin_theta;

    return *this;
}

template<typename real_t>
inline typename v8::math::matrix_2X3<real_t>::class_type&
v8::math::matrix_2X3<real_t>::make_reflection(const real_t vdir_x,
                                              const real_t vdir_y) {
    const vector2<real_t> ref_normal(vector2<real_t>(vdir_x, vdir_y).normalize());
    return make_reflection_from_angle_with_x(ref_normal.x_);
}

template<typename real_t>
inline typename v8::math::matrix_2X3<real_t>::class_type&
v8::math::matrix_2X3<real_t>::make_reflection_point(const real_t pt_x,
                                                    const real_t pt_y) 
{
    a11_ = real_t(-1); a12_ = real_t(0); a13_ = real_t(2) * pt_x;
    a21_ = real_t(0);  a22_ = real_t(-1); a23_ = real_t(2) * pt_y;
    return *this;
}

template<typename real_t>
inline typename v8::math::matrix_2X3<real_t>::class_type&
v8::math::matrix_2X3<real_t>::make_reflection_line(const real_t org_x, 
                                                   const real_t org_y,
                                                   const real_t dir_x, 
                                                   const real_t dir_y) 
{
    //a11_ = real_t(2) * dir_x * dir_x - real_t(1);
    //a12_ = real_t(2) * dir_x * dir_y;
    //a13_ = real_t(2) * (org_x * (real_t(1) - dir_x * dir_x) 
    //                    - dir_x * dir_y * org_y);

    //a21_ = a12_;
    //a22_ = real_t(2) * dir_y * dir_y - real_t(1);
    //a23_ = real_t(2) * (org_y * (real_t(1) - dir_y * dir_y) 
    //                    - dir_x * dir_y * org_x);

    const real_t theta = acos(dir_x);
    const real_t sin2theta = sin(2 * theta);
    const real_t cos2theta = cos(2 * theta);

    a11_ = cos2theta; a12_ = sin2theta; a13_ = org_x * (1 - cos2theta) 
                                               - org_y * sin2theta;

    a21_ = sin2theta; a22_ = -cos2theta; a23_ = org_y * (1 + cos2theta)
                                                - org_x * sin2theta;

    return *this;
}

template<typename real_t>
inline typename v8::math::matrix_2X3<real_t>::class_type&
v8::math::matrix_2X3<real_t>::make_reflection_from_angle_with_x(const real_t theta) {
    const real_t sin2theta = sin(2 * theta);
    const real_t cos2theta = cos(2 * theta);

    a11_ = cos2theta; a12_ = +sin2theta; a13_ = real_t(0);
    a21_ = sin2theta; a22_ = -cos2theta; a23_ = real_t(0);
    return *this;
}

template<typename real_t>
inline typename v8::math::matrix_2X3<real_t>::class_type&
v8::math::matrix_2X3<real_t>::make_reflection_slope(const real_t slope) {
    const real_t ssq = slope * slope;
    const real_t mul_factor = real_t(1) / (ssq + 1);

    a11_ = (real_t(1) - ssq) * mul_factor;
    a12_ = 2 * ssq * mul_factor;
    a13_ = real_t(0);

    a21_ = a12_;
    a22_ = (ssq - real_t(1)) * mul_factor;
    a32_ = real_t(0);

    return *this;
}

template<typename real_t>
inline typename v8::math::matrix_2X3<real_t>::class_type&
v8::math::matrix_2X3<real_t>::make_reflection_x() {
    a11_ = real_t(1); a12_ = real_t(0); a13_ = real_t(0);
    a21_ = real_t(0); a22_ = real_t(-1); a23_ = real_t(0);

    return *this;
}

template<typename real_t>
inline typename v8::math::matrix_2X3<real_t>::class_type&
v8::math::matrix_2X3<real_t>::make_reflection_y() {
    a11_ = real_t(-1); a12_ = real_t(0); a13_ = real_t(0);
    a21_ = real_t(0); a22_ = real_t(+1); a23_ = real_t(0);

    return *this;
}

template<typename real_t>
template<typename real_u>
inline v8::math::matrix_2X3<real_t>&
v8::math::matrix_2X3<real_t>::transform_vector(
    v8::math::vector2<real_u>* vec
    ) {
    vec->x_ = a11_ * vec->x_ + a12_ * vec->y_;
    vec->y_ = a21_ * vec->x_ + a22_ * vec->y_;
    return *this;
}

template<typename real_t>
template<typename real_u>
inline v8::math::matrix_2X3<real_t>&
v8::math::matrix_2X3<real_t>::transform_point(
    v8::math::vector2<real_u>* pt
    ) {
    pt->x_ = a11_ * pt->x_ + a12_ * pt->y_ + a13_;
    pt->y_ = a21_ * pt->x_ + a22_ * pt->y_ + a23_;
    return *this;
}

template<typename real_t>
inline bool v8::math::operator==(
    const v8::math::matrix_2X3<real_t>& lhs,
    const v8::math::matrix_2X3<real_t>& rhs
    ) {
    for (uint32_t i = 0; i < count_of_array(lhs.elements_); ++i)
        if (!operands_eq(lhs.elements_[i], rhs.elements_[i]))
            return false;

    return true;
}

template<typename real_t>
inline bool v8::math::operator!=(
    const v8::math::matrix_2X3<real_t>& lhs,
    const v8::math::matrix_2X3<real_t>& rhs
    ) {
    return !(lhs == rhs);
}

template<typename real_t>
inline v8::math::matrix_2X3<real_t> 
v8::math::operator+(
    const v8::math::matrix_2X3<real_t>& lhs,
    const v8::math::matrix_2X3<real_t>& rhs
    ) {
    matrix_2X3<real_t> result(lhs);
    result += rhs;
    return result;
}

template<typename real_t>
inline v8::math::matrix_2X3<real_t> 
v8::math::operator-(
    const v8::math::matrix_2X3<real_t>& lhs,
    const v8::math::matrix_2X3<real_t>& rhs
    ) {
    matrix_2X3<real_t> result(lhs);
    result -= rhs;
    return result;
}

template<typename real_t>
v8::math::matrix_2X3<real_t> 
v8::math::operator*(
    const v8::math::matrix_2X3<real_t>& lhs,
    const v8::math::matrix_2X3<real_t>& rhs
    ) {
    matrix_2X3<real_t> rs;
    rs.a11_ = lhs.a11_ * rhs.a11_ + lhs.a12_ * rhs.a21_;
    rs.a12_ = lhs.a11_ * rhs.a12_ + lhs.a12_ * rhs.a22_;
    rs.a13_ = lhs.a11_ * rhs.a13_ + lhs.a12_ * rhs.a23_ + lhs.a13_;

    rs.a21_ = lhs.a21_ * rhs.a11_ + lhs.a22_ * rhs.a21_;
    rs.a22_ = lhs.a21_ * rhs.a12_ + lhs.a22_ * rhs.a22_;
    rs.a23_ = lhs.a21_ * rhs.a13_ + lhs.a22_ * rhs.a23_ + lhs.a23_;

    return rs;
}

template<typename real_t, typename real_u>
inline v8::math::matrix_2X3<real_t> 
v8::math::operator*(
    real_u scalar,
    const v8::math::matrix_2X3<real_t>& mtx
    ) {
    matrix_2X3<real_t> result(mtx);
    result *= scalar;
    return result;
}

template<typename real_t, typename real_u>
inline v8::math::matrix_2X3<real_t> 
v8::math::operator*(
    const v8::math::matrix_2X3<real_t>& mtx,
    real_u scalar
    ) {
    return scalar * mtx;
}

