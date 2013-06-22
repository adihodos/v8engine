project(basic_drawing)

add_executable(
    basic_drawing WIN32
    aircraft_f4.cc
    DDSTextureLoader.cc
    floor_plane.cc
    main.cc
)

target_link_libraries(
    basic_drawing
    ${DirectX_D3D11_LIBRARIES}
    ${Assimp_LIBRARIES}
    fw1_fontwrapper
    v8_renderer_directx
    v8_base
    v8_math
    v8_scene
    v8_io
    v8_input
    v8_gui
    v8_app_state
    v8_utility
    pdh
)
