add_library(v8_scene STATIC
    cam_controller_spherical_coordinates.cc
    camera_controller.cc
    custom_entity.cc
    geometric_entity.cc
    group_node.cc
    null_camera_controller.cc
    scene_entity.cc
    scene_system.cc
)

target_link_libraries(v8_scene
    v8_base 
    v8_rendering 
    v8_math 
    v8_utility 
    v8_io
)