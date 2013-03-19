include(FindPkgMacros)

findpkg_begin(Assimp)

getenv_path(ASSIMP_SDK_DIR)

set(Assimp_PREFIX_PATH
    "${ASSIMP_SDK_DIR}" "${ENV_ASSIMP_SDK_DIR}"
)

create_search_paths(Assimp)

find_library(Assimp_LIBRARIES NAME assimp.lib HINTS ${Assimp_LIB_SEARCH_PATH})
find_file(Assimp_DLL NAME assimp.dll HINTS ${Assimp_LIB_SEARCH_PATH})

if (Assimp_LIBRARIES)
    set(Assimp_FOUND true)
endif()    

findpkg_finish(Assimp)