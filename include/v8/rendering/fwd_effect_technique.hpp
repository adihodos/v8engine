#pragma once

//!
//! \file fwd_renderer.hpp Forward declarations for the renderer class.

#include <v8/v8.hpp>

#if defined(V8FRAMEWORK_GRAPHICS_API_IS_DIRECTX)

#include <v8/rendering/directx/fwd_effect_technique.hpp>

#elif defined(V8FRAMEWORK_GRAPHICS_API_IS_OPENGL)

#error Not implemented yet!

#else

#error Unsuppoorted graphics API

#endif

namespace v8 { namespace rendering {

#if defined(V8FRAMEWORK_GRAPHICS_API_IS_DIRECTX)

    using directx::effect_technique;

#else

#error Unsuppoorted graphics API

#endif 

} // namespace rendering
} // namespace v8
