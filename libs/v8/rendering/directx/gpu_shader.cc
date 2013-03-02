#include <third_party/stlsoft/platformstl/filesystem/memory_mapped_file.hpp>

#include "v8/base/com_exclusive_pointer.hpp"
#include "v8/rendering/directx/internal/compute_shader_traits.hpp"
#include "v8/rendering/directx/internal/debug_helpers.hpp"
#include "v8/rendering/directx/internal/geometry_shader_traits.hpp"
#include "v8/rendering/directx/internal/fragment_shader_traits.hpp"
#include "v8/rendering/directx/internal/shader_common_base.hpp"
#include "v8/rendering/directx/internal/shader_uniform.hpp"
#include "v8/rendering/directx/internal/utilities.hpp"
#include "v8/rendering/directx/internal/vertex_shader_traits.hpp"
#include "v8/rendering/directx/renderer.hpp"
#include "v8/rendering/directx/sampler_state.hpp"
#include "v8/rendering/directx/shader_info.hpp"

#include "v8/rendering/directx/gpu_shader.hpp"

namespace v8 { namespace directx {

template<typename U>
struct gpu_shader_private_implementation {
/// \name Defined types.
/// @{

public :
    typedef gpu_shader_private_implementation<U>                class_type;
    typedef U                                                   shader_traits;
    typedef typename shader_traits::interface_type              interface_type;

/// @}

/// \name Construction
/// @{

public :

    gpu_shader_private_implementation();

    ~gpu_shader_private_implementation();

    gpu_shader_private_implementation(class_type&& rval) {
        *this = std::move(rval);
    }

    class_type& operator=(class_type&& rval) {
        interface_ptr = std::move(rval.interface_ptr);
        shader_core = std::move(rval.shader_core);
        is_valid = rval.is_valid;
        return *this;
    }

    v8_bool_t initialize(const shader_info_t& shdr_info, renderer* render_sys);

/// @}


/// \name Data members.
/// @{

public :
    v8_bool_t check_if_valid() const {
        return is_valid;
    }

    ///< Pointer returned by the API.
    typename 
    v8::base::com_exclusive_pointer
    <
        interface_type
    >::type                                                 interface_ptr;

    ///< Data relevant to the shader.
    internal::shader_common_base                           shader_core;

    v8_bool_t                                               is_valid;

/// @}

private :
    NO_CC_ASSIGN(class_type);
};

template<typename U>
gpu_shader_private_implementation<U>::gpu_shader_private_implementation()
    :       interface_ptr()
        ,   shader_core()
        ,   is_valid(false)
{}

template<typename U>
gpu_shader_private_implementation<U>::~gpu_shader_private_implementation() 
{}

template<typename U>
v8_bool_t gpu_shader_private_implementation<U>::initialize(
    const shader_info_t& shdr_info, renderer* render_sys
    ) {
    if (check_if_valid()) {
        assert(false && "Already initialized");
        return true;
    }
    
    //
    // compile shader code
    v8::base::duo<v8_bool_t, ID3D10Blob*> comp_retval(compile_shader(shdr_info));
    if (!comp_retval.First()) {
        return false;
    }

    ID3D11Device* device = render_sys->internal_np_get_device();
    if (!shader_core.reflect_shader(comp_retval.Second(), device)) {
        return false;
    }

    interface_ptr = shader_traits::create_shader(
        device, v8::base::scoped_pointer_get(shader_core.bytecode));
    if (interface_ptr) {
        is_valid = true;
    }

    return is_valid;
}

template<typename T>
v8::directx::gpu_shader<T>::gpu_shader() 
    : pimpl_(new implementation_details()) 
{}

template<typename T>
v8::directx::gpu_shader<T>::~gpu_shader() {}

template<typename T>
v8::directx::gpu_shader<T>::operator int v8::base::operator_bool::*() const {
    return pimpl_->check_if_valid() ? &base::operator_bool::a_member : nullptr;
}

template<typename T>
v8_bool_t v8::directx::gpu_shader<T>::initialize(
    const v8::directx::shader_info_t& shdr_info,
    v8::directx::renderer* render_sys
    ) {
    return pimpl_->initialize(shdr_info, render_sys);
}

template<typename T>
ID3D11InputLayout*
v8::directx::gpu_shader<T>::get_input_signature() const {
    assert(pimpl_->check_if_valid());
    return v8::base::scoped_pointer_get(pimpl_->shader_core.input_signature);
}

template<typename T>
v8_size_t 
v8::directx::gpu_shader<T>::get_uniform_block_count() const {
    assert(pimpl_->check_if_valid());
    return pimpl_->shader_core.uniform_blocks.size();
}

template<typename T>
v8_size_t v8::directx::gpu_shader<T>::get_uniform_var_count() const {
    assert(pimpl_->check_if_valid());
    return pimpl_->shader_core.uniforms.size();
}

template<typename T>
v8_uint32_t v8::directx::gpu_shader<T>::get_uniform_location_by_name(
    const char* uniform_name
    ) const {
    assert(uniform_name);
    assert(pimpl_->check_if_valid());

    auto result = pimpl_->shader_core.get_uniform_by_name(uniform_name);
    return result ? result->start_offset_ : InvalidUniformLocation;
}

template<typename T>
v8_uint32_t v8::directx::gpu_shader<T>::get_uniform_block_location_by_name(
    const char* block_name
    ) const {
    assert(block_name);
    assert(pimpl_->check_if_valid());

    auto result = pimpl_->shader_core.get_uniform_block_by_name(block_name);
    return result ? result->get_bind_point() : InvalidUniformLocation;
}

template<typename data_type>
void set_uniform_block_by_name(
    const char* block_name,
    const data_type& uniform_value
    ) {
        assert(block_name);
        assert(check_valid());

        auto p_block = m_data->get_uniform_block_by_name(block_name);
        if (p_block)
            p_block->set_block_data(uniform_value);
}

template<typename T>
void v8::directx::gpu_shader<T>::set_uniform_by_name(
    const char* uniform_name, const void* uniform_data, v8_size_t byte_count
    ) {
    assert(uniform_name);
    assert(uniform_data);
    assert(pimpl_->check_if_valid());

    auto uniform_ptr = pimpl_->shader_core.get_uniform_by_name(uniform_name);
    if (uniform_ptr) {
        uniform_ptr->set_raw_value(uniform_data, static_cast<v8_uint32_t>(byte_count));
    }
}

template<typename T>
void v8::directx::gpu_shader<T>::set_uniform_block_by_name(
    const char* block_name, 
    const void* block_data, 
    const v8_size_t block_data_size
    ) {
    assert(block_name);
    assert(block_data);
    assert(pimpl_->check_if_valid());

    auto block_ptr = pimpl_->shader_core.get_uniform_block_by_name(block_name);
    if (block_ptr) {
        block_ptr->set_block_component_data(
            block_data, 0U, static_cast<v8_uint32_t>(block_data_size));
    }
}

template<typename T>
void v8::directx::gpu_shader<T>::set_sampler(
    const char* sampler_name, ID3D11SamplerState* sampler_state
    ) {
    assert(sampler_name);
    assert(pimpl_->check_if_valid());

    auto sampler_ptr = pimpl_->shader_core.get_sampler_by_name(sampler_name);
    if (sampler_ptr) {
        pimpl_->shader_core.sampler_state_binding_list[sampler_ptr->bindpoint_] = sampler_state;
    }
}

template<typename T>
void v8::directx::gpu_shader<T>::set_resource_view(
    const char* sampler_name, ID3D11ShaderResourceView* resource_view
    ) {
    assert(sampler_name);
    assert(pimpl_->check_if_valid());

    auto rv_ptr = pimpl_->shader_core.get_rv_by_name(sampler_name);
    if (rv_ptr) {
        pimpl_->shader_core.resource_view_binding_list[rv_ptr->rvt_bindpoint] = resource_view;
    }
}

template<typename T>
void v8::directx::gpu_shader<T>::bind_to_pipeline( 
    v8::directx::renderer* render_sys
    ) {
    assert(render_sys);
    assert(pimpl_->check_if_valid());

    typedef typename implementation_details::shader_traits  shader_traits;

    ID3D11DeviceContext* k_devctx = render_sys->internal_np_get_device_context();
    //
    // Bind uniforms.
    assert(pimpl_->shader_core.uniform_blocks.size() 
            == pimpl_->shader_core.uniform_block_binding_list.size());

    const v8_uint32_t num_uniform_blocks = static_cast<v8_uint32_t>(
        pimpl_->shader_core.uniform_blocks.size());
    if (num_uniform_blocks) {
        //
        // Sync uniform values with values stored in GPU memory
        for (v8_size_t i = 0U; i < num_uniform_blocks; ++i) {
            pimpl_->shader_core.uniform_blocks[i].sync_data_with_gpu(k_devctx);
        }

        shader_traits::bind_uniform_blocks_to_pipeline_stage(
            k_devctx, 0, &pimpl_->shader_core.uniform_block_binding_list[0],
            num_uniform_blocks
            );
    }

    //
    // Bind samplers.
    assert(pimpl_->shader_core.sampler_states.size() 
            == pimpl_->shader_core.sampler_state_binding_list.size());

    const v8_uint32_t num_samplers = static_cast<v8_uint32_t>(
        pimpl_->shader_core.sampler_states.size());
    if (num_samplers) {
        shader_traits::bind_sampler_states_to_pipeline_stage(
            k_devctx, 0, &pimpl_->shader_core.sampler_state_binding_list[0],
            num_samplers
            );
    }

    //
    // Bind resource views.
    assert(pimpl_->shader_core.resource_views.size()
            == pimpl_->shader_core.resource_view_binding_list.size());

    const v8_uint32_t num_rvs = static_cast<v8_uint32_t>(
        pimpl_->shader_core.resource_views.size());
    if (num_rvs) {
        shader_traits::bind_resource_views_to_pipeline_stage(
            k_devctx, 0, &pimpl_->shader_core.resource_view_binding_list[0],
            num_rvs
            );
    }

    //
    // Finally, bind the shader object.
    shader_traits::bind_shader_to_pipeline_stage(
        k_devctx, v8::base::scoped_pointer_get(pimpl_->interface_ptr));
}

} // namespace directx
} // namespace v8

//
// Explicitly instantiate all shader classes below this point.

template 
v8::directx::gpu_shader
<
    v8::directx::dx11_fragmentshader_traits
>;

template 
v8::directx::gpu_shader
<
    v8::directx::dx11_vertexshader_traits
>;

template 
v8::directx::gpu_shader
<
    v8::directx::dx11_geometryshader_traits
>;

template
v8::directx::gpu_shader
<
    v8::directx::dx11_computeshader_traits
>;