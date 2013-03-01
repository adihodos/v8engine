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

#include <v8/v8.hpp>
#include <v8/base/duo.hpp>

namespace v8 { namespace base {

///
/// Lightweight class that stores a triplet of values.
template<typename first_type, typename second_type, typename third_type>
class triple : public duo<first_type, second_type> {
public :

/// \name Defined types.
/// @{

public :

    typedef triple<first_type, second_type, third_type>             class_type;

    typedef duo<first_type, second_type>                            base_class;

/// @}    

/// \name Constructors.
/// @{

public :

    triple()
        :   base_class()
            , third_() 
    {}    

    triple(
           const first_type& first, 
           const second_type& second, 
           const third_type& third
           )
        :   base_class(first, second)
            , third_(third)
    {}

    template<typename F1, typename S1, typename T1>
    triple(const triple<F1, S1, T1>& convertible)
        :   base_class(convertible)
            , third_(convertible.Third())
    {}

/// @}    

/// \name Assignment operators.
/// @{

public :    

    template<typename F1, typename S1, typename T1>
    class_type& operator=(const triple<F1, S1, T1>& convertible) {
        base_class::operator=(convertible);
        third_ = convertible.Third();

        return *this;
    }

/// @}

/// \name Attributes.
/// @{

public :    

    third_type& Third() {
        return third_;
    }

    const third_type& Third() const {
        return third_;
    }

/// @}    

private :

    third_type  third_;
}; 

template
<
    typename F1, typename S1, typename T1,
    typename F2, typename S2, typename T2
>
inline bool operator==(
    const triple<F1, S1, T1>& lhs,
    const triple<F2, S2, T2>& rhs
    ) {
    return (lhs.First() == rhs.First()) 
           && (lhs.Second() == rhs.Second())
           && (lhs.Third() == rhs.Third());
}

template
<
    typename F1, typename S1, typename T1,
    typename F2, typename S2, typename T2
>
inline bool operator!=(
    const triple<F1, S1, T1>& lhs,
    const triple<F2, S2, T2>& rhs
    ) {
    return !(lhs == rhs);
}

template<typename F, typename S, typename T>
inline triple<F, S, T> make_triple(
    const F& first, 
    const S& second, 
    const T& third
    ) {
    return triple<F, S, T>(first, second, third);
}

} // namespace base
} // namespace v8
