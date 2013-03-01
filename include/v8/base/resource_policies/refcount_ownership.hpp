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

//! \file refcount_ownership.hpp
//! \brief Contains policy classes for reference counted resource ownership.

namespace v8 { namespace base {

//! \addtogroup ScopedResourceOwnershipPolicies
//! @{

//!
//! \class ref_count_st_ownership
//! \brief Single threaded reference counting ownership policy.
template<typename T>
class ref_count_st_ownership {
public :

    v8_size_t* get_ref_count() const {
        return ref_count_;
    }

protected :

    typedef ref_count_st_ownership<T>                   class_type;

    ref_count_st_ownership()
        : ref_count_(new v8_size_t(1)) {}

    ~ref_count_st_ownership() {}

    template<typename convertible_type>
    ref_count_st_ownership(const ref_count_st_ownership<convertible_type>& rhs)
        : ref_count_(rhs.get_ref_count()) {

    }

    T clone(T value) {
        ++*ref_count_;
        return value;
    }

    v8_bool_t release(T) {
        if (!--*ref_count_) {
            delete ref_count_;
            return true;
        }

        return false;
    }

    void swap(class_type& rhs) {
        std::swap(ref_count_, rhs.ref_count_);
    }

private :
    v8_size_t*          ref_count_;

};

//!
//! \class ref_count_intrusive
//! \brief Intrusive reference counting ownership policy. This assumes that
//! the managed resource implements reference counting via two member functions
//! add_ref() and dec_ref().
template<typename T>
class ref_count_intrusive {
protected :

    typedef ref_count_intrusive<T>                      class_type;

    ref_count_intrusive() {}

    template<typename convertible_type>
    ref_count_intrusive(const ref_count_intrusive<convertible_type>&) {}

    T clone(T val) {
        if (val)
            val->add_ref();

        return val;
    }

    v8_bool_t release(T val) {
        return val ? val->dec_ref() : false;
    }

    void swap(class_type&) {}
};

//! @}

} // namespace base
} // namespace v8
