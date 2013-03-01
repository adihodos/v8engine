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
template<typename BinaryOp>
void v8::math::polynomial<real_t>::transform_poly(
    const polynomial<real_t>& rhs, BinaryOp bin_op
    ) NOEXCEPT {
    assert(degree_ >= 0);

    if (degree_ < rhs.degree_) {
        //
        // grow to accomodate extra values
        resize((rhs.degree_ + 1), true);

        //
        // copy new values
        for (int i = degree_ + 1; i <= rhs.degree_; ++i) {
            coefficients_[i] = rhs.coefficients_[i];
        }

        //
        // add existing values
        for (int i = 0; i <= degree_; ++i) {
            coefficients_[i] = bin_op(coefficients_[i],
                                      rhs.coefficients_[i]);
        }
        degree_ = rhs.degree_;
    } else if (degree_ > rhs.degree_) {
        for (int i = 0; i <= rhs.degree_; ++i) {
            coefficients_[i] = bin_op(coefficients_[i],
                                      rhs.coefficients_[i]);
        }
    } else {
        for (int i = degree_; i >= 0; --i) {
            coefficients_[i] = bin_op(coefficients_[i],
                                      rhs.coefficients_[i]);
            //
            // check for cancellation and decrement degree
            if ((degree_ == i) && is_zero(coefficients_[i]))
                --degree_;
        }
    }
}

template<typename real_t>
int v8::math::polynomial<real_t>::get_roots(
    real_t a, real_t b, real_t c, real_t* roots
    ) NOEXCEPT {
    if (is_zero(a)) { //
        // solve linear equation
        return get_roots(b, c, roots);
    }

    const real_t delta = b * b - real_t(4) * a * c;
    if (delta > 0.0f) {
        const real_t q = -(b + signum(b) * sqrt(delta)) / real_t(2);
        roots[0] = q / a;
        roots[1] = c / q;
        return 2;
    } else if (is_zero(delta)) {
        roots[0] = -b / real_t(2) * a;
        return 1;
    } else {
        //
        // no real value roots
        return 0;
    }
}

template<typename real_t>
int v8::math::polynomial<real_t>::get_roots(
    real_t s, real_t p, real_t q, real_t r, real_t* roots
    ) NOEXCEPT {
    //
    // See this : http://www.cs.iastate.edu/~cs577/handouts/polyroots.pdf
    // for a more detailed explanation.

    if (is_zero(s)) {
        //
        // solve quadratic equation
        return get_roots(p, q, r, roots);
    }

    using namespace internals;

    //
    // Divide by s to get equation in standard form :
    // x^3 + (p/s)x^2 + (q/s)x + (r/s) = 0
    if (!operands_eq(real_t(1), s)) {
        typename div_wrap_t<real_t>::div_helper_t div_val(s);
        p = div_val(p);
        q = div_val(q);
        r = div_val(r);
    }

    //
    // Reduce equation to the normal form (y^3 + ay + b) = 0 by
    // substituting x = y - p/3
    // a = 1/3 * (3q - p^2)
    // b = 1/27 * (2p^3 -9pq + 27r)
    const real_t one_third = real_t(1) / real_t(3);
    const real_t one_twentyseven = real_t(1) / real_t(27);
    const real_t offset = one_third * p;
    const real_t a = q - p * offset;
    const real_t b = r + p * (real_t(2) * p * p - real_t(9) * q) * one_twentyseven;
    const real_t half_b = real_t(0.5) * b;

    real_t delta = half_b * half_b + a * a * a * one_twentyseven;
    if (is_zero(delta)) {
        delta = real_t(0);
    }

    if (delta > real_t(0)) {
        //
        // one root is real, the other two complex
        const real_t sqrt_delta = sqrt(delta);
        const real_t tmp = -half_b + sqrt_delta;
        if (tmp >= real_t(0)) {
            roots[0] = pow(tmp, one_third);
        } else {
            roots[0] = -pow(-tmp, one_third);
        }

        const real_t tmp1 = -half_b - sqrt_delta;
        if (tmp1 >= real_t(0)) {
            roots[0] += pow(tmp1, one_third);
        } else {
            roots[0] -= pow(-tmp1, one_third);
        }

        roots[0] -= offset;
        return 1;
    } else if (delta < real_t(0)) {
        //
        // three distinct real roots
        const real_t sqrt3 = sqrt(real_t(3));
        const real_t const_fact = sqrt(-one_third * a);
        const real_t angle = one_third * atan2(sqrt(-delta), -half_b);
        const real_t css = cos(angle);
        const real_t sns = sin(angle);

        roots[0] = real_t(2) * const_fact * css - offset;
        roots[1] = -const_fact * (css + sqrt3 * sns) - offset;
        roots[2] = -const_fact * (css - sqrt3 * sns) - offset;

        return 3;
    } else {
        //
        // three real roots, two of them equal
        real_t tmp;
        if (half_b >= real_t(0)) {
            tmp = -pow(half_b, one_third);
        } else {
            tmp = pow(-half_b, one_third);
        }

        roots[0] = real_t(2) * tmp - offset;
        roots[1] = -tmp - offset;
        roots[2] = roots[1];

        return 3;
    }
}

template<typename real_t>
int v8::math::polynomial<real_t>::get_roots(
    real_t t, real_t s, real_t p, real_t q, real_t r, real_t* roots
        ) NOEXCEPT {
    if (is_zero(t)) {
        //
        // solve cubic equation
        return get_roots(s, p, q, r, roots);
    }

    //
    // make monic polynomial
    if (!operands_eq(real_t(1), t)) {
        using namespace internals;
        typename div_wrap_t<real_t>::div_helper_t div_val(t);
        s = div_val(s);
        p = div_val(p);
        q = div_val(q);
        r = div_val(r);
    }

    const real_t p_pow2 = p * p;
    const real_t one_quarter = real_t(0.25);

    //
    // compute the roots of the resolvent cubic
    real_t resolvent_roots[3];
    int root_cnt = get_roots(real_t(1), -q, (p * r - real_t(4) * s),
                             (real_t(4) * q * s - r * r - p_pow2 * s),
                             resolvent_roots);
    assert(root_cnt > 0);

    const real_t z1 = resolvent_roots[0];
    const real_t delta = one_quarter * p_pow2 - q + z1;
    if (delta < real_t(0)) {
        //
        // no real solutions
        return 0;
    } else if (delta > real_t(0)) {
        const real_t r = sqrt(delta);
        const real_t term_a = real_t(0.75) * p_pow2 
            - r * r - real_t(2) * q;
        const real_t term_b = real_t(0.25) * (real_t(4) * p * q - real_t(8) 
                                              * r - p_pow2 * p) * 1.0f / r;

        const real_t t_diff = term_a - term_b;
        const real_t t_sum = term_a + term_b;

        int roots_num = 0;

        if (t_sum >= real_t(0)) {
            const real_t d = sqrt(t_sum);
            roots[roots_num++] = -real_t(0.25) * p + real_t(0.5) * (r + d);
            roots[roots_num++] = -real_t(0.25) * p + real_t(0.5) * (r - d);
        }

        if (t_diff >= real_t(0)) {
            const real_t e = sqrt(t_diff);
            roots[roots_num++] = -real_t(0.25) * p - real_t(0.5) * (r + e);
            roots[roots_num++] = -real_t(0.25) * p - real_t(0.5) * (r - e);
        }

        return roots_num;
    } else {
        const real_t first_sqr = z1 * z1 - real_t(4) * s;
        if (first_sqr >= real_t(0)) {
            const real_t term_a = real_t(0.75) * p_pow2 - real_t(2) * q;
            const real_t term_b = real_t(2) * sqrt(first_sqr);

            const real_t t_sum = term_a + term_b;
            const real_t t_diff = term_a - term_b;

            int roots_num = 0;
            if (t_sum >= real_t(0)) {
                const real_t d = sqrt(t_sum);
                roots[roots_num++] = -real_t(0.25) * p 
                    + real_t(0.5) * (r + d);
                roots[roots_num++] = -real_t(0.25) * p 
                    + real_t(0.5) * (r - d);
            }

            if (t_diff >= real_t(0)) {
                const real_t e = sqrt(t_diff);
                roots[roots_num++] = -real_t(0.25) * p 
                    - real_t(0.5) * (r + e);
                roots[roots_num++] = -real_t(0.25) * p 
                    - real_t(0.5) * (r - e);
            }
            return roots_num;
        }
        return 0;
    }
}
