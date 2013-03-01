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

/**
 * \file convex_polygon2.hpp
 */

#include <vector>
#include <utility>
#include <v8/v8.hpp>
#include <v8/math/math_utils.hpp>
#include <v8/math/vector2.hpp>

namespace v8 { namespace math {

/** \defgroup geometry2D Two dimensional geometry classes and utilities.
 * @{
 */

/**
 * \class   polygon2
 * \brief   A polygon in E2. The vertices are assumed to be counter-clockwise.
 *  ordered.
 */
template<typename real_t>
class polygon2 {
public :
    typedef vector2<real_t>                         vertex_t; //! Vertex type
    typedef polygon2<real_t>                        self_t; //! This class's type.

    //typedef std::vector<vertex_t>::iterator         iterator;
    //typedef std::vector<vertex_t>::const_iterator   const_iterator;

private :
    std::vector<vertex_t> vertices_;

    bool is_valid() const {
        return vertices_.size() >= 3U;
    }

#if defined(V8_COMPILER_HAS_CXX11_MOVE_SEMANTICS)

    void steal_from_rvalue(self_t&& rhs) {
        vertices_ = std::move(rhs.vertices_);
    }

#endif /* V8_COMPILER_HAS_CXX11_MOVE_SEMANTICS */

public :
    /**
     * \name Constructors
     * @{
     */

    /**
     * \brief Constructs a polygon from a pointer to an array of vertices.
     */
    polygon2(const vertex_t* vertices, v8_size_t num_vertices)
        : vertices_(vertices, vertices + num_vertices) {
        assert(is_valid());
    }

    /**
     * \brief Copy constructor.
     */
    polygon2(const self_t& rhs) : vertices_(rhs.vertices_) {
        assert(is_valid());
    }

#if defined(V8_COMPILER_HAS_CXX11_MOVE_SEMANTICS)

    /**
     * \brief Move constructor.
     */
    polygon2(self_t&& rhs) {
        steal_from_rvalue(std::forward(rhs));
    }

#endif /* V8_COMPILER_HAS_CXX11_MOVE_SEMANTICS */    

#if defined(V8_COMPILER_HAS_CXX11_INITIALIZER_LISTS)    

    /**
     * \brief Construct from initializer list.
     */
    polygon2(std::initializer_list<vertex_t> lst) : vertices_(lst) {
        assert(is_valid());
    }

#endif /* V8_COMPILER_HAS_INITIALIZER_LISTS */

    /** @} */

    /**
     * \name Assignment
     * @{
     */

    /**
     * \brief Assignemnt operator.
     */
    self_t& operator=(const self_t& rhs) {
        if (this != & rhs) {
            vertices_ = rhs.vertices_;
        }
        return *this;
    }

#if defined(V8_COMPILER_HAS_CXX11_MOVE_SEMANTICS)    

    /**
     * \brief Move assignment operator.
     */
    self_t& operator=(self_t&& rhs) {
        steal_from_rvalue(std::forward(rhs));
        return *this;
    }

#endif /* V8_COMPILER_HAS_CXX11_MOVE_SEMANTICS */    

    /** @} */

    /**
     * \name Subscripting
     * @{
     */

    /**
     * \brief Returns a reference to the ith vertex.
     */
    vertex_t& operator[](v8_size_t idx) {
        assert(is_valid());
        return vertices_[idx];
    }

    /**
     * \brief   Returns a const reference to the ith vertex.
     */
    const vertex_t& operator[](v8_size_t idx) const {
        assert(is_valid());
        return vertices_[idx];
    }

    /** @} */

    /**
     * \name Properties
     * @{
     */

    /**
     * \brief Returns the number of vertices of the polygon.
     */
    v8_size_t get_vertex_count() const {
        return vertices_.size();
    }

    /**
     * \brief Computes the polygon's perimeter (sum of the length of edges).
     */
    real_t perimeter() const;

    /**
     * \brief Computes the area of the polygon.
     */
    real_t area() const;

    /**
     * \brief Returns the average vertex of the polygon.
     */
    vertex_t vertex_average() const;

    /**
     * \brief Computes the centroid of the polygon.
     * \param[in,out] centroid Pointer to a vertex_t that receives
     *      the coordinates of the centroid, if function succeeds.
     * \param area Optional pointer that gets the value of the polygon's area,
     *      if the function succeeds.
     * \return True if computation succeded, false otherwise (it is possible
     *      for the area to be equal to zero, thus the centroid is undefined)
     * \note The formula to compute the centroid is :
     *  C.x = (1/6) * sum_{i = 0}^{n-1} (x_{i + 1} + x_i) * a_i
     *  C.y = (1/6) * sum_{i = 0}^{n-1} (y_{i + 1} + y_i) * a_i,
     *  where a_i = x_i * y_{i + 1} - x_{i + 1} * y_i.
     *  See Graphic Gems 4, I.1 Centroid of a polygon.
     */
    v8_bool_t centroid(vertex_t* centroid, real_t* area = nullptr) const;
};

template<typename real_t>
real_t polygon2<real_t>::area() const {
    assert(is_valid());

    const v8_size_t num_verts = vertices_.size() - 1;
    real_t sum_area =
        vertices_[num_verts].x_ * (vertices_[0].y_ - vertices_[num_verts - 1].y_)
        + vertices_[0].x_ * (vertices_[1].y_ - vertices_[num_verts].y_);

    for (v8_size_t i = 1; i < num_verts; ++i) {
        sum_area += vertices_[i].x_
                    * (vertices_[i + 1].y_ - vertices_[i - 1].y_);
    }

    typename internals::div_wrap_t<real_t>::div_helper_t div_op(2);
    return div_op(sum_area);
}

template<typename real_t>
real_t polygon2<real_t>::perimeter() const {
    assert(is_valid());

    const v8_size_t num_verts = vertices_.size() - 1;
    real_t perimeter = (vertices_[num_verts] - vertices_[0]).magnitude();

    for (v8_size_t i = 0; i < num_verts; ++i) {
        perimeter += (vertices_[i + 1] - vertices_[i]).magnitude();
    }

    return perimeter;
}

template<typename real_t>
typename polygon2<real_t>::vertex_t
polygon2<real_t>::vertex_average() const {
    assert(is_valid());

    vertex_t vertex_avg(vertices_[0]);
    for (v8_size_t i = 1; i < vertices_.size(); ++i) {
        vertex_avg += vertices_[i];
    }

    return vertex_avg / static_cast<real_t>(vertices_.size());
}

template<typename real_t>
v8_bool_t
polygon2<real_t>::centroid(
    vertex_t *v_centroid,
    real_t* area
    ) const {
    assert(is_valid());

    real_t ai = real_t(0);
    real_t x_tmp = real_t(0);
    real_t y_tmp = real_t(0);
    real_t area_sum = real_t(0);

    const v8_size_t num_verts = vertices_.size();
    v8_size_t i = num_verts - 1;
    v8_size_t j = 0;

    for (; j < num_verts; i = j, ++j) {
        ai = vertices_[i].x_ * vertices_[j].y_
             - vertices_[j].x_ * vertices_[i].y_;
        area_sum += ai;
        x_tmp += (vertices_[j].x_ + vertices_[i].x_) * ai;
        y_tmp += (vertices_[j].y_ + vertices_[i].y_) * ai;
    }

    typename internals::div_wrap_t<real_t>::div_helper_t div_op(real_t(2));

    if (area) {
        *area = div_op(area_sum);
    }

    if (!is_zero(area_sum)) {
        div_op.set_divisor(real_t(3) * area_sum);
        v_centroid->x_ = div_op(x_tmp);
        v_centroid->y_ = div_op(y_tmp);
        return true;
    }

    return false;
}

/**
 * \brief polygon2F Polygon in E2, simple precision floating point components.
 */
typedef polygon2<float>         polygon2F;

/**
 * \brief polygon2F Polygon in E2, double precision floating point components.
 */
typedef polygon2<double>        polygon2D;

/** @} */

} // namespace math
} // namespace v8
