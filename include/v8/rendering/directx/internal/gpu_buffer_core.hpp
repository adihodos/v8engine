#pragma once

#include <cassert>
#include <d3d11.h>
#include <v8/v8.hpp>
#include <v8/base/com_exclusive_pointer.hpp>
#include <v8/rendering/directx/constants.hpp>

namespace v8 { namespace directx {

class   renderer;
struct  dx11_vertexbuffer_traits;
struct  dx11_indexbuffer_traits;

//!
//! \brief Common data and routines for buffers.
class gpu_buffer_core {

//! \name Typedefs.
//! @{

protected :

    typedef ID3D11Buffer                                interface_type;
    typedef interface_type*                             interface_pointer;

//! @}

//! \name Construction/init.
//! @{

protected :

    gpu_buffer_core() 
        :       e_count_(0)
            ,   e_size_(0)
            ,   m_buff()
    {}

    gpu_buffer_core(
        renderer* rsys,
        const v8_size_t k_num_elements,
        const v8_size_t k_element_size,
        const v8_uint32_t k_bind_flags,
        const void* k_initial_data = nullptr,
        const v8_uint32_t k_buff_usage = ResourceUsage::Default,
        const v8_uint32_t k_cpu_flags = CPUAccess::None
        )
        :       e_count_(0)
            ,   e_size_(0)
            ,   m_buff()
    {
        initialize(rsys, k_num_elements, k_element_size, k_bind_flags,
                   k_initial_data, k_buff_usage, k_cpu_flags);
    }

    //!
    //! \brief Copy construct from an rvalue.
    gpu_buffer_core(gpu_buffer_core&& rhs) {
        steal_from_rvalue(std::forward<gpu_buffer_core&&>(rhs));
    }

    //!
    //! \brief Assign from an rvalue.
    gpu_buffer_core& operator=(gpu_buffer_core&& rhs) {
        steal_from_rvalue(std::forward<gpu_buffer_core&&>(rhs));
        return *this;
    }

    ~gpu_buffer_core() {}

    v8_bool_t initialize(
        renderer*      rsys,
        const v8_size_t         k_num_elements,
        const v8_size_t         k_element_size,
        const v8_uint32_t       k_bind_flags,
        const void*             k_initial_data = nullptr,
        const v8_uint32_t       k_buff_usage = ResourceUsage::Default,
        const v8_uint32_t       k_cpu_flags = CPUAccess::None
        );

//! @}

public :

    //! \name Sanity checking
    //! @{

    v8_bool_t check_if_valid() const {
        return m_buff != nullptr;
    }

    //! @}

public :

    //! \name Buffer attributes.
    //! @{

    v8_uint32_t get_element_count() const { return e_count_; }

    v8_uint32_t get_element_size() const { return e_size_; }

    v8_uint32_t get_byte_count() const { return e_count_ * e_size_; }

//! @}

//! \name Operations.
//! @{

public :

    //!
    //! \brief Copies the data from the CPU's memory into the buffer.
    //! \param[in] data Pointer to memory containing data.
    //! \param[in] byte_count Number of bytes to copy.
    //! \note The buffer must have been created with the proper write
    //! flags (i.e. not immutable), otherwise this operation will fail.
    void set_data(renderer* rsys, const void* data, const v8_size_t byte_count);

    //!
    //! \brief Copies the data from the GPU into the CPU's memory.
    //! \param[in] byte_count Number of bytes to copy.
    //! \param[in, out] cpy_buffer Pointer to CPU memory where the data is
    //! to be copied.
    //! \note The buffer must have been created with the proper flags
    //! if data transfer from the GPU to the CPU is needed.
    void get_data(renderer* rsys, const v8_size_t byte_count, void* output_buffer);

    inline interface_pointer internal_np_get_handle() const;

protected :
    static void bind_to_pipeline_impl(
        renderer* rsys, 
        gpu_buffer_core* buff,
        const v8_uint32_t k_offset,
        const dx11_vertexbuffer_traits&
        );

    static void bind_to_pipeline_impl(
        renderer* rsys, 
        gpu_buffer_core* buff,
        const v8_uint32_t k_offset,
        const dx11_indexbuffer_traits&
        );

//! @}

private :

    //!
    //! \brief Acquire resources from an rvalue.
    void steal_from_rvalue(gpu_buffer_core&& rhs) {
        e_count_ = rhs.e_count_;
        e_size_ = rhs.e_size_;
        m_buff = v8::base::scoped_pointer_release(rhs.m_buff);
    }

//! \name Data members.
//! @{

private :

    //! Count of elements stored in the buffer.
    v8_uint32_t		e_count_;

    //! Size of a single element.
    v8_uint32_t		e_size_;

    //! Handle to API buffer resource. Owned by the object.
    v8::base::com_exclusive_pointer<interface_type>::type         m_buff;

//! @}

//! \name Disabled functions.
//! @{

private :

    NO_CC_ASSIGN(gpu_buffer_core);

//! @}
};

inline gpu_buffer_core::interface_pointer 
gpu_buffer_core::internal_np_get_handle() const {
    assert(check_if_valid());
    return v8::base::scoped_pointer_get(m_buff);
}

} // namespace directx
} // namespace v8
