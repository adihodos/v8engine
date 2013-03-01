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

#include <utility>
#include <v8/v8.hpp>
#include <v8/base/operator_bool.hpp>
#include <v8/base/resource_policies/default_pointer_storage.hpp>
#include <v8/base/resource_policies/exclusive_ownership.hpp>
#include <v8/base/resource_policies/assert_check.hpp>

namespace v8 { namespace base {

//! \defgroup v8_base_lib   Basic utilities.

//! \defgroup v8_resource_management Resource management classes and utilities.

//! \defgroup   ScopedResourceStoragePolicies
//! \brief Storage policies to customize behaviour of a scoped_resource.
//! Among other, a storage policy is responsible for releasing the resource,
//! when directed to do so.

//! \defgroup   ScopedResourceOwnershipPolicies
//! \brief Ownership policies for a scoped_resource. The ownership policy
//! controls determines when a resource is no longer in use and needs to
//! be released.

//! \defgroup   ScopedResourceCheckingPolicies
//! \brief Checking policies ensure that a resource meets certain criteria,
//! before the user uses it (eg an assert before attempting to deference a
//! pointer resource).

//! \addtogroup v8_base_lib
//! \addtogroup v8_resource_management
//! @{

//!
//! \brief RAII class for resource management.
//! \param T type of resource owned (file handle, mutex, pointer).
//! \param storage_policy Policy class that determines the resource's
//! characteristics and implements its destruction.
//! \param ownership_policy Policy class that determines how the resource
//! is owned (shared via reference counting/ shared via reference linking/
//! exclusive). It also controls the moment of the resource's destruction.
//! \param checking_policy Implements resource sanity testing functions,
//! that are called before the wrapped raw resource is used.
template
<
        typename T,
        template<typename> class storage_policy = default_pointer_storage,
        template<typename> class ownership_policy = exclusive_ownership,
        template<typename> class checking_policy = assert_checking
>
class scoped_resource
    : public storage_policy<T>,
      public ownership_policy<typename storage_policy<T>::stored_type>,
      public checking_policy<typename storage_policy<T>::stored_type>
{
private :

    //! The storage polic class must define these typedefs :
    //! \par
    //! stored_type - an alias for the type of the resource (e.g T* for
    //! pointers).
    //! pointer_type - an alias for the type returned by operator->(). This
    //! must be defined even if operator->() may not be applied to the resource.
    //! reference_type - an alias for the type returned by operator*().This
    //! must be defined even if operator*() may not be applied to the resource.

    typedef storage_policy<T>                               storage_pol;

    typedef ownership_policy
    <
        typename storage_pol::stored_type
    >                                                       ownership_pol;

    typedef checking_policy
    <
        typename storage_pol::stored_type
    >                                                       checking_pol;

public :

    //! Type of this class.
    typedef scoped_resource
    <
        T,
        storage_policy,
        ownership_policy,
        checking_policy
    >                                                       class_type;

    //! Type of raw wrapped resource.
    typedef typename storage_pol::stored_type               stored_type;

    //! Type returned by operator->() (if applicable).
    typedef typename storage_pol::pointer_type              pointer_type;

    //! Type returned by operator*() and operator[] (if applicable).
    typedef typename storage_pol::reference_type            reference_type;

public :

    //! \name Non member utility functions.
    //! @{

    template
    <
            typename U,
            template<typename> class S,
            template<typename> class O,
            template<typename> class C
    >
    friend typename scoped_resource<U, S, O, C>::stored_type
    scoped_res_get(
        const scoped_resource<U, S, O, C>&
        );

    template
    <
            typename U,
            template<typename> class S,
            template<typename> class O,
            template<typename> class C
    >
    friend typename scoped_resource<U, S, O, C>::stored_type*
    scoped_res_get_pointer(
        scoped_resource<U, S, O, C>&
        );

    template
    <
            typename U,
            template<typename> class S,
            template<typename> class O,
            template<typename> class C
    >
    friend void scoped_res_reset(
        scoped_resource<U, S, O, C>&,
        typename scoped_resource<U, S, O, C>::stored_type
        );

    template
    <
            typename U,
            template<typename> class S,
            template<typename> class O,
            template<typename> class C
    >
    friend void swap(
        scoped_resource<U, S, O, C>&,
        scoped_resource<U, S, O, C>&
        );

    //! @}


public :

    //! \name Constructors.
    //! @{

    scoped_resource()
        : storage_pol(),
          ownership_pol(),
          checking_pol() {}


    explicit scoped_resource(stored_type res)
        : storage_pol(res),
          ownership_pol(),
          checking_pol() {}

    scoped_resource(const class_type& rhs)
        : storage_pol(rhs),
          ownership_pol(rhs),
          checking_pol(rhs) {
        storage_pol::set_resource(ownership_pol::clone(scoped_res_get(rhs)));
    }

    template<typename U>
    scoped_resource(const scoped_resource<U,
                                          storage_policy,
                                          ownership_policy,
                                          checking_policy>& convertible)
        : storage_pol(convertible),
          ownership_pol(convertible),
          checking_pol(convertible) {
        storage_pol::set_resource(ownership_pol::clone(scoped_res_get(convertible)));
    }

    ~scoped_resource() {
        dispose_resource();
    }

    //! @}

public :

    //! \name Assignment operators.
    //! @{

    class_type& operator=(const class_type& rhs) {
        class_type temp_object(rhs);
        temp_object.swap(*this);
        return *this;
    }

    template<typename U>
    class_type& operator=(const scoped_resource<U,
                                                storage_policy,
                                                ownership_policy,
                                                checking_policy>& rhs) {
        class_type temp_object(rhs);
        temp_object.swap(*this);
        return *this;
    }

    //! @}

public :

    //! \name Resource access.
    //! @{

    pointer_type operator->() const {
        checking_pol::check_resource(storage_pol::get_resource());
        return storage_pol::operator->();
    }

    reference_type operator*() {
        checking_pol::check_resource(storage_pol::get_resource());
        return storage_pol::operator*();
    }

    const reference_type operator*() const {
        checking_pol::check_resource(storage_pol::get_resource());
        return storage_pol::operator*();
    }

    //! @}

public :

    //! \name Array access.
    //! @{

    reference_type operator[](v8_size_t index) {
        static_assert(storage_pol::is_array,
                      "Resource does not support array access");

        checking_pol::check_resource(storage_pol::get_resource());
        return storage_pol::get_resource[index];
    }

    const reference_type operator[](v8_size_t index) const {
        static_assert(storage_pol::is_array,
                      "Resource does not support array access");

        checking_pol::check_resource(storage_pol::get_resource());
        return storage_pol::get_resource[index];
    }

    //! @}

public :

    //! \name Sanity checking.
    //! @{

    //!
    //! \brief Test if the object owns a valid resource.
    //! \returns True if owned resource is valid, false otherwise.
    operator int operator_bool::*() const {
        return storage_pol::get_resource() == storage_pol::null_value() ?
                    nullptr : &operator_bool::a_member;
    }

    //! @}

public :

    //! \name Equality operators.
    //! @{

    template
    <
        typename T1,
        template<typename> class S1,
        template<typename> class O1,
        template<typename> class C1
    >
    inline v8_bool_t operator==(
            const scoped_resource<T1, S1, O1, C1>& rhs
            ) const {
        return scoped_res_get(*this) == scoped_res_get(rhs);
    }

    template
    <
        typename T1,
        template<typename> class S1,
        template<typename> class O1,
        template<typename> class C1
    >
    inline v8_bool_t operator!=(
            const scoped_resource<T1, S1, O1, C1>& rhs
            ) const {
        return !(*this == rhs);
    }

    //! @}

private :

    void dispose_resource() {
        if (ownership_pol::release(storage_pol::get_resource())) {
            storage_pol::dispose();
        }
    }

    stored_type get_res() const {
        return storage_pol::get_resource();
    }

    stored_type* get_res_ptr() {
        //
        // Release existing resource.
        class_type temp_sp;
        temp_sp.swap(*this);
        return storage_pol::get_resource_pointer();
    }

    void swap(class_type& rhs) {
        storage_pol::swap(rhs);
        ownership_pol::swap(rhs);
        checking_pol::swap(rhs);
    }
};

template
<
    typename T,
    template<typename> class S,
    template<typename> class O,
    template<typename> class C
>
inline typename scoped_resource<T, S, O, C>::stored_type scoped_res_get(
    const scoped_resource<T, S, O, C>& sp
    ) {
    return sp.get_res();
}

template
<
    typename U,
    template<typename> class S,
    template<typename> class O,
    template<typename> class C
>
inline typename scoped_resource<U, S, O, C>::stored_type*
scoped_res_get_pointer(
    scoped_resource<U, S, O, C>& sp
    ) {
    return sp.get_res_ptr();
}

template
<
    typename U,
    template<typename> class S,
    template<typename> class O,
    template<typename> class C
>
inline void scoped_res_reset(
    scoped_resource<U, S, O, C>& sp,
    typename scoped_resource<U, S, O, C>::stored_type value
    ) {
    scoped_resource<U, S, O, C> temp_sp(value);
    temp_sp.swap(sp);
}

template
<
    typename U,
    template<typename> class S,
    template<typename> class O,
    template<typename> class C
>
inline void swap(
    scoped_resource<U, S, O, C>& lhs,
    scoped_resource<U, S, O, C>& rhs
    ) {
    return lhs.swap(rhs);
}

//! \name Equality operators.
//! @{

//template
//<
//    typename T,
//    template<typename> class S,
//    template<typename> class O,
//    template<typename> class C,
//    typename U
//>
//inline bool operator==(
//    const scoped_resource<T, S, O, C>& lhs,
//    U const& rhs
//    ) {
//    return scoped_res_get(lhs) == rhs;
//}

////template
////<
////    typename T,
////    template<typename> class S,
////    template<typename> class O,
////    template<typename> class C,
////    typename U
////>
////inline bool operator==(
////    U const& lhs,
////    const scoped_resource<T, S, O, C>& rhs
////    ) {
////    return rhs == lhs;
////}

//template
//<
//    typename T,
//    template<typename> class S,
//    template<typename> class O,
//    template<typename> class C,
//    typename U
//>
//inline bool operator!=(
//    const scoped_resource<T, S, O, C>& lhs,
//    U const& rhs
//    ) {
//    return !(lhs == rhs);
//}

////template
////<
////    typename T,
////    template<typename> class S,
////    template<typename> class O,
////    template<typename> class C,
////    typename U
////>
////inline bool operator!=(
////    U const& lhs,
////    const scoped_resource<T, S, O, C>& rhs
////    ) {
////    return !(lhs == rhs);
////}

//! @}


//! @}

} // namespace base
} // namespace v8
