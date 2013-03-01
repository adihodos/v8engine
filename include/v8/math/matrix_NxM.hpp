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

#if defined(V8_COMPILER_IS_GCC) || defined(V8_COMPILER_IS_CLANG) \
    || defined(V8_COMPILER_IS_MINGW)
#include <initializer_list>
#endif

#include <v8/math/math_utils.hpp>
#include <v8/math/vector_N.hpp>

namespace v8 { namespace math {

/** \addtogroup Algebra
 *  @{
 */

/*!
 * \brief   A generic, N by M matrix. It is stored using row major indexing.
 *          <b> When constructing an object of this class using the default
 *          constructor, make sure to call the resize() member function before
 *          using it. Element access using the m(i,j) syntax uses
 *          1 based indexing</b>.
 */
template<typename real_t>
class matrix_NxM {
private :
    size_t  m_rows_;
    size_t  m_columns_;
    real_t* m_data_;

    size_t data_bytesize() const NOEXCEPT {
        return m_rows_ * m_columns_ * sizeof(real_t);
    }

    size_t compute_index(size_t row, size_t col) const NOEXCEPT {
        assert(row > 0);
        assert(col > 0);
        return (row - 1) * m_columns_ + col - 1;
    }

    size_t dimension() const NOEXCEPT {
        return m_rows_ * m_columns_;
    }

public :
    typedef matrix_NxM<real_t>   matrix_NxM_t;

    /*!
     * \brief   Default constructor. Does not initialize object. Before using
     *          the object, you must call resize().
     */
    matrix_NxM() : m_rows_(0), m_columns_(0), m_data_(nullptr) {}

    /*!
     * \brief Construct an N by M matrix.
     * \param rows  Number of rows.
     * \param columns   Number of columns.
     */
    matrix_NxM(size_t rows, size_t columns)
        : m_rows_(rows), m_columns_(columns), m_data_(new real_t[rows * columns]) {}

    /*!
     * \brief Construct and initialize an N by M matrix.
     * \param rows  Number of rows.
     * \param columns   Number of columns.
     * \param init_data Pointer to an array of at least N by M values used to
     *                  initialize the matrix.
     */
    matrix_NxM(size_t rows, size_t columns, const real_t* init_data)
        : m_rows_(rows), m_columns_(columns),
          m_data_(new real_t[rows * columns]) {
        memcpy(m_data_, init_data, data_bytesize());
    }

    /*!
     * \brief Copy constructor.
     */
    matrix_NxM(const matrix_NxM_t& rhs)
        : m_rows_(rhs.m_rows_), m_columns_(rhs.m_columns_) {
        m_data_ = new real_t[dimension()];
        memcpy(m_data_, rhs.m_data_, data_bytesize());
    }

    /*!
     * \brief Constructs from a temporary.
     */
    matrix_NxM(matrix_NxM_t&& rhs_tmp)
        : m_rows_(rhs_tmp.m_rows_), m_columns_(rhs_tmp.m_columns_),
          m_data_(rhs_tmp.m_data_) {
        rhs_tmp.m_data_ = nullptr;
    }

    ~matrix_NxM() {
        delete[] m_data_;
    }

    /*!
     * \brief Self assignment operator. Assign is allowed only between matrices
     *        that have equal number of rows and columns.
     */
    matrix_NxM_t& operator=(const matrix_NxM_t& rhs) {
        assert(m_rows_ == rhs.m_rows_);
        assert(m_columns_ == rhs.m_columns_);

        memcpy(m_data_, rhs.m_data_, data_bytesize());
        return *this;
    }

    /*!
     * \brief Assignment from a temporary. Assignment is allowed only between
     *        matrices that have the same number of rows and columns.
     */
    matrix_NxM_t& operator=(matrix_NxM_t&& rhs_tmp) {
        assert(m_rows_ == rhs.m_rows_);
        assert(m_columns_ == rhs.m_columns_);

        real_t* data_ptr = rhs_tmp.m_data_;
        rhs_tmp.m_data_ = m_data_;
        m_data_ = data_ptr;
        m_rows_ = rhs_tmp.m_rows_;
        m_columns_ = rhs_tmp.m_columns_;
        return *this;
    }

    /*!
     * \brief Self assign add. Only valid between matrices with the same number
     *        of rows and columns.
     */
    matrix_NxM_t& operator+=(const matrix_NxM_t& rhs) {
        assert(m_rows_ == rhs.m_rows_);
        assert(m_columns_ == rhs.m_columns_);

        const size_t dim = dimension();
        for (size_t i = 0; i < dim; ++i)
            m_data_[i] += rhs.m_data_[i];
        return *this;
    }

    /*!
     * \brief Self assign substract. Only valid between matrices with the same
     *        number of rows and columns.
     */
    matrix_NxM_t& operator-=(const matrix_NxM_t& rhs) {
        assert(m_rows_ == rhs.m_rows_);
        assert(m_columns_ == rhs.m_columns_);

        const size_t dim = dimension();
        for (size_t i = 0; i < dim; ++i)
            m_data_[i] -= rhs.m_data_[i];
        return *this;
    }

    /*!
     * \brief Self assign scalar multiply.
     */
    matrix_NxM_t& operator*=(real_t k) {
        const size_t mtx_dim = dimension();
        for (size_t i = 0; i < mtx_dim; ++i)
            m_data_[i] *= k;
        return *this;
    }

    /*!
     * \brief Self assign scalar division.
     */
    matrix_NxM_t& operator/=(real_t k) {
        const size_t dim = dimension();
        typename internals::div_wrap_t<real_t>::div_helper_t div_val(k);
        for (size_t i = 0; i < dim; ++i)
            m_data_[i] = div_val(m_data_[i]);
        return *this;
    }

    /*!
     * \brief Returns the number of rows.
     */
    size_t get_row_count() const NOEXCEPT {
        return m_rows_;
    }

    /*!
     * \brief Returns the number of columns.
     */
    size_t get_column_count() const NOEXCEPT {
        return m_columns_;
    }

    /*!
     * \brief   Member access (read-only) using m(i,j) like syntax.
     * \param row Index of row, using 1 based indices.
     * \param column Index of column, using 1 based indices.
     */
    real_t operator()(size_t row, size_t column) const NOEXCEPT {
        assert(row > 0 && row <= m_rows_);
        assert(column > 0 && column <= m_columns_);
        return m_data_[compute_index(row, column)];
    }

    /*!
     * \brief   Member access (read-write) using m(i,j) like syntax.
     * \param row Index of row, using 1 based indices.
     * \param column Index of column, using 1 based indices.
     */
    real_t& operator()(size_t row, size_t column) NOEXCEPT {
        assert(row > 0 && row <= m_rows_);
        assert(column > 0 && column <= m_columns_);
        return m_data_[compute_index(row, column)];
    }

    /*!
     * \brief Set contents of a row.
     * \param row 1-based index of the row.
     * \param data Pointer to data. Must have at least row_count() elements.
     */
    void set_row(size_t row, const real_t* data) {
        assert(row > 0 && row <= m_rows_);
        memcpy(m_data_ + (row - 1) * m_columns_, data,
               m_columns_ * sizeof(real_t));
    }

    /*!
     * \brief Set contents of a row using a vector.
     * \param row 1-based index of the row.
     * \param vec Reference to a vector. The vector's dimension must be equal to
     *            the number of elements in a row.
     */
    void set_row(size_t row, const vector_N<real_t>& vec) {
        assert(vec.get_dimension() == m_columns_);
        set_row(row, vec.get_data());
    }

    /*!
     * \brief Set contents of a column.
     * \param row 1-based index of the column.
     * \param data Pointer to data. Must have at least column_count() elements.
     */
    void set_column(size_t column, const real_t* data) {
        assert(column > 0 && column <= m_columns_);

        const size_t col = column - 1;
        for (size_t i = 0; i < m_rows_; ++i)
            m_data_[i * m_columns_ + col] = data[i];
    }

    /*!
     * \brief Set contents of a column using a vector.
     * \param row 1-based index of the column.
     * \param vec Reference to a vector. The vector's dimension must be equal to
     *            the number of elements in a column.
     */
    void set_column(size_t column, const vector_N<real_t>& vec) {
        assert(vec.get_dimension() == m_rows_);
        set_column(column, vec.get_data());
    }

    /*!
     * \brief Copies the contents of a row into an array.
     * \param row 1-based index of the row.
     * \param[in,out] data Pointer to an array. The number of elements in the
     *                     array must be at least equal to the number of
     *                     elements of a row.
     */
    void get_row(size_t row, real_t* data) const NOEXCEPT {
        assert(row > 0 && row <= m_rows_);
        memcpy(data, m_data_ + (row - 1) * m_columns_,
               m_columns_ * sizeof(real_t));
    }

    /*!
     * \brief Copies the contents of a row into a vector.
     * \param row 1-based index of the row.
     * \param[in,out] data Pointer to avector. The number of elements in the
     *                     vector must be equal to the number of
     *                     elements of a row.
     */
    void get_row(size_t row, vector_N<real_t>* vec) const NOEXCEPT {
        assert(row > 0 && row <= m_rows_);
        assert(vec->get_dimension() == m_columns_);
        const real_t* data_ptr = m_data_ + (row - 1) * m_columns_;
        vec->assign(data_ptr);
    }

    /*!
     * \brief Copies the contents of a column into an array.
     * \param row 1-based index of the column.
     * \param[in,out] data Pointer to an array. The number of elements in the
     *                     array must be at least equal to the number of
     *                     elements of a column.
     */
    void get_column(size_t column, real_t* data) const NOEXCEPT {
        assert(column > 0 && column <= m_columns_);

        const size_t col = column - 1;
        for (size_t i = 0; i < m_rows_; ++i)
            data[i] = m_data_[i * m_columns_ + col];
    }

    /*!
     * \brief Copies the contents of a row into a column.
     * \param row 1-based index of the column.
     * \param[in,out] data Pointer to avector. The number of elements in the
     *                     vector must be equal to the number of
     *                     elements of a column.
     */
    void get_column(size_t column, vector_N<real_t>* vec) const NOEXCEPT {
        assert(column > 0 && column <= m_columns_);
        assert(vec->get_dimension() == m_rows_);

        const size_t col = column - 1;
        real_t* out_data = vec->get_data();
        for (size_t i = 0; i < m_rows_; ++i)
            out_data[i] = m_data_[i * m_columns_ + col];
    }

    /*!
     * \brief Computes the transpose of the matrix.
     */
    matrix_NxM_t get_transpose() const {
        matrix_NxM_t transp_mtx(m_columns_, m_rows_);
        for (size_t i = 0; i < m_rows_; ++i) {
            for (size_t j = 0; j < m_columns_; ++j) {
                transp_mtx(j + 1, i + 1) = m_data_(i + 1, j + 1);
            }
        }
        return transp_mtx;
    }

    /*!
     * \brief Returns the number of elements in the matrix (rows * columns).
     */
    size_t get_data_size() const NOEXCEPT {
        return dimension();
    }

    /*!
     * \brief Returns a pointer to the elements.
     */
    real_t* get_data() NOEXCEPT {
        return m_data_;
    }

    /*!
     * \brief Returns a pointer to the elements.
     */
    const real_t* get_data() const NOEXCEPT {
        return m_data_;
    }

    /*!
     * \brief   Sets every element equal to zero.
     */
    matrix_NxM_t& make_zero() NOEXCEPT {
        memset(m_data_, 0, sizeof(real_t) * dimension());
        return *this;
    }

    /*!
     * \brief   Sets a square matrix to the identity.
     */
    matrix_NxM_t& make_identity() NOEXCEPT {
        assert(m_rows_ == m_columns_);

        make_zero();
        for (size_t i = 0; i < m_rows_; ++i) {
            m_data_[i * m_columns_ + i] = real_t(1);
        }
        return *this;
    }

    /*!
     * \brief   Given two vectors w and u, make a matrix representing the tensor
     *          product of w and u. The tensor product of two N dimensional
     *          vectors w and u results in an N by N matrix in the form :
     *          [   w0*u0   w0*u1   ... w0*un   ]
     *          [   w1*u0   w1*u1   ... w1*un   ]
     *          [   ...     ...     ... ...     ]
     *          [   wn*u0   wn*u    ... wn*un   ]
     */
    matrix_NxM_t& make_tensor_product(const vector_N<real_t>& w,
                                      const vector_N<real_t>& u);

    /*!
     * \brief Resizes the matrix.
     * \param rows  Number of rows.
     * \param columns   Number of columns.
     */
    matrix_NxM_t& resize(size_t rows, size_t columns);
};

template<typename real_t>
matrix_NxM<real_t>& matrix_NxM<real_t>::make_tensor_product(
        const vector_N<real_t>& w, const vector_N<real_t>& u)  {
    assert(w.get_dimension() == u.get_dimension());

    const size_t new_dim = w.get_dimension();
    if (dimension() < new_dim * new_dim) {
        delete[] m_data_;
        m_data_ = new real_t[new_dim * new_dim];
    }
    m_rows_ = m_columns_ = new_dim;

    const real_t* w_ptr = w.get_data();
    const real_t* u_ptr = u.get_data();
    for (size_t i = 0; i < m_rows_; ++i) {
        for (size_t j = 0; j < m_columns_; ++i) {
            m_data_[i * m_columns_ + j] = w_ptr[i] * u_ptr[j];
        }
    }
    return *this;
}

template<typename real_t>
matrix_NxM_t& matrix_NxM<real_t>::resize(size_t rows, size_t columns) {
    const size_t old_dimension = dimension();
    const size_t new_dimension = rows * columns;
    if (old_dimension < new_dimension) {
        //
        // grow
        real_t* new_data = new real_t[new_dimension];
        if (m_data_)
            memcpy(new_data, m_data_, data_bytesize());
        delete[] m_data_;
        m_data_ = new_data;
    }

    m_rows_ = rows;
    m_columns_ = columns;
    return *this;
}

template<typename real_t>
bool operator==(
    const matrix_NxM<real_t>& lhs,
    const matrix_NxM<real_t>& rhs) {
    assert(lhs.get_row_count() == rhs.get_row_count());
    assert(lhs.get_column_count() == rhs.get_column_count());

    for (size_t i = 0; i < lhs.get_data_size(); ++i) {
        if (!operands_eq(lhs.get_data()[i], rhs.get_data()[i]))
            return false;
    }
    return true;
}

template<typename real_t>
inline bool operator!=(const matrix_NxM<real_t>& lhs,
                       const matrix_NxM<real_t>& rhs) {
    return !(lhs == rhs);
}

template<typename real_t>
matrix_NxM<real_t> operator-(const matrix_NxM<real_t>& mtx) {
    matrix_NxM<real_t> negated(mtx.get_row_count(), mtx.get_column_count());
    for (size_t i = 0; i < mtx.get_data_size(); ++i)
        negated.get_data()[i] = -(mtx.get_data()[i]);

    return negated;
}

template<typename real_t>
inline matrix_NxM<real_t> operator+(const matrix_NxM<real_t>& lhs,
                                    const matrix_NxM<real_t>& rhs) {
    assert(lhs.get_row_count() == rhs.get_row_count());
    assert(lhs.get_column_count() == rhs.get_column_count());

    matrix_NxM<real_t> result(lhs);
    result += rhs;
    return result;
}

template<typename real_t>
inline matrix_NxM<real_t> operator-(const matrix_NxM<real_t>& lhs,
                                    const matrix_NxM<real_t>& rhs) {
    assert(lhs.get_row_count() == rhs.get_row_count());
    assert(lhs.get_column_count() == rhs.get_column_count());

    matrix_NxM<real_t> result(lhs);
    result -= rhs;
    return result;
}

template<typename real_t>
inline matrix_NxM<real_t> operator*(const matrix_NxM<real_t>& lhs,
                                    real_t k) {
    matrix_NxM<real_t> result(lhs);
    result *= k;
    return result;
}

template<typename real_t>
inline matrix_NxM<real_t> operator*(real_t k,
                                    const matrix_NxM<real_t>& rhs,
                                    ) {
    return rhs * k;
}

template<typename real_t>
inline matrix_NxM<real_t> operator/(const matrix_NxM<real_t>& lhs,
                                    real_t k) {
    matrix_NxM<real_t> result(lhs);
    result /= k;
    return result;
}

template<typename real_t>
inline matrix_NxM<real_t> operator*(const matrix_NxM<real_t>& lhs,
                                    const matrix_NxM<real_t>& rhs) {
    assert(lhs.get_column_count() == rhs.get_row_count());

    matrix_NxM<real_t> result(lhs.get_row_count(), rhs.get_column_count());
    real_t* mat_C = result.get_data();

    for (size_t i = 0; i < lhs.get_row_count(); ++i) {
        for (size_t j = 0; j < rhs.get_column_count(); ++j) {
            const size_t idx = i * result.get_column_count() + j;
            mat_C[idx] = real_t(0);
            for (size_t k = 0; k < lhs.get_row_count(); ++k) {
                mat_C[idx] += lhs(i + 1, k + 1) * rhs(k + 1, j + 1);
            }
        }
    }

    return result;
}

template<typename real_t>
inline vector_N<real_t> operator*(const matrix_NxM<real_t>& mtx,
                                  const vector_N<real_t>& vec) {
    assert(mtx.get_column_count() == vec.get_dimension());

    vector_N<real_t> vec_out(vec.get_dimension());
    const real_t* in_data = vec.get_data();
    real_t* out_data = vec_out.get_data();

    for (size_t i = 0; i < mtx.get_row_count(); ++i) {
        vec_out[i] = real_t(0);
        for (size_t j = 0; j < mtx.get_column_count(); ++j) {
            out_data[i] += mtx(i + 1, j + 1) * in_data[j];
        }
    }

    return vec_out;
}

/** @} */

} // namespace math
} // namespace v8
