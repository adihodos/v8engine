project(v8engine)
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include(BundleUtilities)

set(CMAKE_DEBUG_POSTFIX "_d")

# Add path for custom modules
set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules"
  )

if( CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR AND NOT MSVC_IDE )
  message(FATAL_ERROR "In-source builds are not allowed.")
endif()

if (MSVC)
    find_package(DirectX)

    if (NOT DirectX_D3D11_FOUND)
        MESSAGE(FATAL_ERROR "Could not find dependency: DirectX")
    endif()

    include_directories(
        ${DirectX_INCLUDE_DIRS}
    )

    link_directories(
        ${DirectX_LIBRARY_DIR}
    )

    set(cxx_disabled_warnings "/wd 4201")
    add_definitions(/DNOMINMAX)

    if (uppercase_CMAKE_BUILD_TYPE STREQUAL "DEBUG")    
        set(cxx_aditional_flags "/Od /Zi /RTC1")
        add_definitions(/DDEBUG /D_DEBUG)
    endif()
    
    foreach(flags_var
            CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINF)
        string(REPLACE "/MD" "/MT" "${flags_var}" "${${flags_var}}")
    endforeach()
    string(REPLACE "/W3" "/W4" "${flags_var}" "${${flags_var}}")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${cxx_aditional_flags} ${cxx_disabled_warnings}")
endif()


include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_SOURCE_DIR}/include/third_party/stlsoft")
add_subdirectory(libs)
add_subdirectory(sample_projects)
