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

//! \file default_array_storage.hpp
//! Defines the default array storage policy for scoped_resource class.

namespace v8 { namespace base {

//! \addtogroup ScopedResourceStoragePolicies
//! @{

//!
//! \class default_array_storage
//! \brief Default heap-allocated array policy. Uses operator delete[] to
//! dispose of the memory.
template<typename T>
class default_array_storage {
public :

    //! Type of pointer to object.
    typedef T*                                      stored_type;

    //! Type of object returned from operator->().
    typedef T*                                      pointer_type;

    //! Type of reference to the object.
    typedef T&                                      reference_type;

    //! Fully qualified type of this class.
    typedef default_array_storage<T>              class_type;

    enum {
        is_array = true
    };

protected :

    //! \name Construction.
    //! @{

    default_array_storage()
        : pointee_(null_value()) {}

    default_array_storage(
        stored_type ptr
        )
        : pointee_(ptr) {}

    ~default_array_storage() {}

    //! @}

protected :

    //! \name Pointer specific functions.
    //! @{

    pointer_type operator->() const {
        return pointee_;
    }

    reference_type operator*() const {
        return *pointee_;
    }

    //! @}

protected :

    //! \name Resource retrieval and manipulation.
    //! @{

    static stored_type null_value() {
        return nullptr;
    }

    void set_resource(stored_type ptr) {
        pointee_ = ptr;
    }

    stored_type get_resource() const {
        return pointee_;
    }

    stored_type* get_resource_pointer() {
        return &pointee_;
    }

    void dispose() {
        delete[] pointee_;
    }

    void swap(class_type& rhs) {
        std::swap(pointee_, rhs.pointee_);
    }

    //! @}

protected :

    //! \name Array access
    //! @{

    reference_type at(v8_size_t idx) {
        return pointee_[idx];
    }

    const reference_type at(v8_size_t idx) const {
        return pointee_[idx];
    }

    //! @}

private :

    //! Stored pointer to allocated array of objects.
    stored_type                                     pointee_;
};

//! @}

} // namespace base
} // namespace v8
