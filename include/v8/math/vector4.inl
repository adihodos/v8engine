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

//
// static
template<typename real_t>
inline 
v8::math::vector4<real_t> 
v8::math::vector4<real_t>::as_affine_point(
    const v8::math::vector3<real_t>& pt
    ) {
    return vector4_t(pt.x_, pt.y_, pt.z_, real_t(1));
}

//
// static
template<typename real_t>
inline 
v8::math::vector4<real_t> 
v8::math::vector4<real_t>::as_affine_vector(
    const v8::math::vector3<real_t>& v
    ) {
    return vector4_t(v.x_, v.y_, v.z_, real_t(0));
}

//
// static
template<typename real_t>
inline
v8::math::vector4<real_t>
v8::math::vector4<real_t>::as_homogeneous_point(
    const v8::math::vector3<real_t>& pt,
    real_t w
    ) {
    return vector4_t(pt.x_, pt.y_, pt.z_, w);
}

template<typename real_t>
inline v8::math::vector4<real_t>::vector4(
    const real_t* inputs, 
    size_t count
    ) {
    std::memcpy(elements_, inputs, 
                std::min(_countof(elements_), count) * sizeof(real_t));
}

template<typename real_t>
template<typename Convertible_Type>
inline 
v8::math::vector4<real_t>& 
v8::math::vector4<real_t>::operator+=(
    const vector4<Convertible_Type>& rhs
    ) {
    x_ += rhs.x_; y_ += rhs.y_; z_ += rhs.z_; w_ += rhs.w_;
    return *this;
}

template<typename real_t>
template<typename Convertible_Type>
inline
v8::math::vector4<real_t>& 
v8::math::vector4<real_t>::operator-=(
    const vector4<Convertible_Type>& rhs
    ) {
    x_ -= rhs.x_; y_ -= rhs.y_; z_ -= rhs.z_; w_ -= rhs.w_;
    return *this;
}

template<typename real_t>
template<typename Convertible_Type>
inline
v8::math::vector4<real_t>&
v8::math::vector4<real_t>::operator*=(
    Convertible_Type k
    ) {
    x_ *= k; y_ *= k; z_ *= k; w_ *= k;
    return *this;
}

template<typename real_t>
template<typename Convertible_Type>
inline
v8::math::vector4<real_t>&
v8::math::vector4<real_t>::operator/=(
    Convertible_Type k
    ) {
    typename internals::div_wrap_t<real_t>::div_helper_t div_val(k);

    x_ = div_val(x_);
    y_ = div_val(y_);
    z_ = div_val(z_);
    w_ = div_val(w_);
    return *this;
}

template<typename real_t>
inline
real_t
v8::math::vector4<real_t>::sum_components_squared() const {
    return x_ * x_ + y_ * y_ + z_ * z_ + w_ * w_;
}

template<typename real_t>
inline
real_t
v8::math::vector4<real_t>::length_squared() const {
    return x_ * x_ + y_ * y_ + z_ * z_ + w_ * w_;
}

template<typename real_t>
inline
real_t
v8::math::vector4<real_t>::magnitude() const {
    return std::sqrt(sum_components_squared());
}

template<typename real_t>
inline
v8::math::vector4<real_t>&
v8::math::vector4<real_t>::normalize() {
    real_t mag(magnitude());
    if (math::is_zero(mag)) {
        x_ = y_ = z_ = w_ = real_t(0);
    } else {
        *this /= mag;
    }
    return *this;
}

template<typename real_t>
inline
bool
v8::math::vector4<real_t>::is_unit_length() const {
    return operands_eq(real_t(1), length_squared());
}

template<typename real_t>
inline void
v8::math::vector4<real_t>::to_point3(v8::math::vector3<real_t>* p3) const {
    typename internals::div_wrap_t<real_t>::div_helper_t div_val(w_);
    p3->x_ = div_val(x_);
    p3->y_ = div_val(y_);
    p3->z_ = div_val(z_);
}

template<typename real_t>
inline 
void
v8::math::vector4<real_t>::to_vector3(
    v8::math::vector3<real_t>* v3) const {
    memcpy(v3->elements_, elements_, sizeof(v3->elements_));
}

template<typename real_t>
inline
bool
v8::math::operator==(
    const v8::math::vector4<real_t>& lhs,
    const v8::math::vector4<real_t>& rhs
    ) {    
    return  math::operands_eq(lhs.x_, rhs.x_) &&
            math::operands_eq(lhs.y_, rhs.y_) &&
            math::operands_eq(lhs.z_, rhs.z_) &&
            math::operands_eq(lhs.w_, rhs.w_);
}

template<typename real_t>
inline
bool
v8::math::operator!=(
    const v8::math::vector4<real_t>& lhs,
    const v8::math::vector4<real_t>& rhs
    ) {
    return !(lhs == rhs);
}

template<typename real_t>
inline
v8::math::vector4<real_t>
v8::math::operator-(
    const v8::math::vector4<real_t>& vec
    ) {
    return vector4<real_t>(-vec.x_, -vec.y_, -vec.z_, -vec.w_);
}

template<typename real_t>
inline
v8::math::vector4<real_t>
v8::math::operator+(
    const v8::math::vector4<real_t>& lhs,
    const v8::math::vector4<real_t>& rhs
    ) {
    return vector4<real_t>(lhs.x_ + rhs.x_, lhs.y_ + rhs.y_, 
                           lhs.z_ + rhs.z_, lhs.w_ + rhs.w_);
}

template<typename real_t>
inline
v8::math::vector4<real_t>
v8::math::operator-(
    const v8::math::vector4<real_t>& lhs,
    const v8::math::vector4<real_t>& rhs
    ) {
    return vector4<real_t>(lhs.x_ - rhs.x_, 
                           lhs.y_ - rhs.y_, 
                           lhs.z_ - rhs.z_, 
                           lhs.w_ - rhs.w_);
}

template<typename real_t, typename Convertible_Type>
inline
v8::math::vector4<real_t>
v8::math::operator*(
    Convertible_Type k,
    const v8::math::vector4<real_t>& vec
    ) {
    v8::math::vector4<real_t> res(vec);
    res *= k;
    return res;
}

template<typename real_t, typename Convertible_Type>
inline
v8::math::vector4<real_t>
v8::math::operator*(
    const v8::math::vector4<real_t>& vec,
    Convertible_Type k
    ) {
    return k * vec;
}

template<typename real_t, typename Convertible_Type>
inline
v8::math::vector4<real_t>
v8::math::operator/(
    const v8::math::vector4<real_t>& vec,
    Convertible_Type k
    ) {
    v8::math::vector4<real_t> res(vec);
    res /= k;
    return res;
}

template<typename real_t>
inline
v8::math::vector4<real_t>
v8::math::normalized_from(
    const v8::math::vector4<real_t>& vec
    ) {
    v8::math::vector4<real_t> res(vec);
    res.normalize();
    return res;
}

template<typename real_t>
inline
real_t
v8::math::dot_product(
    const v8::math::vector4<real_t>& lhs, 
    const v8::math::vector4<real_t>& rhs
    ) {
    return lhs.x_ * rhs.x_ + lhs.y_ * rhs.y_ + lhs.z_ * rhs.z_ 
           + lhs.w_ * rhs.w_;
}
