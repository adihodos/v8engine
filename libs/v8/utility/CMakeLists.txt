add_library(
    v8_utility STATIC
    geometry_importer.cc
    hash_spooky.cc
    ifs_loader.cc
    string_ext.cc
    win_util.cc
)

target_link_libraries(v8_utility ${Assimp_LIBRARIES})
install(TARGETS v8_utility ARCHIVE DESTINATION libs)