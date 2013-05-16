#pragma once

#include <v8/v8.hpp>

namespace v8 { namespace rendering {

    struct vertex_pn;
}
}

namespace v8 { namespace utility {

v8_bool_t import_geometry(
    const char* file_name,
    v8::rendering::vertex_pn** vertices,
    v8_uint32_t** indices,
    v8_uint32_t* num_vertices,
    v8_uint32_t* num_indices
    );

}
}