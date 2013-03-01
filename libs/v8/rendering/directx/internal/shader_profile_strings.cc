#include "v8/rendering/directx/internal/vertex_shader_traits.hpp"
#include "v8/rendering/directx/internal/geometry_shader_traits.hpp"
#include "v8/rendering/directx/internal/fragment_shader_traits.hpp"

const char* const 
v8::directx::dx11_vertexshader_traits::Profile_Strings[] = {
        "vs_3_0",
        "vs_4_0",
        "vs_4_1",
        "vs_5_0"
};

const char* const
v8::directx::dx11_geometryshader_traits::Profile_Strings[] = {
        "NOT SUPPORTED",
        "gs_4_0",
        "gs_4_1",
        "gs_5_0"
};

const char* const
v8::directx::dx11_fragmentshader_traits::Profile_Strings[] = {
        "ps_3_0",
        "ps_4_0",
        "ps_4_1",
        "ps_5_0"
};