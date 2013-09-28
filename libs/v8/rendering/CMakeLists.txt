add_library(
    v8_renderer_directx STATIC
    directx/internal/dds_texture_loader.cc
    directx/internal/dxerr.cc
    directx/internal/gpu_buffer_core.cc
    directx/internal/include_handler.cc
    directx/internal/shader_common_base.cc
    directx/internal/utilities.cc
    directx/internal/shader_profile_strings.cc
    directx/blend_state.cc
    directx/buffer_swapper.cc
    directx/depthstencil_state.cc
    directx/gpu_shader.cc
    directx/input_layout.cc
#    directx/effect.cc
#    directx/effect_pass.cc
#    directx/effect_technique.cc
    directx/rasterizer_state.cc
    directx/renderer.cc
    directx/sampler_state.cc
    directx/shader_compiler.cc
    directx/texture.cc
    directx/texture_shader_binding.cc
    directx/texture_unordered_access_binding.cc
    material.cc
    render_assets_cache.cc
#    simple_mesh.cc
)

#find_package(DirectX REQUIRED)
#find_package(Assimp REQUIRED)

#if (NOT DirectX_D3D11_FOUND)
#    MESSAGE(FATAL_ERROR "Could not find dependency: DirectX")
#endif()

#include_directories(
    #${DirectX_INCLUDE_DIRS}
#)

#link_directories(
#    ${DirectX_LIBRARY_DIR} ${Assimp_LIBRARY_DIR}
#)

target_link_libraries(
    v8_renderer_directx 
    directx_tex
    ${DirectX_D3D11_LIBRARIES}
    #${Assimp_LIBRARIES}
)

install(TARGETS v8_renderer_directx DESTINATION libs)
