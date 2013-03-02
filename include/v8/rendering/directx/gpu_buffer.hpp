#pragma once

#include <utility>
#include <v8/v8.hpp>
#include <v8/rendering/directx/fwd_renderer.hpp>
#include <v8/rendering/directx/internal/gpu_buffer_core.hpp>
#include <v8/rendering/directx/constants.hpp>
#include <v8/base/operator_bool.hpp>

namespace v8 { namespace directx {

struct dx11_vertexbuffer_traits {
    enum {
        k_bindval = 0
    };
};

struct dx11_indexbuffer_traits {
    enum {
        k_bindval = 1
    };
};

template<typename buffer_traits>
class gpu_buffer : private gpu_buffer_core {

/// \name Defined types.
/// @{

public :
    typedef gpu_buffer<buffer_traits>                               class_type;

private :
    typedef gpu_buffer_core                                         base_class;

/// @}

/// \name Construction/init.
/// @{

public :

    gpu_buffer()
        :   base_class()
    {}

    gpu_buffer(
        renderer* rsys,
        const v8_size_t k_num_elements,
        const v8_size_t k_element_size,
        const void* k_initial_data = nullptr,
        const v8_uint32_t k_buff_usage = ResourceUsage::Default,
        const v8_uint32_t k_cpu_flags = CPUAccess::None
        )
        : base_class(rsys, k_num_elements, k_element_size, 
                     buffer_traits::k_bindval, k_initial_data, k_buff_usage,
                     k_cpu_flags)
    {}

    gpu_buffer(gpu_buffer&& rval)
        : base_class(std::move(rval))
    {}

    class_type& operator=(class_type&& rval) {
        base_class = std::move(rval);
        return *this;
    }

    ~gpu_buffer() {}

    inline v8_bool_t initialize(
        renderer* rsys,
        const v8_size_t k_num_elements,
        const v8_size_t k_element_size,
        const void* k_initial_data = nullptr,
        const v8_uint32_t k_buff_usage = ResourceUsage::Default,
        const v8_uint32_t k_cpu_flags = CPUAccess::None
        );

/// @}

/// \name Sanity check.
/// @{

public :

    operator int base::operator_bool::*() const {
        return base_class::check_if_valid() ? &base::operator_bool::a_member : nullptr;
    }

/// @}

/// \name Buffer attributes.
/// @{

public :

    using base_class::get_element_count;
    using base_class::get_element_size;
    using base_class::get_byte_count;

/// @}

/// \name Buffer operations.
/// @{

public :

    using base_class::set_data;
    using base_class::get_data;
    using base_class::internal_np_get_handle;

    inline void bind_to_pipeline(
        renderer* rsys, 
        const v8_uint32_t k_offset = 0
        );

/// @}

private :
    NO_CC_ASSIGN(gpu_buffer);
};

template<typename T>
inline v8_bool_t gpu_buffer<T>::initialize(
    renderer*  rsys,
    const v8_size_t     k_num_elements,
    const v8_size_t     k_element_size,
    const void*         k_initial_data /*= nullptr*/,
    const v8_uint32_t   k_buff_usage /*= ResourceUsage::Default*/,
    const v8_uint32_t   k_cpu_flags /*= CPUAccess::None*/
    ) {
    return base_class::initialize(rsys, k_num_elements, k_element_size,
                                  T::k_bindval, k_initial_data, k_buff_usage, 
                                  k_cpu_flags);
}

template<typename buffer_traits>
inline void gpu_buffer<buffer_traits>::bind_to_pipeline( 
    renderer*  rsys, 
    const v8_uint32_t   k_offset /* = 0 */ 
    ) {
    base_class::bind_to_pipeline_impl(rsys, this, k_offset, buffer_traits());
}

typedef gpu_buffer<dx11_vertexbuffer_traits>                    vertex_buffer;

typedef gpu_buffer<dx11_indexbuffer_traits>                     index_buffer;

} // namespace directx
} // namespace v8
