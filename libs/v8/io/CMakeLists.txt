add_library(
    v8_io STATIC
    config_file_reader.cc
    filesystem.cc)

install(TARGETS v8_io DESTINATION libs)