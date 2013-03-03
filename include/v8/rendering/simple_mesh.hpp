#pragma once

#include <v8/rendering/vertex_buffer.hpp>
#include <v8/rendering/index_buffer.hpp>
#include <v8/rendering/constants.hpp>

namespace v8 { namespace rendering {

struct simple_mesh {
    simple_mesh()
        :   vertex_buffer_(nullptr)
            , index_buffer_(nullptr)
            , topology_(PrimitiveTopology::Undefined)
    {}

    simple_mesh(
        vertex_buffer* vb, 
        index_buffer* ib, 
        PrimitiveTopology::Type topo
        )
        :   vertex_buffer_(vb)
            , index_buffer_(ib)
            , topology_(topo)
    {}

    vertex_buffer*                     vertex_buffer_;
    index_buffer*                      index_buffer_;
    PrimitiveTopology::Type             topology_;
};

} // namespace rendering
} // namespace v8
