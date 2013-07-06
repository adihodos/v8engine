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

/**
 * \file scoped_handle.hpp
 * \brief Automated resource management classes and utilities.
 */
 
#include <utility>
#include <v8/v8.hpp>
#include <v8/base/handle_traits.hpp>
#include <v8/base/operator_bool.hpp>

namespace v8 { namespace base {

//! \addtogroup v8_base_lib
//! \addtogroup v8_resource_management
//! @{

/**
 * \brief   RAII class for different OS handles (socket descriptors,
 *          file descriptors, etc...). Ownership is exclusive.
 *          When going out of scope, the wrapped raw handle will be disposed
 *          according to the policy specified at template instantiation.
 * \param   management_policy Defines traits for a raw handle and helper
 *          functions.
 */
template<typename management_policy>
class scoped_handle {
public :

    /** \name Typedefs
     *  @{
     */

    /*!< Handle policy type. */
    typedef management_policy                           mpolicy_t;

    /*!< Alias for the raw handle type. */
    typedef typename mpolicy_t::handle_t                handle_t;

    /*!< Type of pointer to the raw handle type. */
    typedef typename mpolicy_t::handle_ptr_t            handle_ptr_t;

    /*!< Type of reference to the raw handle type. */
    typedef typename mpolicy_t::handle_ref_t            handle_ref_t;

    /*!< Type of constant reference to the raw handle type. */
    typedef typename mpolicy_t::handle_const_ref_t      handle_const_ref_t;

    /*!< Fully qualified type of this class. */
    typedef scoped_handle<management_policy>            class_type;

    /** @} */

public :

    /** \name Constructors.
     *  @{
     */

    /**
     * \brief Default constructor. Initializes object with a non valid handle of
     * the corresponding type.
     */
    scoped_handle()
        : handle_(mpolicy_t::null_handle()) {}

    /**
     * \brief   Initialize with an existing raw handle.
     *          The object assumes ownership of the handle.
     */
    explicit scoped_handle(handle_t new_handle) : handle_(new_handle) {}

#if defined(V8_COMPILER_HAS_CXX11_MOVE_SEMANTICS)

	/**
	 * \brief Construct from a temporary of the same type.
	 */
    scoped_handle(class_type&& right) NOEXCEPT {
        handle_ = right.release();
    }

#endif /* V8_COMPILER_HAS_CXX11_MOVE_SEMANTICS */

    ~scoped_handle() {
        mpolicy_t::dispose(handle_);
    }

    /** @} */

public :

    /** \name Sanity checking.
     *  @{
     */

    /**
     * \brief   Supports sanity checking in the form of if(object) {}.
     */
    operator int operator_bool::*() const {
        return handle_ == mpolicy_t::null_handle() ? nullptr :
            &operator_bool::a_member;
    }

    /** @} */

public :

    /** \name Non-member accessor and utility functions.
     *  @{
     */

    /**
     * \brief Explicit access to the raw handle owned by this object.
     */
    template
    <
        typename M
    >
    friend typename scoped_handle<M>::handle_t scoped_handle_get(
        const scoped_handle<M>&
        );

    /**
     * \brief Release ownership of the native handle to the caller.
     */
    template
    <
        typename M
    >
    friend typename scoped_handle<M>::handle_t scoped_handle_release(
        scoped_handle<M>&
        );

    /**
     * \brief Reset the owned handle to a new value.
     * \remarks The old handle is destroyed before the assignment.
     */
    template
    <
        typename M
    >
    friend void scoped_handle_reset(
        scoped_handle<M>&,
        typename scoped_handle<M>::handle_t
        );

    /**
     * \brief Returns a pointer to the raw handle.
     */
    template
    <
        typename M
    >
    friend typename scoped_handle<M>::handle_ptr_t scoped_handle_get_impl(
        scoped_handle<M>& sh
        );

    /**
     * \brief Swaps contents with another object of this type.
     */
    template
    <
        typename M
    >
    friend void swap(
        scoped_handle<M>&,
        scoped_handle<M>&
        );

    /** @} */

public :

    /** \name Assignment operators.
     *  @{
     */

#if defined(V8_COMPILER_HAS_CXX11_MOVE_SEMANTICS)
    
	/**
	 * \brief Assign operator from a temporary of the same type.
	 */
    class_type& operator=(class_type&& right) NOEXCEPT {
        this->swap(right);
        return *this;
    }

#endif /* V8_COMPILER_HAS_CXX11_MOVE_SEMANTICS */
    
    /**
     * \brief Assign from a raw handle and take ownership of it. 
     */
    class_type& operator=(handle_t new_handle) {
        if (new_handle != handle_) {
            mpolicy_t::dispose(handle_);
            handle_ = new_handle;
        }
        return *this;
    }

    /** @} */

private :

    handle_t    handle_;    /*!< Owned handle */

private :

    /** \name Internal helper functions.
     *  @{
     */

    handle_t get() const {
        return handle_;
    }

    handle_ptr_t get_impl_ptr() {
        return &handle_;
    }

    handle_t release() {
        handle_t tmpHandle = handle_;
        handle_ = mpolicy_t::null_handle();
        return tmpHandle;
    }

    void reset(handle_t newValue) {
        if (handle_ != newValue) {
            mpolicy_t::dispose(handle_);
            handle_ = newValue;
        }
    }

    void swap(class_type& rhs) {
        std::swap(handle_, rhs.handle_);
    }

    /** @} */

private :

    /** \name Disabled functions.
     *  @{
     */

    NO_CC_ASSIGN(scoped_handle);

    /** @} */
};

/**
 * \brief Accessor function.
 */
template<typename M>
inline typename scoped_handle<M>::handle_t scoped_handle_get(
    const scoped_handle<M>& sh
    ) {
    return sh.get();
}

/**
 * \brief   Releases ownership of the handle to the caller. The caller is
 *          responsible for closing the handle when no longer needed.
 */
template<typename M> 
inline typename scoped_handle<M>::handle_t scoped_handle_release(
    scoped_handle<M>& sh
    ) {
    return sh.release();
}

/**
 * \brief   Resets the owned handle to the specified one. The old handle is
 *          released in the process.
 */
template<typename M>
inline void scoped_handle_reset(
    scoped_handle<M>& sh, 
    typename scoped_handle<M>::handle_t new_value = M::null_handle()
    ) {
    sh.reset(new_value);
}

/**
 * \brief   Returns a pointer to the owned handle. The existing handle is
 *          closed. This is a convenience function that exists because
 *          of API's that return handles to the caller via pointers.
 * \code
 *  v8::base::scoped_handle<my_handle_policy> h;
 *  api_get_handle(scoped_handle_get_impl(h));
 *  if (!h) {
 *      ...
 *  }
 * \code
 */
template<typename M>
inline typename scoped_handle<M>::handle_ptr_t scoped_handle_get_impl(
    scoped_handle<M>& sh
    ) {
    scoped_handle_reset(sh);
    return sh.get_impl_ptr();
}

/**
 * \brief   Equality test between raw handle and scoped_handle object.
 */
template<typename T>
inline bool operator==(
    const typename T::handle_t& left,
    const scoped_handle<T>& right
    ) {
    return left == scoped_handle_get(right);
}

/**
 * \brief   Equality test between raw handle and scoped_handle object.
 */

template<typename T>
inline bool operator==(
    const scoped_handle<T>& left,
    const typename T::handle_t& right
    ) {
    return right == left;
}

/**
 * \brief   Inequality test between raw handle and scoped_handle object.
 */
template<typename T>
inline bool operator!=(
    const typename T::handle_t& left,
    const scoped_handle<T>& right
    ) {
    return !(left == right);
}

/**
 * \brief   Inequality test between raw handle and scoped_handle object.
 */
template<typename T>
inline bool operator!=(
    const scoped_handle<T>& left,
    const typename T::handle_t& right
    ) {
    return right != left;
}

/**
 * \brief   Swaps the owned handles of two scoped_handle objects.
 */
template<typename M>
inline void swap(
    scoped_handle<M>& left,
    scoped_handle<M>& right
    ) {
    left.swap(right);
}

//! @}

} // namespace base
} // namespace v8
