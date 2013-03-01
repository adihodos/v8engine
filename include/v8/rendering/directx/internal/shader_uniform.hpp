#pragma once

//!
//! \file directx_shader_uniform.hpp
//! \brief Collection of helper classes for directx shader reflection.

#include <cassert>
#include <string>

#include <d3d11.h>
#include <d3d11shader.h>

#include <v8/v8.hpp>
#include <v8/base/com_exclusive_pointer.hpp>
#include <v8/base/debug_helpers.hpp>
#include <v8/base/pointer_policies.hpp>
#include <v8/base/scoped_pointer.hpp>

#include <v8/base/operator_bool.hpp>
#include <v8/rendering/directx/internal/debug_helpers.hpp>

namespace v8 { namespace directx { namespace internal {
    
//!
//! Stores information about a uniform block defined in a shader.
struct shader_uniform_block_t {

    //! Handle to constant buffer in GPU memory.
    v8::base::com_exclusive_pointer<ID3D11Buffer>::type         handle_;

    //! Pointer to the buffer in CPU memory. 
    v8::base::scoped_ptr<v8_byte_t, v8::base::malloc_storage>   data_storage_;

    //! Block's name.
    std::string                                                 name_;

    //! Block variable count.
    v8_uint32_t                                                 varcount_;

    //! Block's size, in bytes.
    v8_uint32_t                                                 size_;

    //! Binding point (GPU register).
    v8_uint32_t                                                 bind_point_;

    //! True if data in CPU memory is out of sync with data in GPU memory.
    mutable v8_bool_t                                           data_dirty_;

public :
    
    //! \name Constructors
    //! @{
    
    shader_uniform_block_t() 
        :   name_(), 
            varcount_(0), 
            size_(0), 
            data_dirty_(false) {}

    shader_uniform_block_t(shader_uniform_block_t&& rvalue) {
        *this = std::move(rvalue);
    }

    //! @}

    //! \name Operators
    //!
    
    shader_uniform_block_t& operator=(
        shader_uniform_block_t&& rvalue
        ) {
        handle_ = std::move(rvalue.handle_);
        data_storage_ = std::move(rvalue.data_storage_);
        name_ = std::move(rvalue.name_);
        varcount_ = rvalue.varcount_;
        size_ = rvalue.size_;
        data_dirty_ = rvalue.data_dirty_;
        return *this;
    }

    operator int base::operator_bool::*() const {
        return !handle_ ? nullptr : &base::operator_bool::a_member;
    }

    //! @}

public :

    //! \name Attributes
    //! @{
    
    const std::string& get_name() const {
        return name_;
    }
    
    v8_uint32_t get_bind_point() const {
        return bind_point_;
    }

    ID3D11Buffer* get_handle() const {
        return v8::base::scoped_pointer_get(handle_);
    }

    //! @}

public :

    //! \name Operations (initialisation + content manipulation routines)
    //! @}
   
    inline v8_bool_t initialize(
        ID3D11Device* device_context, 
        const D3D11_SHADER_BUFFER_DESC& buff_desc,
        v8_uint32_t bind_point
        );

    inline void sync_data_with_gpu(
        ID3D11DeviceContext* device_context
        ); 

    inline void set_block_component_data(
        const void* component_data, 
        v8_uint32_t component_offset, 
        v8_uint32_t component_size
        );

    template<typename Component_Type>
    inline void set_component_data(
        const Component_Type& component_value,
        v8_uint32_t component_offset
        );

    template<typename ValueType>
    inline void set_block_data(
        const ValueType& value
        );

    //! @}

private :
    NO_CC_ASSIGN(shader_uniform_block_t);
};

inline v8_bool_t shader_uniform_block_t::initialize(
    ID3D11Device* device_context, 
    const D3D11_SHADER_BUFFER_DESC& buff_desc,
    v8_uint32_t bind_point
    ) {

    assert(!handle_);
    assert(!data_storage_);

    name_ = buff_desc.Name;
    varcount_ = buff_desc.Variables;
    size_ = buff_desc.Size;
    data_dirty_ = false;
    bind_point_ = bind_point;

    D3D11_BUFFER_DESC buffer_description = {
        buff_desc.Size,
        D3D11_USAGE_DEFAULT,
        D3D11_BIND_CONSTANT_BUFFER,
        0,
        0,
        buff_desc.Size
    };

    HRESULT ret_code;
    CHECK_D3D(
        &ret_code,
        device_context->CreateBuffer(
            &buffer_description, nullptr,
            v8::base::scoped_pointer_get_impl(handle_)));

    if (FAILED(ret_code))
        return false;

    v8::base::scoped_pointer_reset(
        data_storage_,
        static_cast<v8_byte_t*>(malloc(buff_desc.Size * sizeof(v8_byte_t))));
    return true;
}

inline void shader_uniform_block_t::sync_data_with_gpu(
    ID3D11DeviceContext* device_context
    ) {
    if (data_dirty_) {
        device_context->UpdateSubresource(
            v8::base::scoped_pointer_get(handle_), 0, nullptr, 
            v8::base::scoped_pointer_get(data_storage_), 0, 0);
    }
}

inline void shader_uniform_block_t::set_block_component_data(
    const void* component_data, 
    v8_uint32_t component_offset, 
    v8_uint32_t component_size
    ) {

    assert(component_size <= (size_ - component_offset));

    v8_byte_t* mem_ptr = v8::base::scoped_pointer_get(data_storage_) 
                         + component_offset;
    memcpy(mem_ptr, component_data, component_size);
    data_dirty_ = true;
}

template<typename Component_Type>
inline void shader_uniform_block_t::set_component_data( 
    const Component_Type& component_value, 
    v8_uint32_t component_offset
    ) {
    set_block_component_data(&component_value, component_offset, 
                             sizeof(component_value));
}

template<typename ValueType>
inline void shader_uniform_block_t::set_block_data(const ValueType& val) {
    set_block_component_data(&val, 0, sizeof(val));
}

//!
//! Stores information abount a shader uniform.
struct shader_uniform_t {

    //! Pointer to parent block.
    shader_uniform_block_t*     parent_block_;

    //! Uniform's name.
    std::string                 name_;

    //! Uniform's offset in bytes, from the start of the parent block.
    v8_uint32_t                 start_offset_;

    //! Uniform's size in bytes.
    v8_uint32_t                 size_;

    //! \name Constructors
    //! @{
    
    shader_uniform_t(
        shader_uniform_block_t* parent, 
        const D3D11_SHADER_VARIABLE_DESC var_desc
        )
        : parent_block_(parent),
          name_(var_desc.Name),
          start_offset_(var_desc.StartOffset),
          size_(var_desc.Size) {}

    //! @}

    //! \name Uniform data manipulation
    //! @{

    template<typename DataType>
    inline void set_value(
        const DataType& value
        );

    inline void set_raw_value(
        const void* data, 
        v8_uint32_t byte_count
        );

    //! @}
};

template<typename DataType>
inline void shader_uniform_t::set_value(
    const DataType& value
    ) {
    set_raw_value(&value, sizeof(value));
}

inline void shader_uniform_t::set_raw_value(
    const void* data, 
    v8_uint32_t byte_count
    ) {
    assert(parent_block_);
    assert(byte_count <= size_);
    parent_block_->set_block_component_data(data, start_offset_, byte_count);
}

//!
//! Stores information abount a sampler state in a shader.
struct sampler_state_t {

    //! Sampler's name (sampler's variable name in the shader code).
    std::string             name_;

    //! GPU register that the sampler is bound to.
    v8_uint32_t                bindpoint_;

    sampler_state_t(
        const char* name, 
        v8_uint32_t bind_point
        )
        :   name_(name), 
            bindpoint_(bind_point) {}
};

//!
//! Stores information about a resource view in a shader.
struct resource_view_t {

    //! Resource view's name
    std::string             rvt_name;

    //! GPU register that the RV is bound to.
    v8_uint32_t             rvt_bindpoint;

    //! Dimension info for the RV.
    D3D10_SRV_DIMENSION     rvt_dimension;

    //! Number of RV's. 
    v8_size_t               rvt_count;

    resource_view_t(
        const char* name, 
        v8_uint32_t bind_point, 
        D3D10_SRV_DIMENSION dim, 
        v8_uint32_t count
        ) 
        :   rvt_name(name), 
            rvt_bindpoint(bind_point), 
            rvt_dimension(dim), 
            rvt_count(count) {}
};

} // namespace internal
} // namespace directx
} // namespace v8

