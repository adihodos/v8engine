set(SUBDIRS base math utility)

if (WIN32)
    list(APPEND SUBDIRS input io)

    if (MSVC)
        list(APPEND SUBDIRS gui rendering scene)
    endif(MSVC)

endif(WIN32)

foreach(sub_directory ${SUBDIRS})
    add_subdirectory(${sub_directory})
endforeach()