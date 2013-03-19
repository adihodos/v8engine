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
inline v8::math::matrix_2X2<real_t>::matrix_2X2(
    real_t a11, 
    real_t a12, 
    real_t a21, 
    real_t a22
    ) {
    a11_ = a11; a12_ = a12; a21_ = a21; a22_ = a22;
}

template<typename real_t>
inline v8::math::matrix_2X2<real_t>::matrix_2X2(real_t a11, real_t a22) {
    a11_ = a11; a12_ = a21_ = real_t(0); a22_ = a22;
}

template<typename real_t>
inline v8::math::matrix_2X2<real_t>::matrix_2X2( 
    const v8::math::vector2<real_t>& v1, 
    const v8::math::vector2<real_t>& v2, 
    bool column /* = true */ 
    ) {
    if (column) {
        a11_ = v1.x_;
        a21_ = v1.y_;
        a12_ = v2.x_;
        a22_ = v2.y_;
    } else {
        a11_ = v1.x_;
        a12_ = v1.y_;
        a21_ = v2.x_;
        a22_ = v2.y_;
    }
}

template<typename real_t>
inline v8::math::matrix_2X2<real_t>::matrix_2X2(
    const real_t* input, size_t count
    ) {
    memcpy(elements_, input, min((size_t) 4, count) * sizeof(real_t));
}

template<typename Real_Type>
template<typename Convertible_Type>
inline v8::math::matrix_2X2<Real_Type>::matrix_2X2(
    const v8::math::matrix_2X2<Convertible_Type>& other
    ) {
    for (uint32_t i = 0; i < v8::base::count_of_array(elements_); ++i)
        elements_[i] = other.elements_[i];
}

template<typename Real_Type>
template<typename Convertible_Type>
inline v8::math::matrix_2X2<Real_Type>::matrix_2X2(
    const Convertible_Type (&arr)[4]
    ) {
    a11_ = arr[0]; a12_ = arr[1];
    a21_ = arr[2]; a22_ = arr[3];
}

template<typename real_t>
inline v8::math::matrix_2X2<real_t>&
v8::math::matrix_2X2<real_t>::make_tensor_product( 
    const v8::math::vector2<real_t>& u, 
    const v8::math::vector2<real_t>& v 
    ) {
    a11_ = u.x_ * v.x_;
    a12_ = u.x_ * v.y_;
    a21_ = u.y_ * v.x_; 
    a22_ = u.y_ * v.y_;
    return *this;
}

template<typename real_t>
inline v8::math::matrix_2X2<real_t>&
v8::math::matrix_2X2<real_t>::make_rotation(float theta) {
    const real_t sin_theta = sin(theta);
    const real_t cos_theta = cos(theta);
    a11_ = cos_theta; a12_ = -sin_theta;
    a21_ = sin_theta; a22_ = cos_theta;
    return *this;
}

template<typename real_t>
inline v8::math::matrix_2X2<real_t>&
v8::math::matrix_2X2<real_t>::make_scale(real_t sx, real_t sy) {
    a11_ = sx; a12_ = real_t(0);
    a21_ = real_t(0); a22_ = sy;
    return *this;
}

template<typename real_t>
inline v8::math::matrix_2X2<real_t>&
v8::math::matrix_2X2<real_t>::make_scale(const vector2<real_t>& vscale) {
    return make_scale(vscale.x_, vscale.y_);
}

template<typename real_t>
inline v8::math::matrix_2X2<real_t>&
v8::math::matrix_2X2<real_t>::make_skew_symmetric(real_t val) {
    a11_ = real_t(0); a12_ = val;
    a21_ = -val; a22_ = real_t(0);
    return *this;
}

template<typename Real_Type>
inline v8::math::matrix_2X2<Real_Type>&
v8::math::matrix_2X2<Real_Type>::make_symmetric(Real_Type val) {
    a11_ = a22_ = Real_Type(0);
    a12_ = a21_ = val;
    return *this;
}

template<typename Real_Type>
inline v8::math::matrix_2X2<Real_Type>&
v8::math::matrix_2X2<Real_Type>::make_identity() {
    return make_scale(Real_Type(1), Real_Type(1));
}

template<typename Real_Type>
inline v8::math::matrix_2X2<Real_Type>&
v8::math::matrix_2X2<Real_Type>::make_zero() {
    return make_scale(Real_Type(0), Real_Type(0));
}

template<typename Real_Type>
template<typename Convertible_Type>
inline v8::math::matrix_2X2<Real_Type>& 
v8::math::matrix_2X2<Real_Type>::operator=(
    const v8::math::matrix_2X2<Convertible_Type>& other
    ) {
    for (uint32_t i = 0; i < v8::base::count_of_array(elements_); ++i) {
        elements_[i] = other.elements_[i];
    }
}

template<typename real_t>
template<typename Convertible_Type>
v8::math::matrix_2X2<real_t>& v8::math::matrix_2X2<real_t>::operator +=(
    const v8::math::matrix_2X2<Convertible_Type>& right
    ) {
    a11_ += right.a11_; a12_ += right.a12_;
    a21_ += right.a21_; a22_ += right.a22_;
    return *this;
}

template<typename real_t>
template<typename Convertible>
v8::math::matrix_2X2<real_t>& v8::math::matrix_2X2<real_t>::operator -=(
    const v8::math::matrix_2X2<Convertible>& right
    ) {
    a11_ -= right.a11_; a12_ -= right.a12_;
    a21_ -= right.a21_; a22_ -= right.a22_;
    return *this;
}

template<typename real_t>
template<typename Convertible>
v8::math::matrix_2X2<real_t>& v8::math::matrix_2X2<real_t>::operator *=(
    Convertible k
    ) {
    a11_ *= k; a12_ *= k;
    a21_ *= k; a22_ *= k;
    return *this;
}

template<typename real_t>
template<typename Convertible>
v8::math::matrix_2X2<real_t>& v8::math::matrix_2X2<real_t>::operator /=(
    Convertible k
    ) {
    using namespace internals;
    typename div_wrap_t<real_t>::div_helper_t   div_val(k);

    a11_ = div_val(a11_); a12_ = div_val(a12_);
    a21_ = div_val(a21_); a22_ = div_val(a22_);
    return *this;
}

template<typename real_t>
v8::math::matrix_2X2<real_t>& v8::math::matrix_2X2<real_t>::invert() {
    const real_t det_val = determinant();

    using namespace internals;
    typename div_wrap_t<real_t>::div_helper_t   div_val(det_val);

    real_t tmp = a11_;
    a11_ = div_val(a22_);
    a22_ = div_val(tmp);

    a12_ = -div_val(a12_);
    a21_ = -div_val(a21_);

    return *this;
}

template<typename Real_Type>
v8::math::matrix_2X2<Real_Type>&
v8::math::matrix_2X2<Real_Type>::ortho_normalize() {
    typename internals::div_wrap_t<Real_Type>::div_helper_t div_val(
                sqrt(a11_ * a11_ + a21_ * a21_));

    a11_ = div_val(a11_); a21_ = div_val(a21_);

    a12_ -= a12_ * a11_ * a11_;
    a22_ -= a22_ * a21_ * a21_;

    div_val.set_divisor(sqrt(a12_ * a12_ + a22_ * a22_));

    a12_ = div_val(a12_); a22_ = div_val(a22_);
    return *this;
}

template<typename Real_Type>
bool v8::math::operator ==(
    const v8::math::matrix_2X2<Real_Type>& lhs,
    const v8::math::matrix_2X2<Real_Type>& rhs
    ) {
    for (int i = 0; i < 4; ++i) {
        if (!operands_eq(lhs.elements_[i], rhs.elements_[i]))
            return false;
    }
    return true;
}

template<typename Real_Type>
inline bool v8::math::operator !=(
    const v8::math::matrix_2X2<Real_Type>& lhs,
    const v8::math::matrix_2X2<Real_Type>& rhs
    ) {
    return !(lhs == rhs);
}

template<typename real_t>
inline v8::math::matrix_2X2<real_t>
v8::math::adjoint_of(const v8::math::matrix_2X2<real_t>& mtx) {
    return matrix_2X2<real_t>(mtx.a22_, -mtx.a12_, -mtx.a21_, mtx.a11_);
}

template<typename real_t>
inline
v8::math::matrix_2X2<real_t>
v8::math::transpose_of(const v8::math::matrix_2X2<real_t>& mtx) {
    matrix_2X2<real_t> other(mtx);
    return other.transpose();
}

template<typename real_t>
inline
v8::math::matrix_2X2<real_t>
v8::math::inverse_of(const v8::math::matrix_2X2<real_t>& mtx) {
    matrix_2X2<real_t> other(mtx);
    return other.invert();
}

template<typename real_t>
v8::math::matrix_2X2<real_t>
v8::math::multiply_transpose(
    const v8::math::matrix_2X2<real_t>& lhs, 
    const v8::math::matrix_2X2<real_t>& rhs
    ) {
    return matrix_2X2<real_t>(
        lhs.a11_ * rhs.a11_ + lhs.a12_ * rhs.a12_,
        lhs.a11_ * rhs.a21_ + lhs.a12_ * rhs.a22_,
        lhs.a21_ * rhs.a11_ + lhs.a22_ * rhs.a12_,
        lhs.a21_ * rhs.a21_ + lhs.a22_ * rhs.a22_
        );
}

template<typename real_t>
v8::math::matrix_2X2<real_t>
v8::math::transpose_multiply(
    const v8::math::matrix_2X2<real_t>& lhs, 
    const v8::math::matrix_2X2<real_t>& rhs
    ) {
    return matrix_2X2<real_t>(
        lhs.a11_ * rhs.a11_ + lhs.a21_ * rhs.a21_,
        lhs.a11_ * rhs.a12_ + lhs.a21_ * rhs.a22_,
        lhs.a12_ * rhs.a11_ + lhs.a22_ * rhs.a21_,
        lhs.a12_ * rhs.a12_ + lhs.a22_ * rhs.a22_
        );
}

template<typename real_t>
v8::math::matrix_2X2<real_t>
v8::math::transpose_multiply_transpose(
    const v8::math::matrix_2X2<real_t>& lhs, 
    const v8::math::matrix_2X2<real_t>& rhs
    ) {
    return matrix_2X2<real_t>(
        lhs.a11_ * rhs.a11_ + lhs.a21_ * rhs.a12_,
        lhs.a11_ * rhs.a21_ + lhs.a21_ * rhs.a22_,
        lhs.a12_ * rhs.a11_ + lhs.a22_ * rhs.a12_,
        lhs.a12_ * rhs.a21_ + lhs.a22_ * rhs.a22_
        );
}

template<typename real_t>
inline
v8::math::matrix_2X2<real_t>
v8::math::operator+(
    const v8::math::matrix_2X2<real_t>& lhs,
    const v8::math::matrix_2X2<real_t>& rhs
    ) {
    matrix_2X2<real_t> result(lhs);
    return result += rhs;
}

template<typename real_t>
inline
v8::math::matrix_2X2<real_t>
v8::math::operator-(
    const v8::math::matrix_2X2<real_t>& lhs,
    const v8::math::matrix_2X2<real_t>& rhs
    ) {
    matrix_2X2<real_t> result(lhs);
    return result -= rhs;
}

template<typename real_t>
v8::math::matrix_2X2<real_t>
v8::math::operator*(
    const v8::math::matrix_2X2<real_t>& lhs,
    const v8::math::matrix_2X2<real_t>& rhs
    ) {
    return matrix_2X2<real_t>(
        lhs.a11_ * rhs.a11_ + lhs.a12_ * rhs.a21_,
        lhs.a11_ * rhs.a12_ + lhs.a12_ * rhs.a22_,
        lhs.a21_ * rhs.a11_ + lhs.a22_ * rhs.a21_,
        lhs.a21_ * rhs.a12_ + lhs.a22_ * rhs.a22_
        );
}

template<typename real_t, typename convertible_type>
inline
v8::math::matrix_2X2<real_t>
v8::math::operator*(
    convertible_type k,
    const v8::math::matrix_2X2<real_t>& rhs
    ) {
    matrix_2X2<real_t> result(rhs);
    return result *= k;
}

template<typename real_t, typename convertible_type>
inline
v8::math::matrix_2X2<real_t>
v8::math::operator*(
    const v8::math::matrix_2X2<real_t>& lhs,
    convertible_type k
    ) {
    return k * lhs;
}

template<typename real_t, typename convertible_type>
inline
v8::math::matrix_2X2<real_t>
v8::math::operator/(
    const v8::math::matrix_2X2<real_t>& lhs,
    convertible_type k
    ) {
    matrix_2X2<real_t> result(lhs);
    return result /= k;
}

template<typename real_t>
inline
v8::math::vector2<real_t>
v8::math::operator*(
    const v8::math::matrix_2X2<real_t>& mtx,
    const v8::math::vector2<real_t>& vec
    ) {
    return vector2<real_t>(
        mtx.a11_ * vec.x_ + mtx.a12_ * vec.y_,
        mtx.a21_ * vec.x_ + mtx.a22_ * vec.y_
        );
}
