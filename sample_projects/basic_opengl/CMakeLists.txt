project(basic_opengl)

add_executable(basic_opengl WIN32
    opengl_renderer.cc
    main.cc
)

target_link_libraries(basic_opengl 
    v8_base
    v8_utility 
    v8_gui 
    v8_input
    v8_math 
    pdh 
    opengl32
)
