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

#include <cstdint>
#include <vector>

#include <v8/math/vector2.hpp>
#include <v8/math/vector3.hpp>

namespace v8 { namespace math { namespace geometry_gen {

struct vertex_pntt {
    vector3F    vt_position;
    vector3F    vt_normal;
    vector3F    vt_tangent;
    vector2F    vt_texcoord;

    vertex_pntt() {}

    vertex_pntt(const vector3F& position, 
                const vector3F& normal, 
                const vector3F& tangent,
                const vector2F& texcoord)
        : vt_position(position), vt_normal(normal), vt_tangent(tangent),
          vt_texcoord(texcoord) {}

    vertex_pntt(float px, float py, float pz,
               float nx, float ny, float nz,
               float tx, float ty, float tz,
               float tu, float tv) 
        : vt_position(px, py, pz), vt_normal(nx, ny, nz),
          vt_tangent(tx, ty, tz), vt_texcoord(tu, tv) {}
};

struct mesh_data_t {
    std::vector<vertex_pntt> md_vertices;
    std::vector<uint32_t>   md_indices;
};

void create_box(
    float width, 
    float height, 
    float depth, 
    mesh_data_t* mesh_data
    );

void create_sphere(
    float radius,
    size_t slice_count,
    size_t stack_count,
    mesh_data_t* mesh_data
    );

void create_geosphere(
    float radius,
    size_t subdivisions,
    mesh_data_t* mesh_data
    );

void create_cylinder(
    float bottom_radius,
    float top_radius,
    float height,
    size_t slice_count,
    size_t stack_count,
    mesh_data_t* mesh_data
    );

void create_grid(
    const float grid_width,
    const float grid_depth,
    const v8_int_t row_count,
    const v8_int_t column_count,
    mesh_data_t* mesh
    );

void create_fullscreen_quad(
    mesh_data_t* mesh_data
    );

} // namespace geometry_gen
} // namespace math
} // namespace v8
