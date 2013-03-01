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
#include <cstring>

#include <v8/v8.hpp>
#include <v8/math/math_utils.hpp>

namespace v8 { namespace math {

/*! \addtogroup Algebra
 *  @{
 */

/*!
 * \brief An arbitrary degree polynomial with real coefficients.
 */
template<typename real_t>    
class polynomial {
private :

    //! Coefficients array, stored from a0 ... an
    real_t*  coefficients_;
    //! Number of allocated entries (smallest multiple of 4 that is greater
    //! than the polynomial's degree
    int     allocated_;
    //! Polynomial's degree.
    int     degree_;
    
    /*!
     * \brief Resizes the coefficients array.
     * \param size New size.
     * \param keep_data If true, old values will be copied after resizing.
     */
    void resize(int size, bool keep_data) {
        //
        // round up allocations to a multiple of 4
        const int to_alloc = (size + 4) & ~3;
        //
        // Have enough capacity, return
        if (to_alloc <= allocated_)
            return;
        
        real_t* new_data = new real_t[to_alloc];
        if (coefficients_ && keep_data) {
            memcpy(new_data, coefficients_, sizeof(real_t) * (degree_ + 1));
        }
        delete[] coefficients_;
        coefficients_ = new_data;
        allocated_ = to_alloc;
    }

    template<typename BinaryOp>
    void transform_poly(
        const polynomial<real_t>& rhs, BinaryOp bin_op
        ) NOEXCEPT;    
public :
    typedef polynomial<real_t>      poly_t;

    /*!
     * \brief   Default constructor. Leaves object un-initialized. Make sure
     *          that assign is called before attempting to call any other
     *          functions.
     */
    polynomial() : coefficients_(nullptr), allocated_(false), degree_(-1) {}

    /*!
     * \brief Constructs a zero degree polynomial (a constant polynomial)
     */
    polynomial(real_t a) 
        : coefficients_(nullptr), allocated_(false), degree_(0) {
        resize(1, false);
        coefficients_[0] = a;
    }
    
    /*!
     * \brief Constructs a first degree poly (ax + b)
     */
    polynomial(real_t a, real_t b) 
        : coefficients_(nullptr), allocated_(false), degree_(1) {
        resize(2, false);
        coefficients_[0] = b;
        coefficients_[1] = a;
    }
    
    /*!
     * \brief Construct a 2nd degree poly (ax^2 + bx + c)
     */
    polynomial(real_t a, real_t b, real_t c) 
        : coefficients_(nullptr), allocated_(false), degree_(2) {
        resize(3, false);
        coefficients_[0] = c;
        coefficients_[1] = b;
        coefficients_[2] = a;
    }

    /*!
     * \brief Constructs a cubic polynomial (ax^3 + bx^2 + cx + d)
     */
    polynomial(real_t a, real_t b, real_t c, real_t d) 
        : coefficients_(nullptr), allocated_(false), degree_(3) {
        resize(4, false);
        coefficients_[0] = d;
        coefficients_[1] = c;
        coefficients_[2] = b;
        coefficients_[3] = a;
    }

    /*!
     * \brief Constructs a quartic polynomial (ax^4 + bx^3 + cx^2 + dx + e)
     */
    polynomial(real_t a, real_t b, real_t c, real_t d, real_t e) 
        : coefficients_(nullptr), allocated_(false), degree_(4) {
        coefficients_[0] = e;
        coefficients_[1] = d;
        coefficients_[2] = c;
        coefficients_[3] = b;
        coefficients_[4] = a;
    }

    /*!
     * \brief   Constructs from an array of existing values. The first entry is
     *          a0, second a1, ... last an.
     * \param init_data Array of existing values.
     * \param data_size Array size.
     */
    polynomial(const real_t* init_data, size_t data_size)
        : coefficients_(nullptr), allocated_(false), degree_(-1) {
        assign(init_data, data_size);
    }
    
    polynomial(const poly_t& rhs) 
        : coefficients_(nullptr), allocated_(false), degree_(rhs.degree_) {
        resize(degree_ + 1, false);
        if (degree_) {
            memcpy(coefficients_, rhs.coefficients_, 
                   sizeof(real_t) * (degree_ + 1));
        }        
    }
    
    polynomial(poly_t&& rhs) {
        memcpy(this, &rhs, sizeof(*this));
        rhs.coefficients_ = nullptr;
    }
    
    ~polynomial() {
        delete[] coefficients_;
    }
    
    poly_t& operator=(const poly_t& rhs) {
        if (this != &rhs) {
            if (degree_ < rhs.degree_) {
                resize(rhs.degree_ + 1, false);
            }
            
            degree_ = rhs.degree_;
            memcpy(coefficients_, rhs.coefficients_, 
                   (degree_ + 1) * sizeof(real_t));
        }
        return *this;
    }
    
    poly_t& operator=(poly_t&& rhs) {
        memcpy(this, &rhs, sizeof(*this));
        rhs.coefficients_ = nullptr;
        return *this;
    }
    
    int get_degree() const NOEXCEPT {
        return degree_;
    }
    
    poly_t& operator+=(const poly_t& rhs) {
        transform_poly(rhs, [](real_t left, real_t right) {
                       return left + right;
        });
        return *this;
    }

    poly_t& operator-=(const poly_t& rhs) {
        transform_poly(rhs, [](real_t left, real_t right) {
                   return left - right;
        });
        return *this;
    }

    poly_t& operator*=(real_t k) {
        assert(degree_ >= 0);

        for (int i = 0; i <= degree_; ++i)
            coefficients_[i] *= k;

        return *this;
    }

    poly_t& operator/=(real_t k) {
        assert(degree_ >= 0);

        typename internals::div_wrap_t<real_t>::div_helper_t div_val(k);

        for (int i = 0; i <= degree_; ++i)
            coefficients_[i] = div_val(coefficients_[i]);

        return *this;
    }

    /*!
     * \brief   Assigns an array of coefficients. The old coefficients are
     *          destroyed.
     * \param coefficients  Pointer to an array of values.
     * \param size  Array size.
     * \remarks The order of coefficients in the array is assumed to be
     *          a0, a1, a2, ... an
     */
    poly_t& assign(const real_t* coefficients, size_t size) {
        assert(size > 0);

        int new_degree = static_cast<int>(size - 1);
        if (new_degree > degree_) {
            resize(new_degree + 1, false);
        }
        degree_ = new_degree;
        memcpy(coefficients_, coefficients, sizeof(real_t) * size);
        return *this;
    }

    /*!
     * \brief   Read only access to the coefficients.
     */
    real_t operator[](int idx) const NOEXCEPT {
        assert(allocated_);
        assert(idx >= 0 && idx <= degree_);

        return coefficients_[idx];
    }

    /*!
     * \brief   Read write access to the coefficients.
     */
    real_t& operator[](int idx) NOEXCEPT {
        assert(allocated_);
        assert(idx >= 0 && idx <= degree_);

        return coefficients_[idx];   
    }

    /*!
     * \brief   Evalues the polynomial for a specific value.
     */
    real_t evaluate(real_t x) const NOEXCEPT {
        assert(allocated_);

        real_t p_of_x = coefficients_[degree_];
        for (int i = degree_; i > 0; --i) {
            p_of_x = x * p_of_x + coefficients_[i - 1];
        }

        return p_of_x;
    }

    /*!
     * \brief   Evalues the derivative of P(x).
     */
    real_t evaluate_derivative(real_t x) const NOEXCEPT {
        assert(allocated_);

        real_t p_of_dx = coefficients_[degree_] * degree_;
        for (int i = degree_; i > 1; --i) {
            p_of_dx = x * p_of_dx + coefficients_[i - 1] * (i - 1);
        }

        return p_of_dx;
    }

    /*!
     * \brief   Evaluates the integral of the polynomial.
     */
    real_t evaluate_integral(
        real_t x, real_t constant = real_t(0)
        ) const NOEXCEPT {
        assert(allocated_);

        real_t int_px_dx = coefficients_[degree_] / (degree_ + 1);
        for (int i = degree_; i > 0; --i) {
            int_px_dx = x * int_px_dx + coefficients_[i - 1] / i;
        }

        return int_px_dx * x + constant;
    }

    /*!
     * \brief   Computes the roots of a linear equation ax + b = 0;
     * \brief   Number of roots computed (always 1).
     */
    static int get_roots(real_t a, real_t b, real_t* roots) NOEXCEPT {
        assert(!is_zero(a));
        roots[0] = -b / a;
        return 1;
    }

    /*!
     * \brief   Computes the roots of a quadratic equation ax^2 + bx + c = 0;
     * \return  Number of roots computed.
     */
    static int get_roots(real_t a, real_t b, real_t c, real_t* roots) NOEXCEPT;

    /*!
     * \brief   Computes the roots of a cubic equation 
     *          ax^3 + bx^2 + cx + d = 0;
     * \return  Number of roots computed.
     */
    static int get_roots(
            real_t s, real_t p, real_t q, real_t r, real_t* roots
            ) NOEXCEPT;

    /*!
     * \brief   Computes the roots of a quartic equation
     *          ax^4 + bx^3 + cx^2 + dx + e = 0
     * \return  Number of roots computed.
     */
    static int get_roots(
            real_t t, real_t s, real_t p, real_t q, real_t r, real_t* roots
            ) NOEXCEPT;
};

/*!
 * \brief   Nth degree polynomial, simple precision floating point coefficients.
 */
typedef polynomial<float>       polynomialF;

/*!
 * \brief   Nth degree polynomial, double precision floating point coefficients.
 */
typedef polynomial<double>      polynomialD;

/** @} */

} // namespace math
} // namespace v8

#include "v8/math/numerical_analysis/polynomial.inl"
