add_executable(
    julia_fractal WIN32
    fractal.cc
    fractal_window.cc
    main.cc
)

target_link_libraries(
    julia_fractal 
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

#message("Output dir = ${RUNTIME_OUTPUT_DIRECTORY}/${CONFIGURATION}")

#add_custom_command(
#   TARGET julia_fractal POST_BUILD
#  COMMAND copy "${Assimp_DLL}" "${RUNTIME_OUTPUT_DIRECTORY}/${CONFIGURATION}"
    #COMMENT "copying drlls ..."
#)    