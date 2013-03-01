#pragma once

#include <cassert>
#include <d3d11.h>

#include <v8/v8.hpp>
#include <v8/rendering/directx/internal/debug_helpers.hpp>
#include <v8/base/operator_bool.hpp>

namespace v8 { namespace directx {

//!
//! \brief RAII class to map a GPU resource into the CPU's memory.
//! The mapping is automatically released when the object 
//! goes out of scope. Always test if the mapping was successfull,
//! before trying to write to read/write the memory.
//!
//! \code
//! scoped_resource_mapping tex_data_map(device_ptr, texture_ptr, 
//!                                      D3D11_MAP_WRITE);
//! if (tex_data_map) {
//!     //
//!     // mapping succeeded, write texture data
//!     mempcy(tex_data_map.get_data_pointer(), ...);
//! } else {
//!     //
//!     // handle failure
//! }
//! \endcode
class scoped_resource_mapping {
public :

    //! \name Construction/destruction
    //! @{

    scoped_resource_mapping(
        ID3D11DeviceContext* dev_context,
        ID3D11Resource* resource,
        D3D11_MAP map_options,
        v8_uint32_t subresource_index = 0u
        )
        :   m_device_context(dev_context),
            m_resource(resource),
            m_subresource(subresource_index) {

        memset(&m_mapping, 0, sizeof(m_mapping));

        HRESULT ret_code;
        CHECK_D3D(
            &ret_code, 
            m_device_context->Map(m_resource, m_subresource, map_options,
                                  0, &m_mapping));
    }

    ~scoped_resource_mapping() {
        if (m_mapping.pData) {
            m_device_context->Unmap(m_resource, m_subresource);
        }
    }

    //! @}

public :

    //! \name Sanity checking
    //! @{

    operator int base::operator_bool::*() const {
        return m_mapping.pData ? &base::operator_bool::a_member : nullptr;
    }

    //! @}

public :

    //! \name Memory access
    //! @{

    void* get_data_pointer() const {
        assert(check_valid());
        return m_mapping.pData;
    }

    //! @}

private :

    bool check_valid() const {
        return m_device_context && m_resource && m_mapping.pData;
    }

private :

    ID3D11DeviceContext*        m_device_context;
    ID3D11Resource*             m_resource;
    v8_uint32_t                 m_subresource;
    D3D11_MAPPED_SUBRESOURCE    m_mapping;

private :
    NO_CC_ASSIGN(scoped_resource_mapping);
};

} // namespace directx
} // namespace v8
