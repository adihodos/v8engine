project(v8engine)
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include(BundleUtilities)

set(CMAKE_DEBUG_POSTFIX "_d")

# Add path for custom modules
set(CMAKE_MODULE_PATH
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules"
  )

message("Module path = ${CMAKE_MODULE_PATH}")

#find_package(Assimp REQUIRED)

if( CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR AND NOT MSVC_IDE )
  message(FATAL_ERROR "In-source builds are not allowed.")
endif()

if (MSVC)
    set(DirectX_D3D11_LIBRARIES "d3d11.lib;d3dcompiler.lib;dxgi.lib;dxguid.lib")

    set(cxx_disabled_warnings "/wd4201 /wd4127 /wd4005")

    # define NOMINMAX so that code that uses STL can compile
    add_definitions(/DNOMINMAX)

    # Disabled retarded warnings about some functions being deprecated
    add_definitions(/D_CRT_SECURE_NO_WARNINGS)
    add_definitions(/D_CRT_NONSTDC_NO_WARNINGS)
    add_definitions(/D_CRT_SECURE_NO_DEPRECATE)

    #
    # Compile for Windows 7 and later
    add_definitions(/D_WIN32_WINNT=0x0601)
    add_definitions(/DWINVER=0x0601)
    add_definitions(/DDXGI_1_2_FORMATS)
    add_definitions(/D_WIN7_PLATFORM_UPDATE)

    if (CMAKE_BUILD_TYPE MATCHES Debug)
        add_definitions(/DV8_IS_DEBUG_BUILD)
        set(cxx_aditional_flags "/Od /Zi /RTC1")
    endif()
    
    foreach(flags_var
            CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINF)
        string(REPLACE "/MD" "/MT" "${flags_var}" "${${flags_var}}")
        string(REPLACE "/W3" "/W4" "${flags_var}" "${${flags_var}}")
    endforeach()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${cxx_aditional_flags} ${cxx_disabled_warnings}")
    message("${CMAKE_CXX_FLAGS}")
    message("FLAGS VAR = ${flags_var}")
endif()

if (MINGW)
	add_definitions(-DNOMINMAX)
    add_definitions(-D_WIN32_WINNT=0x0601)
    add_definitions(-DWINVER=0x0601)

    if (CMAKE_BUILD_TYPE MATCHES Debug)
        add_definitions(-DV8_IS_DEBUG_BUILD)
        set(cxx_aditional_flags "-O0 -ggdb3")
    endif()

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")
endif(MINGW)

include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_SOURCE_DIR}/include/third_party/stlsoft")
add_subdirectory(libs)

if (MSVC)
	add_subdirectory(sample_projects)
endif(MSVC)

install(DIRECTORY include/v8 DESTINATION include)