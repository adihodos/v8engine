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

namespace v8 { namespace rendering {

struct vertex_pn;
struct vertex_pnt;

}
}

namespace v8 { namespace utility {

///
/// \brief  Imports geometry data from a file.
/// \param  file_name   File with geometric data.
/// \param  flip_around_yaxis   Some models are oriented towards the user (-Z axis). 
///         Passing this flag will reorient the model along the positive Z axis.
/// \param  vertices Receives the address of the vertex data for the model. 
///         Caller must free the data when no longer needed, using delete[].
/// \param  indices Receives the address of the index data for the model.
///         Caller must free the data when no longer needed, using delete[].
/// \param  num_vertices Receives the count of vertices.
/// \param  num_indices Receives the count of indices.
/// \returns True if import succeeded, false otherwise.
v8_bool_t import_geometry(
    const char* file_name,
    const v8_bool_t flip_around_yaxis,
    v8::rendering::vertex_pnt** vertices,
    v8_uint32_t** indices,
    v8_uint32_t* num_vertices,
    v8_uint32_t* num_indices
    );

}
}