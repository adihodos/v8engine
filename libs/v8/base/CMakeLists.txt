set(SOURCES 
    pch_hdr.cc 
    ref_link_base.cc)

set(OS_DEPENDENT_LIBS)

if (WIN32)
    list(APPEND SOURCES debug_helpers_win.cc win32_utils.cc)    
else()
    list(APPEND SOURCES debug_helpers_posix.cc)
    list(APPEND OS_DEPENDENT_LIBS rt)
endif()

#message("source list for v8 base = ${SOURCES}")

add_library(
    v8_base STATIC
    ${SOURCES}
    )

target_link_libraries(v8_base ${OS_DEPENDENT_LIBS})

install(TARGETS v8_base DESTINATION libs)