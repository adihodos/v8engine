#pragma once

#include <v8/v8.hpp>

#if defined(V8FRAMEWORK_GRAPHICS_API_IS_DIRECTX)
#include <v8/rendering/directx/renderer.hpp>
#elif defined(V8FRAMEWORK_GRAPHICS_API_IS_OPENGL)
#error Not implemented yet!
#else
#error Unsuppoorted graphics API
#endif

namespace v8 { namespace rendering {

#if defined(V8FRAMEWORK_GRAPHICS_API_IS_DIRECTX)

    using directx::renderer;

#else
#error Unsuppoorted graphics API
#endif 

} // namespace rendering
} // namespace v8
