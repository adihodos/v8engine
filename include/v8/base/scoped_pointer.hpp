//
// Copyright (c) 2011, 2012, Adrian Hodos
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the author nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR THE CONTRIBUTORS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

//! \file scoped_pointer.hpp
//! \brief Unique owning RAII pointer class and utilities.

#include <v8/v8.hpp>
#include <v8/base/pointer_policies.hpp>
#include <v8/base/operator_bool.hpp>

namespace v8 { namespace base {

//! \addtogroup v8_base_lib
//! \addtogroup v8_resource_management
//! @{


/**
 * \brief A unique owning smart pointer class. It will release the memory
 *  that the pointer points to, using the policy of deallocation specified
 *  by the storage_policy<T> policy. Also supports various degrees
 *  of checking (when accessing the pointer) via the checking_policy<T> policy.
 *  The default storage policy releases the pointer by calling delete. The
 *  default checking policy is to use assertions in debug mode, but no checking
 *  in release mode.
 */
template<
        typename T,
        template<typename> class storage_policy = default_storage,
        template<typename> class checking_policy = assert_check
> class scoped_ptr {
public :

    /** \name Typedefs.
     *  @{
     */

    /** The storage policy determines how the memory is released (via delete,
     *  delete[], free, etc.
     */
    typedef storage_policy<T>                                   spolicy_t;

    /** The checking policy implements pointer sanity checks.
     */
    typedef checking_policy<T>                                  checkpolicy_t;

    /** Type of this class.
     */
    typedef scoped_ptr<T, storage_policy, checking_policy>      class_type;

    /** Type of wrapped pointer.
     */
    typedef T*                                                  pointer;

    typedef T**                                                 pointer_pointer;

    /** Type of wrapped pointer to const.
     */
    typedef const T*                                            const_pointer;

    /** Reference to the pointed object.
     */
    typedef T&                                                  reference;

    /** Const reference to the pointed object.
     */
    typedef const T&                                            const_reference;

    /** @} */

public :

    /** \name Construction.
     *  @{
     */

    scoped_ptr()
        : pointee_(nullptr) {}

    explicit scoped_ptr(T* ptr)
        : pointee_(ptr) {}

    /** @} */

#if defined(V8_COMPILER_HAS_CXX11_MOVE_SEMANTICS)

public :

    /** \name Construction/assignment from temporary.
     *  @{
     */

    /**
     * \brief Assign from a temporary.
     */
    scoped_ptr(class_type&& right) NOEXCEPT
        : pointee_(nullptr) {
        this->swap(right);
    }    

    /**
     * \brief Construct from an rvalue object with a convertible pointer type.
     */
    template<typename U>
    scoped_ptr(scoped_ptr<U, storage_policy, checking_policy>&& right)
        : pointee_(nullptr) {
        this->swap(right);
    }

    /**
     * \brief Construct from a temporary of the same type.
     */
    class_type& operator=(class_type&& right) NOEXCEPT {
        this->swap(right);
        return *this;
    }

    /**
     * \brief Assign from a temporary holding a pointer to a convertible type.
     */
    template<typename U>
    class_type& operator=(
            scoped_ptr<U, storage_policy, checking_policy>&& right
            ) NOEXCEPT {
        this->swap(right);
        return *this;
    }

    /** @} */

#endif /* V8_COMPILER_HAS_CXX11_MOVE_SEMANTICS */

public :

    /** \name Destruction.
     *  @{
     */

    ~scoped_ptr() {
        spolicy_t::dispose(pointee_);
    }

    /** @} */

public :

    /** \name Assignment.
     *  @}
     */

    /**
     * \brief Assign from a raw pointer. 
     * \remarks <b>The objects takes ownership of the raw pointer.
     *          The raw pointer must have been allocated with a method that
     *          is compatible with the smart pointer's storage policy. </b>
     */
    class_type& operator=(T* rhs) {
        if (rhs != pointee_) {
            spolicy_t::dispose(pointee_);
            pointee_ = rhs;
        }
        return *this;
    }

    /** @} */

public :

    /** \name Sanity checking.
     *  @{
     */

    operator int operator_bool::*() const {
        return pointee_ == nullptr ? nullptr : &operator_bool::a_member;
    }

    /** @} */

public :

    /** \name Resource access.
     *  @{
     */

    pointer operator->() const {
        checkpolicy_t::check_ptr(pointee_);
        return pointee_;
    }

    reference  operator*() {
        checkpolicy_t::check_ptr(pointee_);
        return *pointee_;
    }

    const_reference operator*() const {
        checkpolicy_t::check_ptr(pointee_);
        return *pointee_;
    }

    /** @} */

public :

    /** \name Subscripting support.
     *  @{
     */

    /**
     * \brief Implements subscripting, if the pointer points to an array.
     */
    T&  operator[](const v8_size_t index) {
        static_assert(spolicy_t::is_array_ptr,
                      "Subscripting only applies to pointer to array!");
        checkpolicy_t::check_ptr(pointee_);
        return pointee_[index];
    }

    /**
     * \brief Implements subscripting, if the pointer points to an array.
     */
    const T& operator[](const v8_size_t index) const {
        static_assert(spolicy_t::is_array_ptr,
                      "Subscripting only applies to pointer to array!");
        checkpolicy_t::check_ptr(pointee_);
        return pointee_[index];
    }

    /** @} */

public :

    /** \name Non member accessor/manipulation functions.
     *  @{
     */

    /**
     * \brief Explicit access to the owned pointer of a scoped_ptr object.
     * \param Reference to a scoped_ptr object.
     * \return The raw pointer that the scoped_ptr owns.
     */
    template
    <
            typename U,
            template<typename> class V,
            template<typename> class W
    >
    friend U* scoped_pointer_get(
        const scoped_ptr<U, V, W>&
        );

    /**
     * \brief Releases ownerhip of the raw pointer to the caller, who now has the
     *  responsability of freeing the memory, when no longer needed.
     * \param sp Reference to a scoped_ptr object.
     * \return The raw pointer.
     */
    template
    <
            typename U,
            template<typename> class V,
            template<typename> class W
    >
    friend U* scoped_pointer_release(
        scoped_ptr<U, V, W>&
        );

    /**
     * \brief Reset the owned pointer of a scoped_ptr to the new value.
     * \param sp Reference to a scoped_ptr object.
     * \param other The new pointer that the scoped_ptr object will own. This
     *        pointer must have been allocated using the same method as the
     *        original owned pointer.
     * \remarks The old raw pointer owned by the scoped_ptr will be destroyed.
     */
    template
    <
            typename U,
            template<typename> class V,
            template<typename> class W
    >
    friend void scoped_pointer_reset(
        scoped_ptr<U, V, W>&,
        U* other /* = nullptr */
        );

    /**
     * \brief Convenience function to get a pointer to the raw pointer
     *        owned by the scoped_ptr object.
     * \param sp Reference to a scoped_ptr object.
     * \remarks The memory pointed by the pointer owned by the object is
     * first deallocated and the value of the pointer is set to null.
     */
    template
    <
            typename U,
            template<typename> class V,
            template<typename> class W
    >
    friend U** scoped_pointer_get_impl(
        scoped_ptr<U, V, W>&
        );

    /**
     * \brief Swaps contents with another scoped_ptr object.
     */
    template
    <
            typename U,
            template<typename> class V,
            template<typename> class W
    >
    friend void swap(
        scoped_ptr<U, V, W>&,
        scoped_ptr<U, V, W>&
        );

    /** @} */

private :

    /** \name Private accessor/manipulator functions.
     *  @{
     */

    /**
     * @brief get
     * @return Returns owned pointer, after checking by the specified
     * checking policy.
     */
    T* get() const {
        checkpolicy_t::check_ptr(pointee_);
        return pointee_;
    }

    /**
     * @brief release Releases ownership of the owned pointer to the caller.
     * It is the caller's responsability to deallocate the memory.
     * @return Owned pointer.
     */
    T* release() {
        T*  old_val = pointee_;
        pointee_ = nullptr;
        return old_val;
    }

    /**
     * @brief reset Resets the owned pointer to the specified one. Memory
     * pointed by the existing owned pointer is freed. The new pointer
     * must point to memory allocated using a method compatible with the
     * release method specified by the storage policy.
     */
    void reset(T* other) {
        if (pointee_ != other) {
            spolicy_t::dispose(pointee_);
            pointee_ = other;
        }
    }

    /**
     * @brief get_impl Returns a pointer to the owned pointer. Memory
     * pointed by the existing owned pointer is freed.
     * @return Pointer to owned pointer.
     */
    T** get_impl() {
        //
        // Release owned resource.
        class_type tmp_ptr;
        this->swap(tmp_ptr);

        return &pointee_;
    }

    /**
     * \brief Swaps the contents of two scoped_ptr objects.
     */
    void swap(class_type& other) {
        T* t_tmp = scoped_pointer_release(*this);
        T* u_tmp = scoped_pointer_release(other);

        scoped_pointer_reset(other, t_tmp);
        scoped_pointer_reset(*this, u_tmp);
    }

    /** @} */

private :

    /*!< Owned pointer to a block of memeory. */
    T*  pointee_;

private :

    /** \name Disabled functions.
     *  @{
     */

    NO_CC_ASSIGN(scoped_ptr);

    /** @} */

};

template
<
        typename T,
        template<typename> class U,
        template<typename> class W
>
inline bool operator==(
    const T* left,
    scoped_ptr<T, U, W>& right
    ) {
    return left == scoped_pointer_get(right);
}

template
<
        typename T,
        template<typename> class U,
        template<typename> class W
>
inline bool operator!=(
    const T* left,
    scoped_ptr<T, U, W>& right
    ) {
    return !(left == right);
}

template
<
        typename T,
        template<typename> class U,
        template<typename> class W
>
inline bool operator==(
    scoped_ptr<T, U, W>& left,
    const T* right
    ) {
    return right == left;
}

template
<
        typename T,
        template<typename> class U,
        template<typename> class W
>
inline bool operator!=(
    scoped_ptr<T, U, W>& left,
    const T* right
    ) {
    return !(right == left);
}

template
<
        typename U,
        template<typename> class V,
        template<typename> class W
>
inline U* scoped_pointer_get(
    const scoped_ptr<U, V, W>& sp
    ) {
    return sp.get();
}

    
template
<
        typename U,
        template<typename> class V,
        template<typename> class W
>
inline U* scoped_pointer_release(
    scoped_ptr<U, V, W>& sp
    ) {
    return sp.release();
}

template
<
        typename U,
        template<typename> class V,
        template<typename> class W
>
inline void scoped_pointer_reset(
    scoped_ptr<U, V, W>& sp,
    U* other = nullptr
    ) {
    sp.reset(other);
}

template
<
        typename U,
        template<typename> class V,
        template<typename> class W
>
inline U** scoped_pointer_get_impl(
    scoped_ptr<U, V, W>& sp
    ) {
    return sp.get_impl();
}


template
<
        typename T,
        typename U,
        template<typename> class S,
        template<typename> class C
>
inline void swap(
    scoped_ptr<T, S, C>& lhs,
    scoped_ptr<U, S, C>& rhs
    ) {
    return lhs.swap(rhs);
}

template
<
        typename U,
        template<typename> class V,
        template<typename> class W
>
void swap(
    scoped_ptr<U, V, W>& lhs,
    scoped_ptr<U, V, W>& rhs
    )  {
    lhs.swap(rhs);
}

//! @}

} // namespace base 
} // namespace v8
