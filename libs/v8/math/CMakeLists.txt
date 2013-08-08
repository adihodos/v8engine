add_library(
    v8_math STATIC
    camera.cc
    color.cc
    color_palette_generator.cc
    culler.cc
    geometry_generators.cc
    light.cc
    pch_hdr.cc
    random/mtrand.cpp
)

install(TARGETS v8_math DESTINATION libs)