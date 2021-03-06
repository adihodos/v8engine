#pragma once

#include <d3d11.h>
#include <v8/v8.hpp>
#include <v8/rendering/directx/internal/debug_helpers.hpp>

namespace v8 { namespace directx {

///
/// \brief compute shader function and types.
struct dx11_computeshader_traits {
    typedef ID3D11ComputeShader                         interface_type;
    typedef interface_type*                             interface_pointer;

    static const char* const                           Profile_Strings[];

    static inline interface_pointer create_shader(
        ID3D11Device* device, ID3D10Blob* compiledBytecode
        );

    static inline void bind_shader_to_pipeline_stage(
        ID3D11DeviceContext* device_context, interface_pointer compute_shader
        );

    static inline void bind_uniform_blocks_to_pipeline_stage(
        ID3D11DeviceContext* device_context,
        v8_uint32_t start_slot,
        ID3D11Buffer* const * buffers,
        v8_uint32_t num_buffers
        );

    static inline void bind_sampler_states_to_pipeline_stage(
        ID3D11DeviceContext* device_context,
        v8_uint32_t start_slot,
        ID3D11SamplerState* const* sampler_states,
        v8_uint32_t num_states
        );

    static inline void bind_resource_views_to_pipeline_stage(
        ID3D11DeviceContext* device_context,
        v8_uint32_t start_slot,
        ID3D11ShaderResourceView* const* resource_views,
        v8_uint32_t num_rvs
        );
};

inline dx11_computeshader_traits::interface_pointer
    dx11_computeshader_traits::create_shader(
    ID3D11Device* device, ID3D10Blob* compiledBytecode
    ) {
        HRESULT ret_code;
        interface_pointer p_shader = nullptr;

        CHECK_D3D(
            &ret_code,
            device->CreateComputeShader(compiledBytecode->GetBufferPointer(),
                                        compiledBytecode->GetBufferSize(), 
                                        nullptr, &p_shader));
        return p_shader;
}

inline void dx11_computeshader_traits::bind_shader_to_pipeline_stage(
    ID3D11DeviceContext* device_context, interface_pointer compute_shader
    ) {
    device_context->CSSetShader(compute_shader, nullptr, 0);
}

inline void dx11_computeshader_traits::bind_uniform_blocks_to_pipeline_stage(
    ID3D11DeviceContext* device_context, 
    v8_uint32_t start_slot, 
    ID3D11Buffer* const * buffers, 
    v8_uint32_t num_buffers
    ) {
    device_context->CSSetConstantBuffers(start_slot, num_buffers, buffers);
}

inline void dx11_computeshader_traits::bind_sampler_states_to_pipeline_stage(
    ID3D11DeviceContext* device_context, 
    v8_uint32_t start_slot, 
    ID3D11SamplerState* const* sampler_states, 
    v8_uint32_t num_states
    ) {
    device_context->CSSetSamplers(start_slot, num_states, sampler_states);
}

inline void dx11_computeshader_traits::bind_resource_views_to_pipeline_stage( 
    ID3D11DeviceContext* device_context, 
    v8_uint32_t start_slot, 
    ID3D11ShaderResourceView* const* resource_views, 
    v8_uint32_t num_rvs
    ) {
    device_context->CSSetShaderResources(start_slot, num_rvs, resource_views);
}

} // namespace directx
} // namespace v8
