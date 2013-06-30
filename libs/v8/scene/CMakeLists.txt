add_library(
    v8_scene STATIC
    cam_controller_spherical_coordinates.cc
    camera_controller.cc
    custom_entity.cc
    group_node.cc
    null_camera_controller.cc
    scene_config_reader.cc
    scene_entity.cc
    scene_system.cc
)

target_link_libraries(
    v8_scene
    v8_base
    v8_math 
    v8_utility 
    v8_io
)

install(TARGETS v8_scene ARCHIVE DESTINATION libs)