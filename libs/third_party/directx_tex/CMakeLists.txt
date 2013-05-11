add_library(
    directx_tex STATIC
    BC.cpp
    BC4BC5.cpp
    BC6HBC7.cpp
    DirectXTexCompress.cpp
    DirectXTexConvert.cpp
    DirectXTexD3D11.cpp
    DirectXTexDDS.cpp
    DirectXTexFlipRotate.cpp
    DirectXTexImage.cpp
    DirectXTexMipmaps.cpp
    DirectXTexMisc.cpp
    DirectXTexNormalMaps.cpp
    DirectXTexPMAlpha.cpp
    DirectXTexResize.cpp
    DirectXTexTGA.cpp
    DirectXTexUtil.cpp
    DirectXTexWIC.cpp
)

#find_package(DirectX REQUIRED)

include_directories(
    ${DirectX_INCLUDE_DIRS}
)

link_directories(
    ${DirectX_LIBRARY_DIR}
)

target_link_libraries(
    directx_tex
    ${DirectX_D3D11_LIBRARIES}
)