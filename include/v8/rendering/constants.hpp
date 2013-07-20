#pragma once

#include <v8/v8.hpp>

#if defined(V8FRAMEWORK_GRAPHICS_API_IS_DIRECTX)
#include <v8/rendering/directx/constants.hpp>
#elif defined(V8FRAMEWORK_GRAPHICS_API_IS_OPENGL)

#else
#error Unsuppoorted graphics API
#endif

namespace v8 { namespace rendering {

#if defined(V8FRAMEWORK_GRAPHICS_API_IS_DIRECTX)

    using directx::PrimitiveTopology;
    using directx::Filter;
    using directx::Texture_Address_Mode;
    using directx::DepthWriteMask;
    using directx::CompareFunc;
    using directx::StencilOp;
    using directx::Blend_Factor;
    using directx::Blend_Op;
    using directx::Color_Write_Flag;
    using directx::FillMode;
    using directx::CullMode;
    using directx::Shader_Model;
    using directx::FX_Model;
    using directx::ResourceMapFlags;
    using directx::ResourceUsage;
    using directx::CPUAccess;
    using directx::BindingFlag;
    using directx::FramePresent;
    using directx::FramePresentResult;
    using directx::Compile_Options;
    using directx::ElementType;

#else

#endif 

} // namespace rendering
} // namespace v8
