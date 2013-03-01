#pragma once

//! \file directx_shader_common_base.hpp
//! \brief Shader common data and functions

#include <cassert>
#include <vector>

#include <d3d11.h>

struct ID3D11ShaderReflection;

#include <v8/v8.hpp>
#include <v8/base/com_exclusive_pointer.hpp>
#include <v8/base/pointer_policies.hpp>
#include <v8/base/scoped_pointer.hpp>
#include <v8/rendering/directx/internal/shader_uniform.hpp>
#include <v8/utility/algorithms.hpp>

namespace v8 { namespace directx { namespace internal {
    
//!
//! \brief Defines data and routines that are common to all types of
//! shaders.
struct shader_common_base {
public :
    
    //!
    //! Shader's compiled bytecode.
    v8::base::com_exclusive_pointer
    <
        ID3D10Blob
    >::type                                             bytecode;

    //!
    //! Shader's associated input parameters signature.
    v8::base::com_exclusive_pointer
    <
        ID3D11InputLayout
    >::type                                             input_signature;

    //! List of uniform blocks in the shader.
    std::vector<shader_uniform_block_t>                 uniform_blocks;

    //! Uniform block handles. Passed to the shader before it starts
    //! execution on the GPU.
    std::vector<ID3D11Buffer*>                          uniform_block_binding_list;

    //! List of each uniform, in the shader.
    std::vector<shader_uniform_t>                       uniforms;

    //! Sampler state info list.
    std::vector<sampler_state_t>                        sampler_states;

    //! Sampler state handles. Passed to the shader before it starts
    //! execution on the GPU.
    std::vector<ID3D11SamplerState*>                    sampler_state_binding_list;

    //! Resource view info list.
    std::vector<resource_view_t>                        resource_views;

    //! Resource view handle list. Passed to the shader before it starts
    //! execution on the GPU.
    std::vector<ID3D11ShaderResourceView*>              resource_view_binding_list;

public :

    //! \name Construction/initialisation.
    //! @{

    //! Default constructor. Does nothing, shader_common_base::reflect_shader() 
    //! must be called before using the object's members.
    shader_common_base() : bytecode() {}

    ~shader_common_base() {}

    shader_common_base(
        shader_common_base&& rrhs
        ) {
        *this = std::move(rrhs);
    }        

    //! Reflects the shader and initializes this object.
    //! \param compiled_bytecode Pointer to an ID3D10Blob interface,
    //! representing the shader's compiled bytecode. The objects assumes
    //! ownership of the pointer.
    //! \param device Pointer to an ID3D11Device interface.
    //! \returns True on success, false on failure.
    v8_bool_t reflect_shader(
        ID3D10Blob* compiled_bytecode,
        ID3D11Device* device
        );

    //! @}

public :

    //! \name Assignment operators
    //! @{

    //! Self assign from a temporary
    shader_common_base& operator=(
        shader_common_base&& rrhs
        ) {
        bytecode = std::move(rrhs.bytecode);
        uniform_blocks = std::move(rrhs.uniform_blocks);
        uniform_block_binding_list = std::move(rrhs.uniform_block_binding_list);
        uniforms = std::move(rrhs.uniforms);
        sampler_states = std::move(rrhs.sampler_states);
        sampler_state_binding_list = std::move(rrhs.sampler_state_binding_list);
        resource_views = std::move(rrhs.resource_views);
        resource_view_binding_list = std::move(rrhs.resource_view_binding_list);
        return *this;
    }

    //! @}

public :

    //! \name Uniform access.
    //! @{

    //! \todo Debug output when querying invalid uniform
    shader_uniform_block_t* get_uniform_block_by_name(
        const char* block_name
        ) {
        return get_resource_info(
            uniform_blocks, 
            [block_name](const shader_uniform_block_t& ublock) {
                return strcmp(ublock.get_name().c_str(), block_name);
        });
    }

    //! \todo Debug output when querying invalid uniform
    shader_uniform_t* get_uniform_by_name(
        const char* uniform_name
        ) {
        return get_resource_info(
            uniforms,
            [uniform_name](const shader_uniform_t& uniform) {
                return strcmp(uniform.name_.c_str(), uniform_name);
        });
    }

    //! Lookup a bound sampler, by name.
    sampler_state_t* get_sampler_by_name(
        const char* sampler_name
        ) {
        return get_resource_info(
            sampler_states,
            [sampler_name](const sampler_state_t& state) {
            return strcmp(state.name_.c_str(), sampler_name);
        });
    }

    //! Lookup a bound resource view, by name.
    resource_view_t* get_rv_by_name(
        const char* rv_name
        ) {
        return get_resource_info(
            resource_views,
            [rv_name](const resource_view_t& state) {
            return strcmp(state.rvt_name.c_str(), rv_name);
        });
    }

    //! @}
    
private :

    //! \name Initialisation
    //! @{

    //! \brief Reflects a constant buffer.
    void reflect_constant_buffer(
        ID3D11Device*   device,
        ID3D11ShaderReflection* shader_reflect,
        v8_uint32_t buff_index
        );


    //! \brief Reflects a resource.
    void reflect_bound_resource(
        ID3D11ShaderReflection* shader_reflector,
        v8_uint32_t index
        );

    void get_input_signature(
        ID3D11Device* k_device,
        ID3D11ShaderReflection* k_reflector,
        ID3D10Blob* k_bytecode,
        const v8_uint32_t k_num_input_params
        );

    //! @}

    template<typename container, typename predicate>
    typename container::value_type* get_resource_info(
        container& cont,
        predicate pr__
        ) {
        auto itr_resource = utility::binary_search_container(
            cont, pr__);
        if (itr_resource == std::end(cont))
            return nullptr;

        return &*itr_resource;
    }

private :
    NO_CC_ASSIGN(shader_common_base);
};

} // namespace internal
} // namespace directx
} // namespace v8
