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

#include <v8/v8.hpp>
#include <v8/rendering/fragment_shader.hpp>
#include <v8/rendering/rasterizer_state.hpp>
#include <v8/rendering/index_buffer.hpp>
#include <v8/rendering/vertex_buffer.hpp>
#include <v8/rendering/vertex_shader.hpp>
#include <v8/rendering/texture_shader_binding.hpp>
#include <v8/rendering/sampler_state.hpp>

#include "igeometry_shape.hpp"


class FloorPlane : public IGeometryShape {
public :
    typedef IGeometryShape                  base_class;

    FloorPlane();

public :
    v8_bool_t Initialize(const InitContext* init_context);

    void Update(const float delta);

    void Draw(const DrawingContext* draw_context);

private :
    v8::rendering::vertex_buffer            vertexbuffer_;
    v8::rendering::index_buffer             indexbuffer_;
    v8::rendering::vertex_shader            vertexshader_;
    v8::rendering::fragment_shader          fragshader_;
    v8::rendering::rasterizer_state         rs_wireframe_;
    v8::rendering::texture_shader_binding   mtl_grid_;
    v8::rendering::sampler_state            tex_sampler_;

private :
    NO_CC_ASSIGN(FloorPlane);
};
