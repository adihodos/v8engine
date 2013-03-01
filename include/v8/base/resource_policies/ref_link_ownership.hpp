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
#include <v8/base/resource_policies/ref_link_base.hpp>

//! \file ref_link_ownership.hpp
//! \brief Contains policy classes for reference linked resource ownership.

namespace v8 { namespace base {

//! \addtogroup ScopedResourceOwnershipPolicies
//! @{

//!
//! \brief ref_linked_ownership
//! \brief Allows resource sharing via reference linking.
template<typename T>
class ref_linked_ownership : public internal::ref_linked_base {
public :

    //! Complete type of this class.
    typedef ref_linked_ownership<T>                     class_type;

    //! Type of base class.
    typedef internal::ref_linked_base                   base_class;

protected :

    ref_linked_ownership()
        : base_class() {}

    ref_linked_ownership(const class_type& rhs)
        : base_class(rhs) {
    }

    template<typename convertible_type>
    ref_linked_ownership(const ref_linked_ownership<convertible_type>& rhs)
        : base_class(rhs) {}

    T clone(T val) {
        return val;
    }

    v8_bool_t release(T) {
        return base_class::release();
    }

    void swap(class_type& rhs) {
        base_class::swap(rhs);
    }

    template<typename convertible_type>
    void swap(ref_linked_ownership<convertible_type>& rhs) {
        base_class::swap(rhs);
    }
};

//! @}

} // namespace base
} // namespace v8
