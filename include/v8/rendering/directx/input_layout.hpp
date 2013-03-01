#pragma once

#include <d3d11.h>

#include <v8/v8.hpp>
#include <v8/base/com_exclusive_pointer.hpp>
#include <v8/base/operator_bool.hpp>
#include <v8/rendering/directx/constants.hpp>
#include <v8/rendering/directx/fwd_renderer.hpp>

namespace v8 { namespace directx {

struct input_element_descriptor {
    ElementType::Type   type;
    v8_int_t            count;
    v8_int_t            slot_num;
    const char*         semantic;

    input_element_descriptor() {}

    input_element_descriptor(
        const ElementType::Type tp, 
        const v8_int_t cnt,
        const v8_int_t slot,
        const char* const sema
        )
    :       type(tp)
        ,   count(cnt)
        ,   slot_num(slot)
        ,   semantic(sema)
    {}
}; 

///
/// \brief Layout data for a shader.
class input_layout {
/// \name Defined types.
/// @{

public :

    typedef ID3D11InputLayout*                                  handle_type;

/// @}    

/// \name Constructors.
/// @{

public :

    input_layout();

    input_layout(
        const input_element_descriptor* descriptors, 
        const v8_uint32_t num_descriptors,
        renderer* rsys,
        const void* shader_bytecode,
        const v8_uint32_t bytecode_size
        ) {
        initialize(descriptors, num_descriptors, rsys, shader_bytecode, bytecode_size);
    }

    ~input_layout();

/// @}

/// \name Operations
/// @{    

public :

    operator int base::operator_bool::*() const {
        return check_if_valid() ? &base::operator_bool::a_member : nullptr;
    }

    v8_bool_t initialize(
        const input_element_descriptor* descriptors, 
        const v8_uint32_t num_descriptors,
        renderer* rsys,
        const void* shader_bytecode,
        const v8_uint32_t bytecde_size
        );

    handle_type internal_np_get_handle() const {
        return base::scoped_pointer_get(handle_);
    }

    void bind_to_pipeline(renderer* rsys);

/// @}    

private :

    v8_bool_t check_if_valid() const {
        return handle_ != nullptr;
    }

private :

    v8::base::com_exclusive_pointer
    <
        ID3D11InputLayout
    >::type                                                           handle_;

private :
    NO_CC_ASSIGN(input_layout);
};

} // namespace directx
} // namespace v8
