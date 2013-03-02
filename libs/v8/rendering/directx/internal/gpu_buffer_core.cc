#include "v8/rendering/directx/internal/constants_mappings.hpp"
#include "v8/rendering/directx/internal/debug_helpers.hpp"
#include "v8/rendering/directx/renderer.hpp"
#include "v8/rendering/directx/scoped_gpu_resource_mapping.hpp"

#include "v8/rendering/directx/internal/gpu_buffer_core.hpp"

namespace {

const D3D11_BIND_FLAG k_dx11_bind_flag_mappings[] = {
    D3D11_BIND_VERTEX_BUFFER,
    D3D11_BIND_INDEX_BUFFER,
    D3D11_BIND_CONSTANT_BUFFER
};

}

v8_bool_t v8::directx::gpu_buffer_core::initialize(
    renderer*      rsys,
    const v8_size_t         k_num_elements,
    const v8_size_t         k_element_size,
    const v8_uint32_t       k_bind_flags,
    const void*             k_initial_data /*= nullptr*/,
    const v8_uint32_t       k_buff_usage /*= ResourceUsage::Default*/,
    const v8_uint32_t       k_cpu_flags /*= CPUAccess::None*/
    ) {
    assert(!m_buff);

    e_count_    = static_cast<v8_uint32_t>(k_num_elements);
    e_size_     = static_cast<v8_uint32_t>(k_element_size);

    UINT cpu_access_flags = 0;
    if (k_cpu_flags & CPUAccess::Read) {
        cpu_access_flags |= D3D11_CPU_ACCESS_READ;
    }
    if (k_cpu_flags & CPUAccess::Write) {
        cpu_access_flags |= D3D11_CPU_ACCESS_WRITE;
    }

    D3D11_BUFFER_DESC buffer_desc = {
        e_count_ * e_size_,
        internal::kResourceUsageMappings[k_buff_usage],
        k_dx11_bind_flag_mappings[k_bind_flags],
        internal::kCPUAccessMappings[k_cpu_flags],
        0,
        0
    };

    D3D11_SUBRESOURCE_DATA buff_data = { k_initial_data, 0, 0 };
    HRESULT ret_code;
    ID3D11Device* k_device = rsys->internal_np_get_device();

    CHECK_D3D(
        &ret_code,
        k_device->CreateBuffer(&buffer_desc, &buff_data, 
                               v8::base::scoped_pointer_get_impl(m_buff)));

    return SUCCEEDED(ret_code);
}

void v8::directx::gpu_buffer_core::set_data(
    renderer* rsys, const void* data, const v8_size_t byte_count
    ) {
    assert(m_buff);
    assert(byte_count <= (e_count_ * e_size_));

    ID3D11DeviceContext* k_devcontext = rsys->internal_np_get_device_context();

    scoped_resource_mapping data_store(
        k_devcontext, v8::base::scoped_pointer_get(m_buff), D3D11_MAP_WRITE_DISCARD);

    if (!data_store) {
        return;
    }

    memcpy(data_store.get_data_pointer(), data, byte_count);
}

void v8::directx::gpu_buffer_core::get_data(
    renderer* rsys, const v8_size_t byte_count, void* output_buffer
    ) {
    assert(m_buff);
    assert(byte_count <= (e_count_ * e_size_));

    ID3D11DeviceContext* k_devcontext = rsys->internal_np_get_device_context();

    scoped_resource_mapping data_store(
        k_devcontext, v8::base::scoped_pointer_get(m_buff), D3D11_MAP_READ);

    if (data_store) {
        memcpy(output_buffer, data_store.get_data_pointer(), byte_count);
    }
}

void v8::directx::gpu_buffer_core::bind_to_pipeline_impl(
    renderer*                           rsys, 
    gpu_buffer_core*                    buff,
    const v8_uint32_t                   k_offset,
    const dx11_vertexbuffer_traits&
    ) {
    assert(rsys);
    assert(buff);

    ID3D11DeviceContext* k_devcontext = rsys->internal_np_get_device_context();
    ID3D11Buffer* k_vbuff = buff->internal_np_get_handle();
    const UINT k_strides = buff->get_element_size();
    k_devcontext->IASetVertexBuffers(0, 1, &k_vbuff, &k_strides, &k_offset);
}

void v8::directx::gpu_buffer_core::bind_to_pipeline_impl(
    renderer*                  rsys, 
    gpu_buffer_core*                    buff,
    const v8_uint32_t                   k_offset,
    const dx11_indexbuffer_traits&
    ) {
    assert(rsys);
    assert(buff);
    assert((buff->get_element_size() == sizeof(v8_uint16_t)) 
          || (buff->get_element_size() == sizeof(v8_uint32_t)));

    const DXGI_FORMAT k_index_format[] = { 
        DXGI_FORMAT_R16_UINT, DXGI_FORMAT_R32_UINT
    };

    const v8_int_t k_typeidx = buff->get_element_size() == sizeof(v8_uint32_t);
    ID3D11DeviceContext* k_devctx = rsys->internal_np_get_device_context();
    ID3D11Buffer* k_ibuff = buff->internal_np_get_handle();
    k_devctx->IASetIndexBuffer(k_ibuff, k_index_format[k_typeidx], k_offset);
}
