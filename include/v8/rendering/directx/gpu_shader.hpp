#pragma once

#include <d3d11.h>
#include <v8/v8.hpp>
#include <v8/base/scoped_pointer.hpp>

#include <v8/rendering/directx/internal/fwd_shader_traits.hpp>
#include <v8/rendering/directx/fwd_renderer.hpp>
#include <v8/base/operator_bool.hpp>

namespace v8 { namespace directx {

class renderer;

struct shader_info_t;

template<typename T>
struct gpu_shader_private_implementation;

template<typename gpu_shader_traits>
class gpu_shader {
/// \name Defined types.
/// @{

public :
    typedef     gpu_shader<gpu_shader_traits>                       class_type;

/// @}

/// \name Construction and initialization.
/// @{

public :
    gpu_shader();

    ~gpu_shader();

    v8_bool_t initialize(const shader_info_t& shdr_info, renderer* render_sys);

/// @}

/// \name Sanity checking
/// @{

public :

    operator int base::operator_bool::*() const;

/// @}

/// \name Shader operations.
/// @{

public :

    enum {
        InvalidUniformLocation = v8_uint32_t(-1)
    };

    ID3D11InputLayout* get_input_signature() const;

    v8_size_t get_uniform_block_count() const;

    v8_size_t get_uniform_var_count() const;

    v8_uint32_t get_uniform_location_by_name(
        const char* uniform_name
        ) const;

    v8_uint32_t get_uniform_block_location_by_name(
        const char* block_name
        ) const;

    template<typename data_type>
    inline void set_uniform_block_by_name(
        const char* block_name, const data_type& uniform_value
        );

    template<typename data_type>
    inline void set_uniform_by_name(
        const char* uniform_name, const data_type& uniform_value
        );

    void set_uniform_by_name(
        const char* uniform_name, 
        const void* uniform_data, 
        v8_size_t byte_count
        );

    void set_uniform_block_by_name(
        const char* block_name, 
        const void* block_data, 
        const v8_size_t block_data_size
        );

    void set_sampler(
        const char* sampler_name, ID3D11SamplerState* sampler_state
        );

    void set_resource_view(
        const char* sampler_name, ID3D11ShaderResourceView* res_view
        );

    void bind_to_pipeline(
        renderer* render_sys
        );

/// @}

/// \name Members.
/// @{

private :
    typedef gpu_shader_private_implementation
    <
        gpu_shader_traits
    >                                                    implementation_details;

    v8::base::scoped_ptr<implementation_details>         pimpl_;

/// @}

/// \name Disabled member functions.
/// @{

private :
    NO_CC_ASSIGN(class_type);

/// @}
};

template<typename T>
template<typename U>
inline void gpu_shader<T>::set_uniform_block_by_name(
    const char* block_name, const U& uniform_value
    ) {
    set_uniform_block_by_name(block_name, &uniform_value, sizeof(uniform_value));
}

template<typename T>
template<typename U>
inline void gpu_shader<T>::set_uniform_by_name(
    const char* uniform_name, const U& uniform_value
    ) {
    set_uniform_by_name(uniform_name, &uniform_value, sizeof(uniform_value));
}

typedef gpu_shader<dx11_vertexshader_traits>            VertexShader_t;

typedef gpu_shader<dx11_fragmentshader_traits>          FragmentShader_t;

typedef gpu_shader<dx11_geometryshader_traits>          GeometryShader_t;

} // namespace directx
} // namespace v8
