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

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>

#include <v8/v8.hpp>

#if defined(V8_COMPILER_IS_GCC) || defined(V8_COMPILER_IS_CLANG) \
    || defined(V8_COMPILER_IS_MINGW)
#include <initializer_list>
#endif

#include <v8/math/math_utils.hpp>

namespace v8 { namespace math {

/** \defgroup Algebra   Algebraic entities and helper functions.
 */

/** \addtogroup Algebra
 *  @{
 */

template<typename real_t>
class vector_N {
private :
    size_t      v_dimension_;
    real_t*     v_data_;
public :
    typedef vector_N<real_t> vector_N_t;

    explicit vector_N(size_t dimension)
        : v_dimension_(dimension), v_data_(new real_t[dimension]) {}

    vector_N(const real_t* data, size_t dimension) :
        v_dimension_(dimension),
        v_data_(new real_t[dimension]) {
        memcpy(v_data_, data, sizeof(real_t) * v_dimension_);
    }

    vector_N(const vector_N_t& other)
        : v_dimension_(other.v_dimension_),
          v_data_(new real_t[other.v_dimension_]) {
        memcpy(v_data_, other.v_data_, sizeof(real_t) * v_dimension_);
    }

    vector_N(vector_N_t&& other)
        : v_dimension_(other.v_dimension_),
          v_data_(other.v_data_) {
        other.v_data_ = nullptr;
    }

#if defined(V8_COMPILER_IS_GCC) || defined(V8_COMPILER_IS_CLANG) \
    || defined(V8_COMPILER_IS_MINGW)
    vector_N(std::initializer_list<real_t> init_lst) {
        v_dimension_ = init_lst.size();
        v_data_ = new real_t[v_dimension_];
        memcpy(v_data_, init_lst.begin(), sizeof(real_t) * v_dimension_);
    }
#endif

    ~vector_N() {
        delete[] v_data_;
    }

    vector_N_t& operator=(const vector_N_t& rhs) {
        assert(v_dimension_ == rhs.v_dimension_);
        memcpy(v_data_, rhs.v_data_, sizeof(real_t) * v_dimension_);
        return *this;
    }

    vector_N_t& operator=(vector_N_t&& rhs) {
        assert(rhs.v_dimension_ == v_dimension_);
        real_t* tmp;
        tmp = rhs.v_data_;
        rhs.v_data_ = v_data_;
        v_data_ = tmp;
        return *this;
    }

    vector_N_t& operator+=(const vector_N_t& rhs) {
        assert(v_dimension_ == rhs.v_dimension_);

        for (size_t i = 0; i < v_dimension_; ++i)
            v_data_[i] += rhs.v_data_[i];
        return *this;
    }

    vector_N_t& operator-=(const vector_N_t& rhs) {
        assert(v_dimension_ == rhs.v_dimension_);

        for (size_t i = 0; i < v_dimension_; ++i)
            v_data_[i] -= rhs.v_data_[i];
        return *this;
    }

    vector_N_t& operator*=(real_t k) {
        for (size_t i = 0; i < v_dimension_; ++i)
            v_data_[i] *= k;
        return *this;
    }

    vector_N_t& operator/=(real_t k) {
        typename internals::div_wrap_t<real_t>::div_helper_t div_val(k);
        for (size_t i = 0; i < v_dimension_; ++i)
            v_data_[i] = div_val(v_data_[i]);
        return *this;
    }

    vector_N_t& operator^=(const vector_N_t& rhs) {
        assert(v_dimension_ == rhs.v_dimension_);

        for (size_t i = 0; i < v_dimension_; ++i)
            v_data_[i] *= rhs.v_data_[i];
        return *this;
    }

    real_t length_squared() const {
        real_t len_sq = real_t(0);
        for (size_t i = 0; i < v_dimension_; ++i)
            len_sq += v_data_[i] * v_data_[i];
        return len_sq;
    }

    real_t length() const {
        return sqrt(length_squared());
    }

    vector_N_t& normalize() {
        const real_t vec_mag = length();
        if (is_zero(vec_mag)) {
            memset(v_data_, 0, sizeof(real_t) * v_dimension_);
        } else {
            typename internals::div_wrap_t<real_t>::div_helper_t div_val(vec_mag);
            for (size_t i = 0; i < v_dimension_; ++i) {
                v_data_[i] = div_val(v_data_[i]);
            }
        }
        return *this;
    }

    real_t operator[](size_t i) const {
        assert(i > 0 && i <= v_dimension_);
        return v_data_[i - 1];
    }

    real_t& operator[](size_t i) {
        assert(i > 0 && i <= v_dimension_);
        return v_data_[i - 1];
    }

    size_t get_dimension() const {
        return v_dimension_;
    }

    void set_dimension(size_t new_dim) {
        if (new_dim > v_dimension_) {
            //
            // Reallocate to grow and copy old elements to new location
            real_t* new_data = new real_t[new_dim];
            memcpy(new_data, v_data_, sizeof(real_t) * v_dimension_);
            delete[] v_data_;
            v_data_ = new_data;
        }
        v_dimension_ = new_dim;
    }

    real_t* get_data() NOEXCEPT {
        return v_data_;
    }

    const real_t* get_data() const NOEXCEPT {
        return v_data_;
    }

    void assign(const real_t* data) {
        memcpy(v_data_, data, v_dimension_ * sizeof(real_t));
    }
};

template<typename real_t>
inline bool operator==(const vector_N<real_t>& v0, const vector_N<real_t>& v1) {
    assert(v0.get_dimension() == v1.get_dimension());
    const real_t* v0_data = v0.get_data();
    const real_t* v1_data = v1.get_data();

    for (size_t i = 0; i < v0.get_dimension(); ++i) {
        if (!operands_eq(v0_data[i], v1_data[i]))
            return false;
    }
    return true;
}

template<typename real_t>
inline bool operator!=(const vector_N<real_t>& v0, const vector_N<real_t>& v1) {
    return !(v0 == v1);
}

template<typename real_t>
inline vector_N<real_t> operator-(const vector_N<real_t>& in_vec) {
    vector_N<real_t> out_vec(in_vec.get_dimension());
    const real_t* in_data = in_vec.get_data();
    real_t* out_data = out_vec.get_data();

    for (size_t i = 0; i < in_vec.get_dimension(); ++i)
        out_data[i] = -in_data[i];
    return out_vec;
}

template<typename real_t>
inline vector_N<real_t>
operator+(const vector_N<real_t>& v0, const vector_N<real_t>& v1) {
    vector_N<real_t> result(v0);
    result += v1;
    return result;
}

template<typename real_t>
inline vector_N<real_t>
operator-(const vector_N<real_t>& v0, const vector_N<real_t>& v1) {
    vector_N<real_t> result(v0);
    result -= v1;
    return result;
}

template<typename real_t>
inline vector_N<real_t> operator*(const vector_N<real_t>& v0, real_t k) {
    vector_N<real_t> result(v0);
    result *= k;
    return result;
}

template<typename real_t>
inline vector_N<real_t> operator*(real_t k, const vector_N<real_t>& v0) {
    return v0 * k;
}

template<typename real_t>
inline vector_N<real_t> operator/(const vector_N<real_t>& v0, real_t k) {
    vector_N<real_t> result(v0);
    result /= k;
    return result;
}

template<typename real_t>
inline vector_N<real_t> operator^(const vector_N<real_t>& v0,
                                  const vector_N<real_t>& v1) {
    vector_N<real_t> result(v0);
    result ^= v1;
    return result;
}

template<typename real_t>
inline real_t dot_product(const vector_N<real_t>& v0,
                          const vector_N<real_t>& v1) {
    assert(v0.get_dimension() == v1.get_dimension());
    const real_t* v0_data = v0.get_data();
    const real_t* v1_data = v1.get_data();
    real_t dotp_value = real_t(0);

    for (size_t i = 0; i < v0.get_dimension(); ++i) {
        dotp_value += v0_data[i] * v1_data[i];
    }
    return dotp_value;
}

/** @} */

} // namespace math
} // namespace v8
