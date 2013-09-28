#pragma once

#include <v8/v8.hpp>

#if defined(V8FRAMEWORK_GRAPHICS_API_IS_DIRECTX)
#include <v8/rendering/directx/buffer_swapper.hpp>
#elif defined(V8FRAMEWORK_GRAPHICS_API_IS_OPENGL)
#error Not implemented yet!
#else
#error Unsuppoorted graphics API
#endif

namespace v8 { namespace rendering {

#if defined(V8FRAMEWORK_GRAPHICS_API_IS_DIRECTX)

    using directx::buffer_swapper;
    using directx::bufferSwapStatus_t;
    using directx::bufferSwapOptions_t;

#else
#error Unsuppoorted graphics API
#endif 

} // namespace rendering
} // namespace v8
