set(V8_LIB_TARGETS "${V8_LIB_TARGETS} v8_math")
add_library(
    v8_math STATIC
    camera.cc
    color.cc
    culler.cc
    geometry_generators.cc
    light.cc
    pch_hdr.cc
    )
