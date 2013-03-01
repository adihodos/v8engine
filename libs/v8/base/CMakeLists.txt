if ("${CMAKE_SYSTEM_NAME}" MATCHES "Windows")
add_library(
    v8_base STATIC
    pch_hdr.cc
    debug_helpers_win.cc
    ref_link_base.cc
    win32_utils.cc
    )
else()
add_library(
    v8_base STATIC
    debug_helpers_posix.cc
    pch_hdr.cc
    ref_link_base.cc
    )
target_link_libraries(v8_base rt)
endif()    
