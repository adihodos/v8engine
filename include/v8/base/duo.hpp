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

namespace v8 { namespace base {

///
/// \brief Lightweight class that stores a pair of values.
template<typename first_type, typename second_type>
class duo {
public :
/// \name Defined types.
/// @{

public :

    typedef duo<first_type, second_type>                     class_type;
    
/// @}

/// \name Constructors.
/// @{

public :    
    
    duo() : first_(), second_() {}
    
    duo(const first_type& first, const second_type& second) 
        :   first_(first)
            , second_(second) 
    {
    }
    
    ///
    /// \brief Copy constructs from an object with convertible types.
    template<typename F1, typename S1>
    duo(const duo<F1, S1>& convertible)
        :   first_(convertible.First())
            , second_(convertible.Second())
    {
    }

/// @}

/// \name Operations.
/// @{

public :    
    
    ///
    /// \brief Assign from an object with convertible types.
    template<typename F1, typename S1>
    class_type& operator=(const duo<F1, S1>& convertible) {
        first_ = convertible.First();
        second_ = convertible.Second();
        
        return *this;
    }
    
/// @}

/// \name Attributes.
/// @{    
    
public :
    
    ///
    /// \brief RW acces to the first value.
    first_type& First() {
        return first_;
    }
    
    ///
    /// \brief RO access to the first value.
    const first_type& First() const {
        return first_;
    }
    
    ///
    /// \brief RW access to the second value.
    second_type& Second() {
        return second_;
    }
    
    ///
    /// \brief RO access to the second value.
    const second_type& Second() const {
        return second_;
    }
    
/// @}    

private :
    first_type      first_;     ///<    The first stored value.
    second_type     second_;    ///<    The second stored value.
};

template<typename F1, typename S1, typename F2, typename S2>
inline bool operator==(const duo<F1, S1>& lhs, const duo<F2, S2>& rhs) {
    return (lhs.First() == rhs.First()) && (lhs.Second() == rhs.Second());
}

template<typename F1, typename S1, typename F2, typename S2>
inline bool operator!=(const duo<F1, S1>& lhs, const duo<F2, S2>& rhs) {
    return !(lhs == rhs);
}

template<typename T, typename U>
inline duo<T, U> make_duo(const T& first, const U& second) {
    return duo<T, U>(first, second);
}

} // namespace base
} // namespace v8
