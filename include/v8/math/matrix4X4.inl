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
v8::math::matrix_4X4<real_t>::matrix_4X4(
    real_t a11, real_t a12, real_t a13, real_t a14,
    real_t a21, real_t a22, real_t a23, real_t a24,
    real_t a31, real_t a32, real_t a33, real_t a34,
    real_t a41, real_t a42, real_t a43, real_t a44
    ) {
    a11_ = a11; a12_ = a12; a13_ = a13; a14_ = a14;
    a21_ = a21; a22_ = a22; a23_ = a23; a24_ = a24;
    a31_ = a31; a32_ = a32; a33_ = a33; a34_ = a34;
    a41_ = a41; a42_ = a42; a43_ = a43; a44_ = a44;
}

template<typename real_t>
v8::math::matrix_4X4<real_t>::matrix_4X4(
    real_t a11, 
    real_t a22, 
    real_t a33,
    real_t a44
    ) {
    a11_ = a11; a12_ = real_t(0); a13_ = real_t(0); a14_ = real_t(0);
    a21_ = real_t(0); a22_ = a22; a23_ = real_t(0); a24_ = real_t(0);
    a31_ = real_t(0); a32_ = real_t(0); a33_ = a33; a34_ = real_t(0);
    a41_ = real_t(0); a42_ = real_t(0); a43_ = real_t(0); a44_ = a44;
}

template<typename real_t>
v8::math::matrix_4X4<real_t>::matrix_4X4(
    const v8::math::vector4<real_t>& v1,
    const v8::math::vector4<real_t>& v2,
    const v8::math::vector4<real_t>& v3,
    const v8::math::vector4<real_t>& v4,
    bool column
    ) {
    if (column) {
        a11_ = v1.x_;
        a12_ = v2.x_;
        a13_ = v3.x_;
        a14_ = v4.x_;

        a21_ = v1.y_;
        a22_ = v2.y_;
        a23_ = v3.y_;
        a24_ = v4.y_;

        a31_ = v1.z_;
        a32_ = v2.z_;
        a33_ = v3.z_;
        a34_ = v4.z_;

        a41_ = v1.w_;
        a42_ = v2.w_;
        a43_ = v3.w_;
        a44_ = v4.w_;
    } else {
        a11_ = v1.x_;
        a12_ = v1.y_;
        a13_ = v1.z_;
        a14_ = v1.w_;

        a21_ = v2.x_;
        a22_ = v2.y_;
        a23_ = v2.z_;
        a24_ = v2.w_;

        a31_ = v3.x_;
        a32_ = v3.y_;
        a33_ = v3.z_;
        a34_ = v3.w_;

        a41_ = v4.x_;
        a42_ = v4.y_;
        a43_ = v4.z_;
        a44_ = v4.w_;
    }
}

template<typename real_type>
template<typename convertible_type>
v8::math::matrix_4X4<real_type>::matrix_4X4(
    const matrix_3X3<convertible_type>& m_in
    ) {
    make_identity();
    a11_ = m_in.a11_; a12_ = m_in.a12_; a13_ = m_in.a13_;
    a21_ = m_in.a21_; a22_ = m_in.a22_; a23_ = m_in.a23_;
    a31_ = m_in.a31_; a32_ = m_in.a32_; a33_ = m_in.a33_;
}

template<typename real_t>
v8::math::matrix_4X4<real_t>&
v8::math::matrix_4X4<real_t>::operator+=(const v8::math::matrix_4X4<real_t>& rhs) {
    for (size_t i = 0; i < v8::base::count_of_array(elements_); ++i)
        elements_[i] += rhs.elements_[i];
    return *this;
}

template<typename real_t>
v8::math::matrix_4X4<real_t>&
v8::math::matrix_4X4<real_t>::operator-=(const v8::math::matrix_4X4<real_t>& rhs) {
    for (size_t i = 0; i < v8::base::count_of_array(elements_); ++i)
        elements_[i] -= rhs.elements_[i];
    return *this;
}

template<typename real_t>
v8::math::matrix_4X4<real_t>&
v8::math::matrix_4X4<real_t>::operator*=(real_t k) {
    for (size_t i = 0; i < v8::base::count_of_array(elements_); ++i)
        elements_[i] *= k;
    return *this;
}

template<typename real_t>
v8::math::matrix_4X4<real_t>&
v8::math::matrix_4X4<real_t>::operator/=(real_t k) {
    using namespace internals;

    typename div_wrap_t<real_t>::div_helper_t div_val(k);
    
    for (size_t i = 0; i < v8::base::count_of_array(elements_); ++i)
        elements_[i] = div_val(elements_[i]);
    return *this;
}

template<typename real_t>
real_t
v8::math::matrix_4X4<real_t>::determinant() const {
    //
    // This computation is based on Laplace's theorem
    // which states that the value of a determinant is equal to the product of
    // the minor determinants formed with the elements of p rows/columns and
    // their algebraic complements.
    const real_t k1 = a11_ * a22_ - a12_ * a21_;
    const real_t l1 = a33_ * a44_ - a34_ * a43_;

    const real_t k2 = a11_ * a23_ - a13_ * a21_;
    const real_t l2 = a32_ * a44_ - a34_ * a42_;

    const real_t k3 = a11_ * a24_ - a14_ * a21_;
    const real_t l3 = a32_ * a43_ - a33_ * a42_;

    const real_t k4 = a12_ * a23_ - a13_ * a22_;
    const real_t l4 = a31_ * a44_ - a43_ * a41_;

    const real_t k5 = a12_ * a24_ - a14_ * a22_;
    const real_t l5 = a31_ * a43_ - a33_ * a41_;

    const real_t k6 = a13_ * a24_ - a14_ * a23_;
    const real_t l6 = a31_ * a42_ - a32_ * a41_;

    return k1 * l1 - k2 * l2 + k3 * l3 + k4 * l4 - k5 * l5 + k6 * l6;
}

template<typename real_t>
v8::math::matrix_4X4<real_t>&
v8::math::matrix_4X4<real_t>::invert() {
    const real_t kDetValue = determinant();
    assert(!math::operands_eq(real_t(0), kDetValue));

    matrix4X4_t adjoint;
    get_adjoint(&adjoint);
    adjoint /= kDetValue;
    memcpy(elements_, adjoint.elements_, sizeof(elements_));
    return *this;
}

template<typename real_t>
v8::math::matrix_4X4<real_t>&
v8::math::matrix_4X4<real_t>::transpose() {
    using namespace internals;
    
    swap(a12_, a21_);
    swap(a13_, a31_);
    swap(a14_, a41_);

    swap(a23_, a32_);
    swap(a24_, a42_);

    swap(a34_, a43_);

    return *this;
}

template<typename real_t>
void v8::math::matrix_4X4<real_t>::get_transpose(
    v8::math::matrix_4X4<real_t>* mx
    ) const {
    mx->a11_ = a11_; mx->a12_ = a21_; mx->a13_ = a31_; mx->a14_ = a41_;
    mx->a21_ = a12_; mx->a22_ = a22_; mx->a23_ = a32_; mx->a24_ = a42_;
    mx->a31_ = a13_; mx->a32_ = a23_; mx->a33_ = a33_; mx->a34_ = a43_;
    mx->a41_ = a14_; mx->a42_ = a24_; mx->a43_ = a34_; mx->a44_ = a44_;
}

template<typename real_t>
void v8::math::matrix_4X4<real_t>::get_adjoint(
    v8::math::matrix_4X4<real_t>* mx
    ) const {
    const real_t m1 = a33_ * a44_ - a34_ * a43_;
    const real_t m2 = a32_ * a44_ - a34_ * a42_;
    const real_t m3 = a32_ * a43_ - a33_ * a42_;
    const real_t m4 = a13_ * a24_ - a14_ * a23_;
    const real_t m5 = a12_ * a24_ - a14_ * a22_;
    const real_t m6 = a12_ * a23_ - a13_ * a22_;
    const real_t m7 = a31_ * a44_ - a34_ * a41_;
    const real_t m8 = a31_ * a43_ - a33_ * a41_;
    const real_t m9 = a23_ * a44_ - a24_ * a43_;
    const real_t m10 = a21_ * a44_ - a24_ * a41_;
    const real_t m11 = a21_ * a43_ - a23_ * a41_;
    const real_t m12 = a11_ * a24_ - a14_ * a21_;
    const real_t m13 = a11_ * a23_ - a13_ * a21_;
    const real_t m14 = a31_ * a42_ - a32_ * a41_;
    const real_t m15 = a11_ * a22_ - a12_ * a21_;
    const real_t m16 = a31_ * a42_ - a32_ * a41_;

    mx->a11_ = a22_ * m1 - a23_ * m2 + a24_ * m3;
    mx->a12_ = -a12_ * m1 + a13_ * m2 - a14_ * m3;
    mx->a13_ = a42_ * m4 - a43_ * m5 + a44_ * m6;
    mx->a14_ = -a32_ * m4 + a33_ * m5 - a34_ * m6;

    mx->a21_ = -a21_ * m1 + a23_ * m7 - a24_ * m8;
    mx->a22_ = a11_ * m1 - a13_ * m7 + a14_ * m8;
    mx->a23_ = -a11_ * m9 + a13_ * m10 - a14_ * m11;
    mx->a24_ = a31_ * m4 - a33_ * m12 + a34_ * m13;

    mx->a31_ = a21_ * m2 - a22_ * m7  + a24_ * m16;
    mx->a32_ = -a11_ * m2 + a12_ * m7 - a14_ * m16;
    mx->a33_ = a41_ * m5 - a42_ * m12 + a44_ * m15;
    mx->a34_ = -a31_ * m5 + a32_ * m12 - a34_ * m15;

    mx->a41_ = -a21_ * m3 + a22_ * m8 - a23_ * m14;
    mx->a42_ = a11_ * m3 - a12_ * m8 + a13_ * m14;
    mx->a43_ = -a41_ * m6 + a42_ * m13 - a43_ * m15;
    mx->a44_ = a31_ * m6 - a32_ * m13 + a33_ * m15;
}

template<typename real_t>
inline 
void v8::math::matrix_4X4<real_t>::get_inverse(
    v8::math::matrix_4X4<real_t>* mx
    ) const {
    const real_t det = determinant();
    assert(!math::operands_eq(real_t(0), det));

    get_adjoint(mx);
    *mx /= det;
}

template<typename real_t>
template<typename real_u>
v8::math::matrix_4X4<real_t>&
v8::math::matrix_4X4<real_t>::set_upper3x3(const real_u* data) {
    for (size_t i = 0; i < 3; ++i) {
        size_t row = i * 4;
        for (size_t j = 0; j < 3; ++j) {
            elements_[row + j] = data[i * 3 + j];
        }
    }
    return *this;
}

template<typename real_t>
template<typename real_u>
void
v8::math::matrix_4X4<real_t>::get_upper3x3(real_u* data) const {
    for (size_t i = 0; i < 3; ++i) {
        size_t row = i * 4;
        for (size_t j = 0; j < 3; ++j) {
            data[i * 3 + j] = elements_[row + j];
        }
    }
}

template<typename real_t>
v8::math::matrix_4X4<real_t>&
v8::math::matrix_4X4<real_t>::set_row(
    int row, real_t r1, real_t r2, real_t r3, real_t r4
    ) {
    const int idx = (row - 1) * 4;
    elements_[idx] = r1;
    elements_[idx + 1] = r2;
    elements_[idx + 2] = r3;
    elements_[idx + 3] = r4;
    return *this;
}

template<typename real_t>
v8::math::matrix_4X4<real_t>&
v8::math::matrix_4X4<real_t>::set_row(int row, const real_t* row_data) {
    real_t* row_ptr = elements_ + (row - 1) * 4;
    for (size_t i = 0; i < 4; ++i)
        row_ptr[i] = row_data[i];
    return *this;
}

template<typename real_t>
v8::math::matrix_4X4<real_t>&
v8::math::matrix_4X4<real_t>::set_column(
    int column, real_t r1, real_t r2, real_t r3, real_t r4
    ) {
    const int idx = column - 1;
    elements_[idx] = r1;
    elements_[idx + 4] = r2;
    elements_[idx + 8] = r3;
    elements_[idx + 12] = r4;
    return *this;
}

template<typename real_t>
v8::math::matrix_4X4<real_t>&
v8::math::matrix_4X4<real_t>::set_column(int column, const real_t* data) {
    for (size_t i = 0; i < 4; ++i)
        elements_[i * 4 + column - 1] = data[i];
    return *this;
}

template<typename real_t>
void
v8::math::matrix_4X4<real_t>::get_row(int row, real_t* data) const {
    const real_t* row_ptr = elements_ + (row - 1) * 4;
    for (size_t i = 0; i < 4; ++i)
        data[i] = row_ptr[i];
}

template<typename real_t>
void
v8::math::matrix_4X4<real_t>::get_column(int col, real_t* data) const {
    for (size_t i = 0; i < 4; ++i)
        data[i] = elements_[i * 4 + col - 1];
}

template<typename Real_Ty>
template<typename Real_Ty2>
const v8::math::matrix_4X4<Real_Ty>&
v8::math::matrix_4X4<Real_Ty>::transform_affine_vector(
    v8::math::vector3<Real_Ty2>* pvec
    ) const
{
    pvec->x_ = a11_ * pvec->x_ + a12_ * pvec->y_ + a13_ * pvec->z_;
    pvec->y_ = a21_ * pvec->x_ + a22_ * pvec->y_ + a23_ * pvec->z_;
    pvec->z_ = a31_ * pvec->x_ + a32_ * pvec->y_ + a33_ * pvec->z_;
    return *this;
}

template<typename Real_Ty>
template<typename Real_Ty2>
const v8::math::matrix_4X4<Real_Ty>&
v8::math::matrix_4X4<Real_Ty>::transform_affine_point(
    v8::math::vector3<Real_Ty2>* point
    ) const
{
    point->x_ = a11_ * point->x_ + a12_ * point->y_ + a13_ * point->z_ + a14_;
    point->y_ = a21_ * point->x_ + a22_ * point->y_ + a23_ * point->z_ + a24_;
    point->z_ = a31_ * point->x_ + a32_ * point->y_ + a33_ * point->z_ + a34_;
    return *this;
}

template<typename real_t>
const v8::math::matrix_4X4<real_t>&
v8::math::matrix_4X4<real_t>::transform_affine_vector(
    v8::math::vector4<real_t>* pvec
    ) const
{
    pvec->x_ = a11_ * pvec->x_ + a12_ * pvec->y_ + a13_ * pvec->z_;
    pvec->y_ = a21_ * pvec->x_ + a22_ * pvec->y_ + a23_ * pvec->z_;
    pvec->z_ = a31_ * pvec->x_ + a32_ * pvec->y_ + a33_ * pvec->z_;

    return *this;
}

template<typename real_t>
const v8::math::matrix_4X4<real_t>&
v8::math::matrix_4X4<real_t>::transform_affine_point(
    v8::math::vector4<real_t>* apt
    ) const
{
    apt->x_ = a11_ * apt->x_ + a12_ * apt->y_ + a13_ * apt->z_ + a14_;
    apt->y_ = a21_ * apt->x_ + a22_ * apt->y_ + a23_ * apt->z_ + a24_;
    apt->z_ = a31_ * apt->x_ + a32_ * apt->y_ + a33_ * apt->z_ + a34_;

    return *this;
}

template<typename real_t>
const v8::math::matrix_4X4<real_t>&
v8::math::matrix_4X4<real_t>::transform_homogeneous_point(
    v8::math::vector4<real_t>* hpt
    ) const
{
    hpt->x_ = a11_ * hpt->x_ + a12_ * hpt->y_ + a13_ * hpt->z_ + a14_ * hpt->w_;
    hpt->y_ = a21_ * hpt->x_ + a22_ * hpt->y_ + a23_ * hpt->z_ + a24_ * hpt->w_;
    hpt->z_ = a31_ * hpt->x_ + a32_ * hpt->y_ + a33_ * hpt->z_ + a34_ * hpt->w_;
    hpt->w_ = a41_ * hpt->x_ + a42_ * hpt->y_ + a43_ * hpt->z_ + a44_ * hpt->w_;

    return *this;
}

template<typename real_t>
bool
v8::math::operator==(
    const v8::math::matrix_4X4<real_t>& lhs,
    const v8::math::matrix_4X4<real_t>& rhs
        )
{
    for (size_t i = 0; i < v8::base::count_of_array(lhs.elements_); ++i) {
        if (!math::operands_eq(lhs.elements_[i], rhs.elements_[i]))
            return false;
    }
    return true;
}

template<typename real_t>
inline
bool
v8::math::operator!=(
    const v8::math::matrix_4X4<real_t>& lhs,
    const v8::math::matrix_4X4<real_t>& rhs
        )
{
    return !(lhs == rhs);
}


template<typename real_t>
inline
v8::math::matrix_4X4<real_t>
v8::math::operator+(
    const v8::math::matrix_4X4<real_t>& lhs,
    const v8::math::matrix_4X4<real_t>& rhs
    )
{
    v8::math::matrix_4X4<real_t> result(lhs);
    result += rhs;
    return result;
}

template<typename real_t>
inline
v8::math::matrix_4X4<real_t>
v8::math::operator-(
    const v8::math::matrix_4X4<real_t>& lhs,
    const v8::math::matrix_4X4<real_t>& rhs
    )
{
    v8::math::matrix_4X4<real_t> result(lhs);
    result -= rhs;
    return result;
}

template<typename real_t>
inline
v8::math::matrix_4X4<real_t>
v8::math::operator-(
    const v8::math::matrix_4X4<real_t>& mtx
    )
{
    return v8::math::matrix_4X4<real_t>(
        -mtx.a11_, -mtx.a12_, -mtx.a13_, -mtx.a14_,
        -mtx.a21_, -mtx.a22_, -mtx.a23_, -mtx.a24_,
        -mtx.a31_, -mtx.a32_, -mtx.a33_, -mtx.a34_,
        -mtx.a41_, -mtx.a42_, -mtx.a43_, -mtx.a44_
        );
}

template<typename real_t>
v8::math::matrix_4X4<real_t>
v8::math::operator*(
    const v8::math::matrix_4X4<real_t>& lhs,
    const v8::math::matrix_4X4<real_t>& rhs
    )
{
    v8::math::matrix_4X4<real_t> res;
    res.a11_ = lhs.a11_ * rhs.a11_ + lhs.a12_ * rhs.a21_ 
        + lhs.a13_ * rhs.a31_ + lhs.a14_ * rhs.a41_;
    res.a12_ = lhs.a11_ * rhs.a12_ + lhs.a12_ * rhs.a22_ 
        + lhs.a13_ * rhs.a32_ + lhs.a14_ * rhs.a42_;
    res.a13_ = lhs.a11_ * rhs.a13_ + lhs.a12_ * rhs.a23_ 
        + lhs.a13_ * rhs.a33_ + lhs.a14_ * rhs.a43_;
    res.a14_ = lhs.a11_ * rhs.a14_ + lhs.a12_ * rhs.a24_ 
        + lhs.a13_ * rhs.a34_ + lhs.a14_ * rhs.a44_;

    res.a21_ = lhs.a21_ * rhs.a11_ + lhs.a22_ * rhs.a21_ 
        + lhs.a23_ * rhs.a31_ + lhs.a24_ * rhs.a41_;
    res.a22_ = lhs.a21_ * rhs.a12_ + lhs.a22_ * rhs.a22_ 
        + lhs.a23_ * rhs.a32_ + lhs.a24_ * rhs.a42_;
    res.a23_ = lhs.a21_ * rhs.a13_ + lhs.a22_ * rhs.a23_ 
        + lhs.a23_ * rhs.a33_ + lhs.a24_ * rhs.a43_;
    res.a24_ = lhs.a21_ * rhs.a14_ + lhs.a22_ * rhs.a24_ 
        + lhs.a23_ * rhs.a34_ + lhs.a24_ * rhs.a44_;

    res.a31_ = lhs.a31_ * rhs.a11_ + lhs.a32_ * rhs.a21_ 
        + lhs.a33_ * rhs.a31_ + lhs.a34_ * rhs.a41_;
    res.a32_ = lhs.a31_ * rhs.a12_ + lhs.a32_ * rhs.a22_ 
        + lhs.a33_ * rhs.a32_ + lhs.a34_ * rhs.a42_;
    res.a33_ = lhs.a31_ * rhs.a13_ + lhs.a32_ * rhs.a23_ 
        + lhs.a33_ * rhs.a33_ + lhs.a34_ * rhs.a43_;
    res.a34_ = lhs.a31_ * rhs.a14_ + lhs.a32_ * rhs.a24_ 
        + lhs.a33_ * rhs.a34_ + lhs.a34_ * rhs.a44_;

    res.a41_ = lhs.a41_ * rhs.a11_ + lhs.a42_ * rhs.a21_ 
        + lhs.a43_ * rhs.a31_ + lhs.a44_ * rhs.a41_;
    res.a42_ = lhs.a41_ * rhs.a12_ + lhs.a42_ * rhs.a22_ 
        + lhs.a43_ * rhs.a32_ + lhs.a44_ * rhs.a42_;
    res.a43_ = lhs.a41_ * rhs.a13_ + lhs.a42_ * rhs.a23_ 
        + lhs.a43_ * rhs.a33_ + lhs.a44_ * rhs.a43_;
    res.a44_ = lhs.a41_ * rhs.a14_ + lhs.a42_ * rhs.a24_ 
        + lhs.a43_ * rhs.a34_ + lhs.a44_ * rhs.a44_;

    return res;
}

template<typename real_t>
inline
v8::math::matrix_4X4<real_t>
v8::math::operator*(
    float k,
    const v8::math::matrix_4X4<real_t>& mtx
    )
{
    v8::math::matrix_4X4<real_t> result(mtx);
    result *= k;
    return result;
}

template<typename real_t>
inline
v8::math::matrix_4X4<real_t>
v8::math::operator*(
    const v8::math::matrix_4X4<real_t>& mtx,
    real_t k
    )
{
    return k * mtx;
}

template<typename real_t>
inline
v8::math::matrix_4X4<real_t>
v8::math::operator/(
    const v8::math::matrix_4X4<real_t>& mtx,
    real_t k
    )
{
    v8::math::matrix_4X4<real_t> result(mtx);
    result /= k;
    return result; 
}

template<typename real_t>
inline
v8::math::matrix_4X4<real_t>&
v8::math::matrix_4X4<real_t>::make_zero() {
    memset(elements_, 0, sizeof(elements_));
    return *this;
}

template<typename real_t>
inline
v8::math::matrix_4X4<real_t>&
v8::math::matrix_4X4<real_t>::make_identity() {
    *this = matrix_4X4<real_t>::identity;
    return *this;
}

template<typename real_t>
v8::math::matrix_4X4<real_t>&
v8::math::matrix_4X4<real_t>::make_symmetric_perspective_projection_rh(
    real_t aspect_ratio,
    real_t vertical_fov,
    real_t d_min,
    real_t d_max,
    real_t ndc_min,
    real_t ndc_max
    ) {
    make_zero();

    const real_t d_val = real_t(1) / tan(vertical_fov / 2);
    const real_t inv_depth = real_t(1) / (d_max - d_min);

    a11_ = d_val / aspect_ratio;
    a22_ = d_val;
    a33_ = (d_min * ndc_min - d_max * ndc_max) * inv_depth;
    a34_ = d_min * d_max * (ndc_min - ndc_max) * inv_depth;
    a43_ = real_t(-1);

    return *this;
}

template<typename real_t>
v8::math::matrix_4X4<real_t>&
v8::math::matrix_4X4<real_t>::make_symmetric_perspective_infinity_rh(
    real_t aspect_ratio,
    real_t vertical_fov,
    real_t d_min,
    real_t ndc_min,
    real_t ndc_max
    ) {
    make_zero();

    const real_t d_val = real_t(1) / tan(vertical_fov / 2);
    a11_ = d_val / aspect_ratio;
    a22_ = d_val;
    a33_ = -ndc_max;
    a34_ = d_min * (ndc_min - ndc_max);
    a43_ = real_t(-1);

    return *this;
}

template<typename real_t>
v8::math::matrix_4X4<real_t>&
v8::math::matrix_4X4<real_t>::make_perspective_projection_rh(
    real_t r_min,
    real_t r_max,
    real_t u_min,
    real_t u_max,
    real_t d_min,
    real_t d_max,
    real_t ndc_min,
    real_t ndc_max
    ) {
    make_zero();

    const real_t inv_r_diff = real_t(1) / (r_max - r_min);
    const real_t r_sum = r_max + r_min;
    const real_t inv_u_diff = real_t(1) / (u_max - u_min);
    const real_t u_sum = u_max + u_min;
    const real_t inv_depth = real_t(1) / (d_max - d_min);

    a11_ = real_t(2) * d_min * inv_r_diff;
    a13_ = -(r_sum) * inv_r_diff;

    a22_ = real_t(2) * d_min * inv_r_diff;
    a23_ = -(u_sum) * inv_u_diff;

    a33_ = (d_min * ndc_min - d_max * ndc_max) * inv_depth;
    a34_ = d_min * d_max * (ndc_min - ndc_max) * inv_depth;

    a43_ = real_t(-1);

    return *this;
}
