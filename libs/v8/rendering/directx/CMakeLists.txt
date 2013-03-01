add_library(
    v8_renderer_directx

    internal/debug_helpers.cc
    internal/gpu_buffer_core.cc
    internal/include_handler.cc
    internal/shader_common_base.cc
    internal/utilities.cc
    internal/shader_profile_strings.cc
    blend_state.cc
    depthstencil_state.cc
    gpu_shader.cc
    input_layout.cc
    effect.cc
    effect_pass.cc
    effect_technique.cc
    rasterizer_state.cc
    renderer.cc
    sampler_state.cc
    shader_compiler.cc
    texture.cc
)

find_package(DirectX)

if (NOT DirectX_D3D11_FOUND)
    MESSAGE(FATAL_ERROR "Could not find dependency: DirectX")
endif()

include_directories(
    ${DirectX_INCLUDE_DIRS}
)

link_directories(
    ${DirectX_D3D11_LIBRARIES}
)

target_link_libraries(v8_renderer_directx ${DirectX_D3D11_LIBRARIES})