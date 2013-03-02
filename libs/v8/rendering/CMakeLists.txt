add_library(
    v8_renderer_directx

    directx/internal/debug_helpers.cc
    directx/internal/gpu_buffer_core.cc
    directx/internal/include_handler.cc
    directx/internal/shader_common_base.cc
    directx/internal/utilities.cc
    directx/internal/shader_profile_strings.cc
    directx/blend_state.cc
    directx/depthstencil_state.cc
    directx/gpu_shader.cc
    directx/input_layout.cc
    directx/effect.cc
    directx/effect_pass.cc
    directx/effect_technique.cc
    directx/rasterizer_state.cc
    directx/renderer.cc
    directx/sampler_state.cc
    directx/shader_compiler.cc
    directx/texture.cc
    material.cc
    render_assets_cache.cc
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