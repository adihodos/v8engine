add_library(fw1_fontwrapper STATIC
    CFW1ColorRGBA.cpp
    CFW1ColorRGBAInterface.cpp
    CFW1DWriteRenderTarget.cpp
    CFW1DWriteRenderTargetInterface.cpp
    CFW1Factory.cpp
    CFW1FactoryInterface.cpp
    CFW1FontWrapper.cpp
    CFW1FontWrapperInterface.cpp
    CFW1GlyphAtlas.cpp
    CFW1GlyphAtlasInterface.cpp
    CFW1GlyphProvider.cpp
    CFW1GlyphProviderInterface.cpp
    CFW1GlyphRenderStates.cpp
    CFW1GlyphRenderStatesInterface.cpp
    CFW1GlyphSheet.cpp
    CFW1GlyphSheetInterface.cpp
    CFW1GlyphVertexDrawer.cpp
    CFW1GlyphVertexDrawerInterface.cpp
    CFW1StateSaver.cpp
    CFW1TextGeometry.cpp
    CFW1TextGeometryInterface.cpp
    CFW1TextRenderer.cpp
    CFW1TextRendererInterface.cpp
    FW1FontWrapper.cpp
    FW1Precompiled.cpp    
)

target_link_libraries(fw1_fontwrapper ${DirectX_D3D11_LIBRARIES})