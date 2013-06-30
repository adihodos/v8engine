add_library(
    v8_math STATIC
    camera.cc
    color.cc
    culler.cc
    geometry_generators.cc
    light.cc
    pch_hdr.cc
    )

install(TARGETS v8_math ARCHIVE DESTINATION libs)