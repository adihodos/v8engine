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
inline v8::math::quaternion<real_t>::quaternion() {}

template<typename real_t>
inline v8::math::quaternion<real_t>::quaternion(
    const real_t w, const real_t x, const real_t y, const real_t z
    ) : w_(w), x_(x), y_(y), z_(z) {}

template<typename real_t>
inline v8::math::quaternion<real_t>::quaternion(
    const real_t* init_data
    ) {
    memcpy(elements_, init_data, sizeof(elements_));
}

template<typename real_t>
inline v8::math::quaternion<real_t>::quaternion(
    const float angle,
    const v8::math::vector3<real_t>& axis
    ) {
    make_from_axis_angle(angle, axis);
}

template<typename real_t>
inline v8::math::quaternion<real_t>::quaternion(
    const v8::math::vector3<real_t>& v1, 
    const v8::math::vector3<real_t>& v2
    ) {
   make_from_vectors(v1, v2);
}

template<typename real_t>
inline v8::math::quaternion<real_t>&
v8::math::quaternion<real_t>::make_zero() {
    w_ = x_ = y_ = z_ = real_t(0);
    return *this;
}

template<typename real_t>
inline v8::math::quaternion<real_t>&
v8::math::quaternion<real_t>::make_identity() {
    w_ = real_t(1);
    x_ = y_ = z_ = real_t(0);
    return *this;
}

template<typename real_t>
inline v8::math::quaternion<real_t>&
v8::math::quaternion<real_t>::make_conjugate() {
    x_ = -x_; y_ = -y_; z_ = -z_;
    return *this;
}

template<typename real_t>
inline v8::math::quaternion<real_t>& 
v8::math::quaternion<real_t>::make_from_vector_and_scalar(
    const v8::math::vector3<real_t>& vec,
    const real_t scalar
    ) {
    w_ = scalar; x_ = vec.x_; y_ = vec.y_; z_ = vec.z_;
    return *this;
}

template<typename real_t>
v8::math::quaternion<real_t>&
v8::math::quaternion<real_t>::make_from_axis_angle(
    const float angle, 
    const v8::math::vector3<real_t>& axis
    ) {
    real_t length_squared = axis.sum_components_squared();
    if (math::operands_eq(real_t(0), length_squared)) {
        return make_identity();
    }

    const real_t scale_factor = sin(angle / 2) / sqrt(length_squared);

    w_ = cos(angle / 2);
    x_ = axis.x_ * scale_factor;
    y_ = axis.y_ * scale_factor;
    z_ = axis.z_ * scale_factor;
    return *this;
}

template<typename real_t>
v8::math::quaternion<real_t>&
v8::math::quaternion<real_t>::make_from_vectors( 
    const v8::math::vector3<real_t>& v1, 
    const v8::math::vector3<real_t>& v2
    ) {
    //
    // What's the explanation behind this method ?? I can't remember where
    // I've seen it first.

    vector3<real_t> bisector(v1 + v2);
    bisector.normalize();

    w_ = dot_product(v1, bisector);

    if (!math::operands_eq(real_t(0), w_)) {
        vector3<real_t> axis(cross_product(v1, bisector));
        x_ = axis.x_;
        y_ = axis.y_;
        z_ = axis.z_;
    } else {
        real_t inv_length;
        if (abs(v1.x_) >= abs(v1.y_)) {
            inv_length = math::inv_sqrt(v1.x_ * v1.x_ + v1.z_ * v1.z_);
            x_ = -v1.z_ * inv_length;
            y_ = real_t(0);
            z_ = +v1.x_ * inv_length;
        } else {
            inv_length = math::inv_sqrt(v1.y_ * v1.y_ + v1.z_ * v1.z_);
            x_ = real_t(0);
            y_ = +v1.z_ * inv_length;
            z_ = -v1.y_ * inv_length;
        }
    }

    return *this;
}

template<typename real_t>
v8::math::quaternion<real_t>&
v8::math::quaternion<real_t>::make_from_matrix(
    const v8::math::matrix_3X3<real_t>& mtx
    ) {
    const real_t trace = mtx.trace();
    if (trace > real_t(0)) {
        const real_t s = sqrt(trace + 1);
        w_ = s * real_t(0.5);
        const real_t recip = real_t(0.5) / s;
        x_ = (mtx(3, 2) - mtx(2, 3)) * recip;
        y_ = (mtx(1, 3) - mtx(3, 1)) * recip;
        z_ = (mtx(2, 1) - mtx(1, 2)) * recip;
    } else {
        unsigned int i = 0;
        if (mtx(2, 2) > mtx(1, 1))
            i = 1;

        if (mtx(3, 3) > mtx(i, i))
            i = 2;

        unsigned int j = (i + 1) % 3;
        unsigned int k = (j + 1) % 3;

        const real_t s = static_cast<real_t>(std::sqrt(mtx(i + 1, i + 1) 
                                                       - mtx(j + 1, j + 1) 
                                                       - mtx(k + 1, k + 1) 
                                                       + real_t(1)));
        elements_[i + 1] = real_t(0.5) * s;
        const real_t recip = real_t(0.5) / s;
        w_ = (mtx(k + 1, j + 1) - mtx(j + 1, k + 1)) * recip;
        elements_[j + 1] = (mtx(j + 1, i + 1) + mtx(i + 1, j + 1)) * recip;
        elements_[k + 1] = (mtx(k + 1, i + 1) + mtx(i + 1, k + 1)) * recip;
    }
    return *this;
}

template<typename real_t>
v8::math::quaternion<real_t>&
v8::math::quaternion<real_t>::operator +=(
    const v8::math::quaternion<real_t>& rhs
    ) {
    w_ += rhs.w_;
    x_ += rhs.x_;
    y_ += rhs.y_;
    z_ += rhs.z_;
    return *this;
}

template<typename real_t>
v8::math::quaternion<real_t>&
v8::math::quaternion<real_t>::operator -=(
    const v8::math::quaternion<real_t>& rhs
    ) {
    w_ -= rhs.w_;
    x_ -= rhs.x_;
    y_ -= rhs.y_;
    z_ -= rhs.z_;
    return *this;
}

template<typename real_t>
v8::math::quaternion<real_t>&
v8::math::quaternion<real_t>::operator *=(const real_t scalar) {
    w_ *= scalar;
    x_ *= scalar;
    y_ *= scalar;
    z_ *= scalar;
    return *this;
}

template<typename real_t>
v8::math::quaternion<real_t>&
v8::math::quaternion<real_t>::operator /=(const real_t scalar) {
    static_assert(is_floating_point, "Type must be a floating point value!");
    return *this *= (real_t(1) / scalar);    
}

template<typename real_t>
v8::math::quaternion<real_t>&
v8::math::quaternion<real_t>::normalize() {
    const real_t len_sq = length_squared();
    if (math::operands_eq(real_t(0), len_sq))
        return make_zero();

    const real_t scale_factor = real_t(1) / len_sq;
    w_ *= scale_factor;
    x_ *= scale_factor;
    y_ *= scale_factor;
    z_ *= scale_factor;

    return *this;
}

template<typename real_t>
v8::math::quaternion<real_t>&
v8::math::quaternion<real_t>::invert() {
    const real_t len_sq = length_squared();
    if (math::operands_eq(real_t(0), len_sq)) {
        return make_identity();
    }

    const real_t scale_factor = real_t(1) / len_sq;
    w_ *= scale_factor;
    x_ = (-x_ * scale_factor);
    y_ = (-y_ * scale_factor);
    z_ = (-z_ * scale_factor);

    return *this;
}

template<typename real_t>
void
v8::math::quaternion<real_t>::rotate_vector(v8::math::vector3<real_t>* p) {
    assert(is_unit() && "Quaternion must be unit length!");

    const real_t dotp = 2 * (x_ * p->x_ + y_ * p->y_ + z_ * p->z_);
    const real_t cross_mul = 2 * w_;
    const real_t vmul = cross_mul * w_ - real_t(1);
    const real_t x_val = p->x_;
    const real_t y_val = p->y_;
    const real_t z_val = p->z_;

    p->x_ = vmul * x_val + dotp * x_ + cross_mul * (y_ * z_val - z_ * y_val);
    p->y_ = vmul * y_val + dotp * y_ + cross_mul * (z_ * x_val - x_ * z_val);
    p->z_ = vmul * z_val + dotp * z_ + cross_mul * (x_ * y_val - y_ * x_val);
}

template<typename real_t>
inline
real_t
v8::math::quaternion<real_t>::length_squared() const {
    return w_ * w_ + x_ * x_ + y_ * y_ + z_ * z_;
}

template<typename real_t>
inline
real_t
v8::math::quaternion<real_t>::magnitude() const {
    return std::sqrt(length_squared());
}

template<typename real_t>
inline
bool
v8::math::quaternion<real_t>::is_zero() const {
    return math::operands_eq(real_t(0), length_squared());
}

template<typename real_t>
inline
bool
v8::math::quaternion<real_t>::is_unit() const {
    return math::operands_eq(real_t(1), length_squared());
}

template<typename real_t>
inline
bool
v8::math::quaternion<real_t>::is_identity() const {
    return math::operands_eq(real_t(1), w_) && 
           math::operands_eq(real_t(0), x_ * x_ + y_ * y_ + z_ * z_);
}

template<typename real_t>
void 
v8::math::quaternion<real_t>::extract_rotation_matrix(
    v8::math::matrix_4X4<real_t>* mtx
    ) const {
    real_t s, xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;

    s = 2 / length_squared();

    xs = s * x_;
    ys = s * y_;
    zs = s * z_;

    wx = w_ * xs;
    wy = w_ * ys;
    wz = w_ * zs;

    xx = x_ * xs;
    xy = x_ * ys;
    xz = x_ * zs;

    yy = y_ * ys;
    yz = y_ * zs;

    zz = z_ * zs;

    mtx->a11_ = real_t(1) - (yy + zz);
    mtx->a12_ = xy - wz;
    mtx->a13_ = xz + wy;
    mtx->a14_ = real_t(0);

    mtx->a21_ = xy + wz;
    mtx->a22_ = real_t(1) - (xx + zz);
    mtx->a23_ = yz - wx;
    mtx->a24_ = real_t(0);

    mtx->a31_ = xz - wy;
    mtx->a32_ = yz + wx;
    mtx->a33_ = real_t(1) - (xx + yy);
    mtx->a34_ = real_t(0);

    mtx->a41_ = mtx->a42_ = mtx->a43_ = real_t(0);
    mtx->a44_ = real_t(1);
}

template<typename real_t>
v8::math::quaternion<real_t>&
v8::math::quaternion<real_t>::extract_axis_angle(
    v8::math::vector3<real_t>* axis, 
    real_t* angle
    ) const {

    assert(is_unit() && "Quaternion must be of unit length!!");
    *angle = 2 * std::acos(w_);

    const real_t len_sq = real_t(1) - x_ * x_ + y_ * y_ + z_ * z_;
    const real_t scale_factor = real_t(1) / len_sq;
    axis->x_ = x_ * scale_factor;
    axis->y_ = y_ * scale_factor;
    axis->z_ = z_ * scale_factor;
    return *this;
}

template<typename real_t>
inline
real_t
v8::math::dot_product(
    const v8::math::quaternion<real_t>& lhs, 
    const v8::math::quaternion<real_t>& rhs
    ) {
    return lhs.x_ * rhs.x_ + lhs.y_ * rhs.y_ + lhs.z_ * rhs.z_ + lhs.w_ * rhs.w_;
}

template<typename real_t>
inline
v8::math::quaternion<real_t>
v8::math::inverse_of(
    const v8::math::quaternion<real_t>& quat
    ) {
    quaternion<real_t> result(quat);
    return result.invert();
}

template<typename real_t>
inline
v8::math::quaternion<real_t>
v8::math::normal_of(
    const v8::math::quaternion<real_t>& quat
    ) {
    quaternion<real_t> result(quat);
    return result.normalize();
}

template<typename real_t>
inline
v8::math::quaternion<real_t>
v8::math::conjugate_of(
    const v8::math::quaternion<real_t>& quat
    ) {
    quaternion<real_t> result(quat);
    return result.make_conjugate();
}

template<typename real_t>
bool
v8::math::operator==(
    const v8::math::quaternion<real_t>& lhs, 
    const v8::math::quaternion<real_t>& rhs
    ) {
    return math::operands_eq(lhs.w_, rhs.w_) && 
           math::operands_eq(lhs.x_, rhs.x_) &&
           math::operands_eq(lhs.y_, rhs.y_) &&
           math::operands_eq(lhs.z_, rhs.z_);
}

template<typename real_t>
inline
bool
v8::math::operator!=(
    const v8::math::quaternion<real_t>& lhs, 
    const v8::math::quaternion<real_t>& rhs
    ) {
    return !(lhs == rhs);
}

template<typename real_t>
inline
v8::math::quaternion<real_t>
v8::math::operator+(
    const v8::math::quaternion<real_t>& lhs, 
    const v8::math::quaternion<real_t>& rhs
    ) {
    quaternion<real_t> result(lhs);
    return result += rhs;
}

template<typename real_t>
inline
v8::math::quaternion<real_t>
v8::math::operator-(
    const v8::math::quaternion<real_t>& lhs, 
    const v8::math::quaternion<real_t>& rhs
    ) {
    quaternion<real_t> result(lhs);
    return result -= rhs;
}

template<typename real_t>
inline
v8::math::quaternion<real_t>
v8::math::operator-(
    const v8::math::quaternion<real_t>& quat
    ) {
    return quaternion<real_t>(-quat.w_, -quat.x_, -quat.y_, -quat.z_);
}

template<typename real_t>
v8::math::quaternion<real_t>
v8::math::operator*(
    const v8::math::quaternion<real_t>& lhs, 
    const v8::math::quaternion<real_t>& rhs
    ) {
    vector3<real_t> v1(lhs.x_, lhs.y_, lhs.z_);
    vector3<real_t> v2(rhs.x_, rhs.y_, rhs.z_);

    quaternion<real_t> result;
    result.w_ = lhs.w_ * rhs.w_ - dot_product(v1, v2);
    result.x_ = lhs.w_ * rhs.x_ + rhs.w_ * lhs.x_ + lhs.y_ * rhs.z_ - lhs.z_ * rhs.y_;
    result.y_ = lhs.w_ * rhs.y_ + rhs.w_ * lhs.y_ + lhs.z_ * rhs.x_ - lhs.x_ * rhs.z_;
    result.z_ = lhs.w_ * rhs.z_ + rhs.w_ * lhs.z_ + lhs.x_ * rhs.y_ - lhs.y_ * rhs.x_;

    return result;
}

template<typename real_t>
v8::math::quaternion<real_t>
v8::math::operator*(
    const v8::math::quaternion<real_t>& lhs, 
    const real_t scalar
    ) {
    quaternion<real_t> result(lhs);
    return lhs *= scalar;
}

template<typename real_t>
inline
v8::math::quaternion<real_t>
v8::math::operator*(
    const real_t scalar,
    const v8::math::quaternion<real_t>& rhs
    ) {
    return rhs * scalar;
}

template<typename real_t>
v8::math::quaternion<real_t>
v8::math::operator/(
    const v8::math::quaternion<real_t>& lhs,
    const real_t scalar    
    ) {
    quaternion<real_t> result(lhs);
    return result /= scalar;
}
