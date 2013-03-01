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
const v8::math::vector3<real_t> 
v8::math::vector3<real_t>::zero(real_t(0), real_t(0), real_t(0));

template<typename real_t>
const v8::math::vector3<real_t> 
v8::math::vector3<real_t>::unit_x(real_t(1), real_t(0), real_t(0));

template<typename real_t>
const v8::math::vector3<real_t> 
v8::math::vector3<real_t>::unit_y(real_t(0), real_t(1), real_t(0));

template<typename real_t>
const v8::math::vector3<real_t> 
v8::math::vector3<real_t>::unit_z(real_t(0), real_t(0), real_t(1));

template<typename real_t>
v8::math::vector3<real_t>::vector3(const real_t* input, size_t count) {
    std::memcpy(elements_, input, std::min(_countof(elements_), 
                count * sizeof(real_t)));
}

template<typename real_t>
template<typename Convertible_Type>
inline
v8::math::vector3<real_t>&
v8::math::vector3<real_t>::operator+=(
    const vector3<Convertible_Type>& rhs
    ) {
    x_ += rhs.x_;
    y_ += rhs.y_;
    z_ += rhs.z_;
    return *this;
}

template<typename real_t>
template<typename Convertible_Type>
inline
v8::math::vector3<real_t>&
v8::math::vector3<real_t>::operator-=(
    const vector3<Convertible_Type>& rhs
    ) 
{
    x_ -= rhs.x_;
    y_ -= rhs.y_;
    z_ -= rhs.z_;
    return *this;
}

template<typename real_t>
template<typename Convertible_Type>
inline
v8::math::vector3<real_t>&
v8::math::vector3<real_t>::operator *=(Convertible_Type k) {
    x_ *= k;
    y_ *= k;
    z_ *= k;
    return *this;
}

template<typename real_t>
template<typename Convertible_Type>
inline
v8::math::vector3<real_t>&
v8::math::vector3<real_t>::operator /=(Convertible_Type k) {
    using namespace v8::math::internals;
    typename div_wrap_t<real_t>::div_helper_t   div_val(k);

    x_ = div_val(x_); y_ = div_val(y_); z_ = div_val(z_);
    return *this;
}

template<typename real_t>
inline
real_t
v8::math::vector3<real_t>::sum_components_squared() const {
    return x_ * x_ + y_ * y_ + z_ * z_;
}

template<typename real_t>
inline
real_t
v8::math::vector3<real_t>::length_squared() const {
    return x_ * x_ + y_ * y_ + z_ * z_;
}

template<typename real_t>
inline
real_t
v8::math::vector3<real_t>::magnitude() const {
    return std::sqrt(sum_components_squared());
}

template<typename real_t>
inline
v8::math::vector3<real_t>&
v8::math::vector3<real_t>::normalize() {
    float magn(magnitude());
    if (math::is_zero(magn)) {
        x_ = y_ = z_ = real_t(0);
    } else {
        *this /= magn;
    }
    return *this;
}

template<typename real_t>
inline
bool
v8::math::vector3<real_t>::is_unit_length() const {
    return operands_eq(real_t(1), length_squared());
}

template<typename real_t>
inline
bool
v8::math::operator==(
    const v8::math::vector3<real_t>& lhs, 
    const v8::math::vector3<real_t>& rhs
    ) {
    return  math::operands_eq(lhs.x_, rhs.x_) &&
            math::operands_eq(lhs.y_, rhs.y_) &&
            math::operands_eq(lhs.z_, rhs.z_);
}

template<typename real_t>
inline
bool
v8::math::operator!=(
    const v8::math::vector3<real_t>& lhs, 
    const v8::math::vector3<real_t>& rhs
    ) {
    return !(lhs == rhs);
}

template<typename real_t>
inline
v8::math::vector3<real_t>
v8::math::operator+(
    const v8::math::vector3<real_t>& lhs, 
    const v8::math::vector3<real_t>& rhs
    ) {
    vector3<real_t> res(lhs);
    res += rhs;
    return res;
}

template<typename real_t>
inline
v8::math::vector3<real_t>
v8::math::operator-(
    const v8::math::vector3<real_t>& lhs, 
    const vector3<real_t>& rhs
    ) {
    vector3<real_t> res(lhs);
    res -= rhs;
    return res;
}

template<typename real_t>
inline
v8::math::vector3<real_t>
v8::math::operator-(const v8::math::vector3<real_t>& vec) {
    return vector3<real_t>(-vec.x_, -vec.y_, -vec.z_);
}

template<typename real_t, typename Convertible_Type>
inline
v8::math::vector3<real_t>
v8::math::operator*(
    const v8::math::vector3<real_t>& vec, 
    Convertible_Type k
    ) {
    vector3<real_t> result = vec;
    result *= k;
    return result;
}

template<typename real_t, typename Convertible_Type>
inline
v8::math::vector3<real_t>
v8::math::operator*(
    Convertible_Type k, 
    const v8::math::vector3<real_t>& vec
    ) {
    return vec * k;
}

template<typename real_t, typename Convertible_Type>
inline
v8::math::vector3<real_t>
v8::math::operator/(
    const v8::math::vector3<real_t>& vec, 
    Convertible_Type k
    ) {
    v8::math::vector3<real_t> result(vec);
    result /= k;
    return result;
}

template<typename real_t>
inline
real_t
v8::math::dot_product(
    const v8::math::vector3<real_t>& lhs, 
    const v8::math::vector3<real_t>& rhs
    ) {
    return lhs.x_ * rhs.x_ + lhs.y_ * rhs.y_ + lhs.z_ * rhs.z_;
}

template<typename real_t>
inline
v8::math::vector3<real_t>
v8::math::cross_product(
    const v8::math::vector3<real_t>& lhs, 
    const v8::math::vector3<real_t>& rhs
    ) {
    return vector3<real_t>(
        lhs.y_ * rhs.z_ - lhs.z_ * rhs.y_,
        lhs.z_ * rhs.x_ - lhs.x_ * rhs.z_,
        lhs.x_ * rhs.y_ - lhs.y_ * rhs.x_
        );
}

template<typename real_t>
inline
bool
v8::math::ortho_test(
    const v8::math::vector3<real_t>& lhs, 
    const v8::math::vector3<real_t>& rhs
    )
{
    return math::is_zero(dot_product(lhs, rhs));
}

template<typename real_t>
inline
float
v8::math::angle_of(
    const v8::math::vector3<real_t>& lhs, 
    const v8::math::vector3<real_t>& rhs
    )
{
    return acos(dot_product(lhs, rhs) / (lhs.magnitude() * rhs.magnitude()));
}

template<typename real_t>
inline
v8::math::vector3<real_t>
v8::math::project_vector_on_vector(
    const v8::math::vector3<real_t>& lhs, 
    const v8::math::vector3<real_t>& rhs
    )
{
    return (dot_product(lhs, rhs) / rhs.sum_components_squared()) * rhs;
}

template<typename real_t>
inline
v8::math::vector3<real_t>
v8::math::project_vector_on_unit_vector(
    const v8::math::vector3<real_t>& lhs, 
    const v8::math::vector3<real_t>& rhs
    )
{
    return dot_product(lhs, rhs) * rhs;
}

template<typename real_t>
inline
v8::math::vector3<real_t>
v8::math::normal_of(
    const v8::math::vector3<real_t>& vec
    )
{
    vector3<real_t> res(vec);
    res.normalize();
    return res;
}

template<typename real_t>
inline
real_t
v8::math::triple_scalar_product(
    const v8::math::vector3<real_t>& v1, 
    const v8::math::vector3<real_t>& v2, 
    const v8::math::vector3<real_t>& v3
    )
{
    return dot_product(v1, cross_product(v2, v3));
}

template<typename real_t>
inline
v8::math::vector3<real_t>
v8::math::triple_vector_product(
    const v8::math::vector3<real_t>& v1, 
    const v8::math::vector3<real_t>& v2, 
    const v8::math::vector3<real_t>& v3
    )
{
    return cross_product(v1, cross_product(v2, v3));
}

template<typename real_t>
inline
real_t
v8::math::distance_squared(
    const v8::math::vector3<real_t>& point1, 
    const v8::math::vector3<real_t>& point2
    )
{
    float px = point2.x_ - point1.x_;
    float py = point2.y_ - point1.y_;
    float pz = point2.z_ - point1.z_;
    return px * px + py * py + pz * pz;
}

template<typename real_t>
inline
real_t
v8::math::distance(
    const v8::math::vector3<real_t>& point1, 
    const v8::math::vector3<real_t>& point2
    )
{
    return sqrt(distance_squared(point1, point2));
}

template<typename real_t>
inline v8::math::vector3<real_t> 
v8::math::point_from_spherical_coordinates( 
    real_t delta, 
    real_t phi, 
    real_t theta
    ) {
    const real_t phi_rads = to_radians(phi);
    const real_t theta_rads = to_radians(theta);

    const real_t sin_phi = std::sin(phi_rads);
    const real_t sin_theta = std::sin(theta_rads);
    const real_t cos_phi = std::cos(phi_rads);
    const real_t cos_theta = std::cos(theta_rads);

    return vector3<real_t>(
        delta * sin_phi * sin_theta,
        delta * cos_phi,
        delta * sin_phi * cos_theta
        );
}

template<typename real_t>
inline v8::math::vector3<real_t>
v8::math::point_to_spherical_coordinates(
    const vector3<real_t>& pt
    )
{
    const real_t delta = pt.magnitude();
    const real_t phi = atan2(sqrt(pt.x_ * pt.x_ + pt.z_ * pt.z_), pt.y_);
    const real_t theta = atan2(pt.x_, pt.z_);

    return vector3<real_t>(delta, phi, theta);
}

template<typename real_t>
void v8::math::generate_complement_basis(
    const v8::math::vector3<real_t>& u_vec,
    v8::math::vector3<real_t>* v_vec,
    v8::math::vector3<real_t>* w_vec
    ) {
    using namespace internals;
    if (operands_ge(abs(u_vec.x_), abs(u_vec.y_))) {
        const real_t v_length = sqrt(u_vec.x_ * u_vec.x_ + u_vec.z_ * u_vec.z_);
        typename div_wrap_t<real_t>::div_helper_t div_len(v_length);

        v_vec->x_ = div_len(-u_vec.z_);
        v_vec->y_ = real_t(0);
        v_vec->z_ = div_len(u_vec.x_);

        w_vec->x_ = u_vec.y_ * v_vec->z_;
        w_vec->y_ = v_vec->x_ * u_vec.z_ - u_vec.x_ * v_vec->z_;
        w_vec->z_ = -v_vec->x_ * u_vec.y_;
    } else {
        const real_t v_length = sqrt(u_vec.y_ * u_vec.y_ + u_vec.z_ * u_vec.z_);
        typename div_wrap_t<real_t>::div_helper_t div_len(v_length);

        v_vec->x_ = real_t(0);
        v_vec->y_ = div_len(u_vec.z_);
        v_vec->z_ = div_len(-u_vec.y_);

        w_vec->x_ = u_vec.y_ * v_vec->z_ - u_vec.z_ * v_vec->y_;
        w_vec->y_ = -u_vec.x_ * v_vec->z_;
        w_vec->z_ = u_vec.x_ * v_vec->y_;
    }
}
