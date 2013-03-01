#pragma once

#include <v8/v8.hpp>

#include <framework/rendering/vertex_buffer.hpp>
#include <framework/rendering/index_buffer.hpp>
#include <framework/rendering/topology.hpp>

namespace v8_framework { namespace directx {

class mesh_data_t {
public :

    mesh_data_t() 
        : name(nullptr) 
    {}

    ~mesh_data_t() {}

public :
    render_engine::vertex_buffer                    vertices;
    render_engine::index_buffer                     indices;
    render_engine::PrimitiveTopology::Type          topology;
    const char*                                     name;

private :
    NO_CC_ASSIGN(mesh_data_t);
};

} // namespace render_engine
} // namespace v8_framework
