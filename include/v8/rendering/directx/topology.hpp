#pragma once

namespace v8 { namespace directx {

struct PrimitiveTopology { 
    enum Type {
        Undefined,
        PointList,
        LineList,
        LineStrip,
        TriangleList,
        TriangleStrip,
        LineList_Adjacency,
        LineStrip_Adjacency,
        TriangleList_Adjacency,
        TriangleStrip_Adjacency
    };
};

} // namespace directx
} // namespace v8
