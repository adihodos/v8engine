set(SUBDIRS base math utility)

if (WIN32)
    list(APPEND SUBDIRS gui input io)

    if (MSVC)
        list(APPEND SUBDIRS rendering scene)
    endif(MSVC)

endif(WIN32)

foreach(sub_directory ${SUBDIRS})
    add_subdirectory(${sub_directory})
endforeach()
