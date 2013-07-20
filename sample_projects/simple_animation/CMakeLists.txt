set(
    PROJECT_SOURCES
    animated_aircraft.cc
    main.cc
    main_window.cc
)

add_executable(simple_animation WIN32 ${PROJECT_SOURCES})

target_link_libraries(simple_animation
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