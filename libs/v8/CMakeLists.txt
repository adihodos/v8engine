add_subdirectory(base)
#add_subdirectory(gui)
#add_subdirectory(input)
#add_subdirectory(io)
add_subdirectory(math)

if (MSVC)
	add_subdirectory(rendering)
	add_subdirectory(scene)
endif(MSVC)

add_subdirectory(utility)

#add_library(v8_app_state global_state.cc)