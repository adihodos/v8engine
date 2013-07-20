project(test)

add_executable(test WIN32 main.cc)

target_link_libraries(test
    ${DirectX_D3D11_LIBRARIES}
    fw1_fontwrapper
    v8_renderer_directx
    v8_base
    v8_math
    v8_scene
    v8_io
    v8_input
    v8_gui
    v8_utility
    pdh
)