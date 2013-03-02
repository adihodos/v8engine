add_executable(
    julia_fractal
    fractal.cc
    fractal_window.cc
    main.cc
)

target_link_libraries(julia_fractal v8_app_state)